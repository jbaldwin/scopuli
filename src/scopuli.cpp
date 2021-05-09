#include "scopuli/scopuli.hpp"

#include <chain/chain.hpp>
#include <spdlog/spdlog.h>

namespace scopuli::http {

server::server(coro::net::ip_address ip_addr, uint16_t port, routes_type routes,
               processing_method pm)
    : m_ip_addr(std::move(ip_addr)), m_port(port), m_routes(std::move(routes)) {
  size_t scheduler_count = 1;
  auto execution_strategy =
      coro::io_scheduler::execution_strategy_t::process_tasks_on_thread_pool;

  if (pm == processing_method::process_requests_inline) {
    execution_strategy =
        coro::io_scheduler::execution_strategy_t::process_tasks_inline;
    scheduler_count = std::thread::hardware_concurrency();
  }

  for (size_t i = 0; i < scheduler_count; ++i) {
    m_io_schedulers.push_back(
        std::make_unique<coro::io_scheduler>(coro::io_scheduler::options{
            .thread_strategy = coro::io_scheduler::thread_strategy_t::spawn,
            .on_io_thread_start_functor = nullptr,
            .on_io_thread_stop_functor = nullptr,
            .pool =
                coro::thread_pool::options{.on_thread_start_functor = nullptr,
                                           .on_thread_stop_functor = nullptr},
            .execution_strategy = execution_strategy}));

    m_accept_tasks.emplace_back(
        make_accept_task(*m_io_schedulers.back().get(), ip_addr, port));
  }

  for (auto &task : m_accept_tasks) {
    task.resume();
  }
}

server::~server() { shutdown(); }

auto server::shutdown() -> void {
  if (m_shutdown.exchange(true, std::memory_order::acq_rel) == false) {
    for (auto &scheduler : m_io_schedulers) {
      scheduler->shutdown();
    }
  }
}

auto server::make_accept_task(coro::io_scheduler &scheduler,
                              coro::net::ip_address ip_addr, uint16_t port)
    -> coro::task<void> {
  static std::atomic<uint64_t> execute_counter{0};
  uint64_t my_id = execute_counter++;
  spdlog::info("make_accept_task start({})", my_id);

  co_await scheduler.schedule();

  coro::net::tcp_server tcp_server{
      scheduler,
      coro::net::tcp_server::options{.address = ip_addr, .port = port}};

  coro::task_container<coro::io_scheduler> live_web_requests{scheduler};

  while (!m_shutdown.load(std::memory_order::acquire)) {
    auto pstatus = co_await tcp_server.poll(std::chrono::seconds{1});
    if (pstatus == coro::poll_status::timeout) {
      continue;
    }

    if (pstatus == coro::poll_status::event) {
      // spdlog::info("co_await tcp_server.poll() event");
      auto tcp_client = tcp_server.accept();
      if (tcp_client.socket().is_valid()) {
        live_web_requests.start(make_web_request_task(std::move(tcp_client)));
      } else {
        spdlog::error(
            "scopuli::http::server error accepting incoming connection");
      }
    } else {
      spdlog::error("scopuli::http::server accept socket poll error");
    }
  }

  while (!live_web_requests.empty()) {
    spdlog::info(
        "scopuli::http::server waiting for all web requests to complete...");
    co_await live_web_requests.garbage_collect_and_yield_until_empty();
    spdlog::info("scopuli::http::server all web requests completed.");
  }

  spdlog::info("make_accept_task end({})", my_id);

  co_return;
}

auto server::make_web_request_task(coro::net::tcp_client tcp_client)
    -> coro::task<void> {
  static std::atomic<uint64_t> execute_counter{0};
  uint64_t my_id = execute_counter++;
  spdlog::info("make_web_request_task start({})", my_id);

  std::string input_buffer(16384, '\0');
  // The view that can safely have new bytes recv()'ed into.  This is a sliding
  // window across the input buffer.
  std::span<char> recv_view{input_buffer.data(), input_buffer.size()};
  // The view of bytes that have been read so far.
  std::span<char> http_request_view{input_buffer.data(), 0};

  turbo::http::request request_parser{};

  bool keep_alive{true};

  // TODO: timeout should subtract used time?  Right now each operation resets
  // to maximum timeout.

  while (keep_alive) {
    auto pstatus = co_await tcp_client.poll(coro::poll_op::read, m_timeout);
    switch (pstatus) {
    case coro::poll_status::event: {
      auto [rstatus, rspan] = tcp_client.recv(recv_view);
      // Append the just read bytes into the http_request_view.
      http_request_view = std::span<char>{
          input_buffer.data(), http_request_view.size() + rspan.size()};

      if (rstatus == coro::net::recv_status::ok) {
        auto parse_result = request_parser.parse(http_request_view);

        switch (parse_result) {
        case turbo::http::request_parse_result::complete: {
          request req{tcp_client, request_parser};
          response rep{req};
          route(req, rep);

          auto response_data = rep.serialized();

          auto response_view =
              std::span<const char>{response_data.data(), response_data.size()};

          // Flush the response to the client.
          do {
            auto [send_status, remaining] = tcp_client.send(response_view);
            if (send_status != coro::net::send_status::ok) {
              spdlog::error("web_requst error sending data to client [{}]",
                            static_cast<int64_t>(send_status));
              keep_alive = false;
              break;
            } else {
              if (remaining.empty()) {
                break;
              } else {
                response_view = remaining;

                auto pstatus =
                    co_await tcp_client.poll(coro::poll_op::write, m_timeout);
                if (pstatus != coro::poll_status::event) {
                  spdlog::error("web_request error polling for write [{}]",
                                pstatus);
                  keep_alive = false;
                }
              }
            }

          } while (!response_view.empty());

          if (keep_alive) {
            // Check to see if the request wants to keep the connection alive.
            auto connection_header = req.header("connection");
            if (connection_header.has_value() &&
                chain::str::equal<chain::str::case_t::insensitive>(
                    connection_header.value(), "keep-alive")) {
              // Reset inputs and wait for another request.
              request_parser.reset();
              recv_view =
                  std::span<char>{input_buffer.data(), input_buffer.size()};
              http_request_view = std::span<char>{input_buffer.data(), 0};
            } else {
              keep_alive = false;
            }
          }
        } break;
        case turbo::http::request_parse_result::incomplete: {
          // If the view of the data matches the full buffer size then
          // re-allocate space.
          if (http_request_view.size() == input_buffer.size()) {
            input_buffer.resize(input_buffer.size() * 2);
            // Reset views into the buffer, the 2nd half can no be recv()'ed
            // into.
            recv_view =
                std::span<char>{input_buffer.data() + input_buffer.size() / 2,
                                input_buffer.size() / 2};
            http_request_view =
                std::span<char>{input_buffer.data(), http_request_view.size()};
          }
          // else there is buffer space left, just wait for more bytes.
        } break;
        default:
          spdlog::error("web_request error parsing {}", parse_result);
          keep_alive = false; // This will exit the loop.
          // any other state is a fatal error
          break;
        }
      } else {
        spdlog::error("web request recv() data error [{}]",
                      coro::net::to_string(rstatus));
        keep_alive = false;
      }
    } break;
    case coro::poll_status::timeout:
      spdlog::error("web_request timed out waiting to recv data");
      keep_alive = false;
      break;
    case coro::poll_status::error:
      spdlog::error("web_request had a socket erro waiting to recv data");
      keep_alive = false;
      break;
    case coro::poll_status::closed:
      spdlog::error("web_request client closed the connection");
      keep_alive = false;
      break;
    }
  }

  spdlog::info("make_web_request_task end({})", my_id);

  co_return;
}

auto server::route(const request &req, response &rep) -> void {
  // TODO complete routing, for now forcing a single callback for each web
  // request.

  m_routes(req, rep);
}

} // namespace scopuli::http
