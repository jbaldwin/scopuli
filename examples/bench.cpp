#include <scopuli/scopuli.hpp>
#include <scopuli/version.hpp>

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <atomic>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <string>
#include <string_view>
#include <vector>

std::atomic<sig_atomic_t> g_signal{0};

static auto signal_handler_termination(int sig_num) -> void
{
    g_signal.exchange(sig_num, std::memory_order::release);
    std::cerr << fmt::format("signal {} received\n", sig_num);
}

static auto register_signal_handlers() -> void
{
    struct sigaction new_action
    {
    };

    new_action.sa_handler = signal_handler_termination;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = static_cast<int>(SA_RESETHAND);

    const int terminating_signals[] = {SIGTERM, SIGINT, SIGHUP};

    for (auto sig : terminating_signals)
    {
        sigaction(sig, &new_action, nullptr);
    }

    new_action.sa_handler = SIG_IGN;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = static_cast<int>(SA_RESETHAND);

    const int ignored_signals[] = {SIGCONT};

    for (auto sig : ignored_signals)
    {
        sigaction(sig, &new_action, nullptr);
    }
}

static auto print_help(std::string_view name) -> void
{
    std::cout << fmt::format(
        "{0} [vh]\n"
        "    --version -v Print the version of {0}.\n"
        "    --help    -h Print this help message\n",
        name);
}

static auto print_version(std::string_view name) -> void
{
    std::cout << fmt::format("{0} {1}\n", name, scopuli::version);
}

int main(int argc, char* argv[])
{
    std::vector<std::string_view> args{argv, argv + argc};

    constexpr char   short_options[] = "vh";
    constexpr option long_options[]  = {
        {"version", no_argument, nullptr, 'v'}, {"help", no_argument, nullptr, 'h'}, {nullptr, 0, nullptr, 0}};

    int option_index = 0;
    int opt          = 0;

    while ((opt = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
            case 'v':
                print_version(args[0]);
                return EXIT_SUCCESS;
            case 'h':
                print_help(args[0]);
                return EXIT_SUCCESS;
            default:
                spdlog::error("Unknown option provided: [{}], exiting.", static_cast<char>(opt));
                return EXIT_FAILURE;
        }
    }

    register_signal_handlers();

    scopuli::http::server server{
        coro::net::ip_address::from_string("127.0.0.1"),
        8080,
        [](const scopuli::http::request& req, scopuli::http::response& rep) -> void {
            rep.set_status_code(scopuli::http::status_code::http_200_ok);

            const auto& body_opt = req.body();
            if (body_opt.has_value())
            {
                const auto& body = body_opt.value();

                rep.add_header("content-length", std::to_string(body.size()));
                rep.complete_headers();
                rep.append_body(body);
            }
            else
            {
                rep.add_header("content-length", "0");
                rep.complete_headers();
            }
        },
        scopuli::processing_method::process_requests_inline};

    while (g_signal.load(std::memory_order::acquire) == 0)
    {
        std::this_thread::sleep_for(std::chrono::seconds{1});
    }

    return 0;
}
