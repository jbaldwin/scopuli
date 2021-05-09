#pragma once

#include <coro/coro.hpp>
#include <scopuli/status_code.hpp>
#include <turbohttp/turbohttp.hpp>

#include <functional>
#include <memory>

namespace scopuli
{
enum class processing_method
{
    /// Process HTTP requests on back threads via a FIFO queue.
    process_requests_on_thread_pool,
    /// Process HTTP requests inline with a thread per core.
    process_requests_inline,
};

namespace http
{
class request
{
public:
    request(const coro::net::tcp_client& tcp_client, const turbo::http::request<>& request_data)
        : m_tcp_client(tcp_client),
          m_request_data(request_data)
    {
    }
    ~request() = default;

    auto method() const -> turbo::http::method { return m_request_data.http_method(); }

    auto uri() const -> std::string_view { return m_request_data.http_uri(); }

    auto version() const -> turbo::http::version { return m_request_data.http_version(); }

    auto header(std::string_view name) const -> std::optional<std::string_view>
    {
        return m_request_data.http_header(name);
    }

    auto body() const -> const std::optional<std::string_view>& { return m_request_data.http_body(); }

private:
    const coro::net::tcp_client&  m_tcp_client;
    const turbo::http::request<>& m_request_data;
};

class response
{
public:
    response(const request& req, size_t response_size_bytes = 4096) : m_request(req)
    {
        m_data.reserve(response_size_bytes);

        m_data.append("HTTP/");
        m_data.append(turbo::http::to_string(m_request.version()));
    }
    ~response() = default;

    auto set_status_code(status_code status) -> void
    {
        m_data.append(" ");
        m_data.append(to_string(status));
        m_data.append("\r\n");
    }

    auto add_header(std::string_view name, std::string_view value) -> void
    {
        m_data.append(name);
        m_data.append(": ");
        m_data.append(value);
        m_data.append("\r\n");
    }

    auto complete_headers() -> void { m_data.append("\r\n"); }

    auto append_body(std::string_view data) -> void
    {
        // this is _ok_ for small bodies, but won't scale well on large bodies,
        // writev would be far more efficient I think.
        m_data.append(data);
    }

    auto serialized() const -> const std::string& { return m_data; }

private:
    const request& m_request;

    // TODO make this api friendly and reasonably fast, probably need a
    // sendv/writev in libcoro?

    // status_code m_status_code{status_code::http_unknown};
    // std::vector<std::pair<std::string, std::string>> m_headers;
    // std::string m_body;

    std::string m_data;
};

class server
{
public:
    using request_handler = std::function<void(const request& req, response& rep)>;

    // using routes_type = std::map<std::string, request_handle>;
    using routes_type = request_handler;

    server(
        coro::net::ip_address ip_addr,
        uint16_t              port,
        routes_type           routes,
        processing_method     pm = processing_method::process_requests_on_thread_pool);

    ~server();

    server(const server&) = delete;
    server(server&&)      = delete;

    auto operator=(const server&) -> server& = delete;
    auto operator=(server&&) -> server& = delete;

    auto shutdown() -> void;

private:
    const coro::net::ip_address m_ip_addr;
    const uint16_t              m_port;

    std::atomic<bool> m_shutdown{false};

    std::vector<std::unique_ptr<coro::io_scheduler>> m_io_schedulers{};
    std::vector<coro::task<void>>                    m_accept_tasks{};

    std::chrono::milliseconds m_timeout{30'000};

    routes_type m_routes;

    auto make_accept_task(coro::io_scheduler& scheduler, coro::net::ip_address ip_addr, uint16_t port)
        -> coro::task<void>;

    auto make_web_request_task(coro::net::tcp_client tcp_client) -> coro::task<void>;

    auto route(const request& req, response& rep) -> void;
};

} // namespace http
} // namespace scopuli
