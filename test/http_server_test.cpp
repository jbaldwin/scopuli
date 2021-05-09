#include <catch.hpp>
#include <scopuli/scopuli.hpp>

TEST_CASE("test to get CI working", "[ci]")
{
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

    REQUIRE(true);
}