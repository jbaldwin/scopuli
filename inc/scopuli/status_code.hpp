#pragma once

#include <string>

namespace scopuli::http
{
inline const std::string status_code_http_unknown{"unknown"};

inline const std::string status_code_http_100_continue{"100 Continue"};
inline const std::string status_code_http_101_switching_protocols{"101 Switching Protocols"};
inline const std::string status_code_http_102_processing{"102 Processing"};
inline const std::string status_code_http_103_early_hints{"103 Early Hints"};

inline const std::string status_code_http_200_ok{"200 OK"};
inline const std::string status_code_http_201_created{"201 Created"};
inline const std::string status_code_http_202_accepted{"202 Accepted"};
inline const std::string status_code_http_203_non_authoritative_information{"203 Non-Authoritative Information"};
inline const std::string status_code_http_204_no_content{"204 No Content"};
inline const std::string status_code_http_205_reset_content{"205 Reset Content"};
inline const std::string status_code_http_206_partial_content{"206 Partial Content"};
inline const std::string status_code_http_207_multi_status{"207 Multi-Status"};
inline const std::string status_code_http_208_already_reported{"208 Already Reported"};
inline const std::string status_code_http_226_im_used{"226 IM Used"};

inline const std::string status_code_http_300_multiple_choices{"300 Multiple Choices"};
inline const std::string status_code_http_301_moved_permanently{"301 Moved Permanently"};
inline const std::string status_code_http_302_found{"302 Found"};
inline const std::string status_code_http_303_see_other{"303 See Other"};
inline const std::string status_code_http_304_not_modified{"304 Not Modified"};
inline const std::string status_code_http_305_use_proxy{"305 Use Proxy"};
inline const std::string status_code_http_306_switch_proxy{"306 Switch Proxy"};
inline const std::string status_code_http_307_temporary_redirect{"307 Temporary Redirect"};
inline const std::string status_code_http_308_permanent_redirect{"308 Permanent Redirect"};

inline const std::string status_code_http_400_bad_request{"400 Bad Request"};
inline const std::string status_code_http_401_unauthorized{"401 Unauthorized"};
inline const std::string status_code_http_402_payment_required{"402 Payment Required"};
inline const std::string status_code_http_403_forbidden{"403 Forbidden"};
inline const std::string status_code_http_404_not_found{"404 Not Found"};
inline const std::string status_code_http_405_method_not_allowed{"405 Method Not Allowed"};
inline const std::string status_code_http_406_not_acceptable{"406 Not Acceptable"};
inline const std::string status_code_http_407_proxy_authentication_required{"407 Proxy Authentication Required"};
inline const std::string status_code_http_408_request_timeout{"408 Request Timeout"};
inline const std::string status_code_http_409_conflict{"409 Conflict"};
inline const std::string status_code_http_410_gone{"410 Gone"};
inline const std::string status_code_http_411_length_required{"411 Length Required"};
inline const std::string status_code_http_412_precondition_failed{"412 Precondition Failed"};
inline const std::string status_code_http_413_payload_too_large{"413 Payload Too Large"};
inline const std::string status_code_http_414_uri_too_long{"414 URI Too Long"};
inline const std::string status_code_http_415_unsupported_media_type{"415 Unsupported Media Type"};
inline const std::string status_code_http_416_range_not_satisfiable{"416 Range Not Satisfiable"};
inline const std::string status_code_http_417_expectation_failed{"417 Expectation Failed"};
inline const std::string status_code_http_418_im_a_teapot{"418 I'm a teapot"};
inline const std::string status_code_http_421_misdirected_request{"421 Misdirected Request"};
inline const std::string status_code_http_422_unprocessable_entity{"422 Unprocessable Entity"};
inline const std::string status_code_http_423_locked{"423 Locked"};
inline const std::string status_code_http_424_failed_dependency{"424 Failed Dependency"};
inline const std::string status_code_http_425_too_early{"425 Too Early"};
inline const std::string status_code_http_426_upgrade_required{"426 Upgrade Required"};
inline const std::string status_code_http_428_precondition_required{"428 Precondition Required"};
inline const std::string status_code_http_429_too_many_requests{"429 Too Many Requests"};
inline const std::string status_code_http_431_request_header_fields_too_large{"431 Request Header Fields Too Large"};
inline const std::string status_code_http_451_unavailable_for_legal_reasons{"451 Unavailable For Legal Reasons"};

inline const std::string status_code_http_500_internal_server_error{"500 Internal Server Error"};
inline const std::string status_code_http_501_not_implemented{"501 Not Implemented"};
inline const std::string status_code_http_502_bad_gateway{"502 Bad Gateway"};
inline const std::string status_code_http_503_service_unavailable{"503 Service Unavailable"};
inline const std::string status_code_http_504_gateway_timeout{"504 Gateway Timeout"};
inline const std::string status_code_http_505_http_version_not_supported{"505 HTTP Version Not Supported"};
inline const std::string status_code_http_506_variant_also_negotiates{"506 Variant Also Negotiates"};
inline const std::string status_code_http_507_insufficient_storage{"507 Insufficient Storage"};
inline const std::string status_code_http_508_loop_detected{"508 Loop Detected"};
inline const std::string status_code_http_510_not_extended{"510 Not Extended"};
inline const std::string status_code_http_511_network_authentication_required{"511 Network Authentication Required"};

/**
 * https://en.wikipedia.org/wiki/List_of_HTTP_status_codes
 */
enum class status_code : uint16_t
{
    http_unknown = 0,

    http_100_continue            = 100,
    http_101_switching_protocols = 101,
    http_102_processing          = 102,
    http_103_early_hints         = 103,

    http_200_ok                            = 200,
    http_201_created                       = 201,
    http_202_accepted                      = 202,
    http_203_non_authoritative_information = 203,
    http_204_no_content                    = 204,
    http_205_reset_content                 = 205,
    http_206_partial_content               = 206,
    http_207_multi_status                  = 207,
    http_208_already_reported              = 208,
    http_226_im_used                       = 226,

    http_300_multiple_choices  = 300,
    http_301_moved_permanently = 301,
    http_302_found             = 302,
    http_303_see_other         = 303,
    http_304_not_modified      = 304,
    http_305_use_proxy         = 305,
    /*
     * HTTP status code 306 is unused and reserved per RFC 7231
     * (https://tools.ietf.org/html/rfc7231#section-6.4.6), but originally meant
     * 'switch proxy', so leaving for backwards compatibility.
     */
    http_306_switch_proxy       = 306,
    http_307_temporary_redirect = 307,
    http_308_permanent_redirect = 308,

    http_400_bad_request                     = 400,
    http_401_unauthorized                    = 401,
    http_402_payment_required                = 402,
    http_403_forbidden                       = 403,
    http_404_not_found                       = 404,
    http_405_method_not_allowed              = 405,
    http_406_not_acceptable                  = 406,
    http_407_proxy_authentication_required   = 407,
    http_408_request_timeout                 = 408,
    http_409_conflict                        = 409,
    http_410_gone                            = 410,
    http_411_length_required                 = 411,
    http_412_precondition_failed             = 412,
    http_413_payload_too_large               = 413,
    http_414_uri_too_long                    = 414,
    http_415_unsupported_media_type          = 415,
    http_416_range_not_satisfiable           = 416,
    http_417_expectation_failed              = 417,
    http_418_im_a_teapot                     = 418,
    http_421_misdirected_request             = 421,
    http_422_unprocessable_entity            = 422,
    http_423_locked                          = 423,
    http_424_failed_dependency               = 424,
    http_425_too_early                       = 425, // https://tools.ietf.org/html/rfc8470#section-5.2
    http_426_upgrade_required                = 426,
    http_428_precondition_required           = 428, // https://tools.ietf.org/html/rfc6585
    http_429_too_many_requests               = 429,
    http_431_request_header_fields_too_large = 431,
    http_451_unavailable_for_legal_reasons   = 451,

    http_500_internal_server_error           = 500,
    http_501_not_implemented                 = 501,
    http_502_bad_gateway                     = 502,
    http_503_service_unavailable             = 503,
    http_504_gateway_timeout                 = 504,
    http_505_http_version_not_supported      = 505,
    http_506_variant_also_negotiates         = 506,
    http_507_insufficient_storage            = 507,
    http_508_loop_detected                   = 508,
    http_510_not_extended                    = 510,
    http_511_network_authentication_required = 511
};

/**
 * @param code The status code to retrieve its string representation.
 * @return string
 */
static inline auto to_string(status_code code) -> const std::string&
{
    switch (code)
    {
        case status_code::http_unknown:
            return status_code_http_unknown;

        case status_code::http_100_continue:
            return status_code_http_100_continue;
        case status_code::http_101_switching_protocols:
            return status_code_http_101_switching_protocols;
        case status_code::http_102_processing:
            return status_code_http_102_processing;
        case status_code::http_103_early_hints:
            return status_code_http_103_early_hints;

        case status_code::http_200_ok:
            return status_code_http_200_ok;
        case status_code::http_201_created:
            return status_code_http_201_created;
        case status_code::http_202_accepted:
            return status_code_http_202_accepted;
        case status_code::http_203_non_authoritative_information:
            return status_code_http_203_non_authoritative_information;
        case status_code::http_204_no_content:
            return status_code_http_204_no_content;
        case status_code::http_205_reset_content:
            return status_code_http_205_reset_content;
        case status_code::http_206_partial_content:
            return status_code_http_206_partial_content;
        case status_code::http_207_multi_status:
            return status_code_http_207_multi_status;
        case status_code::http_208_already_reported:
            return status_code_http_208_already_reported;
        case status_code::http_226_im_used:
            return status_code_http_226_im_used;

        case status_code::http_300_multiple_choices:
            return status_code_http_300_multiple_choices;
        case status_code::http_301_moved_permanently:
            return status_code_http_301_moved_permanently;
        case status_code::http_302_found:
            return status_code_http_302_found;
        case status_code::http_303_see_other:
            return status_code_http_303_see_other;
        case status_code::http_304_not_modified:
            return status_code_http_304_not_modified;
        case status_code::http_305_use_proxy:
            return status_code_http_305_use_proxy;
        case status_code::http_306_switch_proxy:
            return status_code_http_306_switch_proxy;
        case status_code::http_307_temporary_redirect:
            return status_code_http_307_temporary_redirect;
        case status_code::http_308_permanent_redirect:
            return status_code_http_308_permanent_redirect;

        case status_code::http_400_bad_request:
            return status_code_http_400_bad_request;
        case status_code::http_401_unauthorized:
            return status_code_http_401_unauthorized;
        case status_code::http_402_payment_required:
            return status_code_http_402_payment_required;
        case status_code::http_403_forbidden:
            return status_code_http_403_forbidden;
        case status_code::http_404_not_found:
            return status_code_http_404_not_found;
        case status_code::http_405_method_not_allowed:
            return status_code_http_405_method_not_allowed;
        case status_code::http_406_not_acceptable:
            return status_code_http_406_not_acceptable;
        case status_code::http_407_proxy_authentication_required:
            return status_code_http_407_proxy_authentication_required;
        case status_code::http_408_request_timeout:
            return status_code_http_408_request_timeout;
        case status_code::http_409_conflict:
            return status_code_http_409_conflict;
        case status_code::http_410_gone:
            return status_code_http_410_gone;
        case status_code::http_411_length_required:
            return status_code_http_411_length_required;
        case status_code::http_412_precondition_failed:
            return status_code_http_412_precondition_failed;
        case status_code::http_413_payload_too_large:
            return status_code_http_413_payload_too_large;
        case status_code::http_414_uri_too_long:
            return status_code_http_414_uri_too_long;
        case status_code::http_415_unsupported_media_type:
            return status_code_http_415_unsupported_media_type;
        case status_code::http_416_range_not_satisfiable:
            return status_code_http_416_range_not_satisfiable;
        case status_code::http_417_expectation_failed:
            return status_code_http_417_expectation_failed;
        case status_code::http_418_im_a_teapot:
            return status_code_http_418_im_a_teapot;
        case status_code::http_421_misdirected_request:
            return status_code_http_421_misdirected_request;
        case status_code::http_422_unprocessable_entity:
            return status_code_http_422_unprocessable_entity;
        case status_code::http_423_locked:
            return status_code_http_423_locked;
        case status_code::http_424_failed_dependency:
            return status_code_http_424_failed_dependency;
        case status_code::http_425_too_early:
            return status_code_http_425_too_early;
        case status_code::http_426_upgrade_required:
            return status_code_http_426_upgrade_required;
        case status_code::http_428_precondition_required:
            return status_code_http_428_precondition_required;
        case status_code::http_429_too_many_requests:
            return status_code_http_429_too_many_requests;
        case status_code::http_431_request_header_fields_too_large:
            return status_code_http_431_request_header_fields_too_large;
        case status_code::http_451_unavailable_for_legal_reasons:
            return status_code_http_451_unavailable_for_legal_reasons;

        case status_code::http_500_internal_server_error:
            return status_code_http_500_internal_server_error;
        case status_code::http_501_not_implemented:
            return status_code_http_501_not_implemented;
        case status_code::http_502_bad_gateway:
            return status_code_http_502_bad_gateway;
        case status_code::http_503_service_unavailable:
            return status_code_http_503_service_unavailable;
        case status_code::http_504_gateway_timeout:
            return status_code_http_504_gateway_timeout;
        case status_code::http_505_http_version_not_supported:
            return status_code_http_505_http_version_not_supported;
        case status_code::http_506_variant_also_negotiates:
            return status_code_http_506_variant_also_negotiates;
        case status_code::http_507_insufficient_storage:
            return status_code_http_507_insufficient_storage;
        case status_code::http_508_loop_detected:
            return status_code_http_508_loop_detected;
        case status_code::http_510_not_extended:
            return status_code_http_510_not_extended;
        case status_code::http_511_network_authentication_required:
            return status_code_http_511_network_authentication_required;

        default:
            return status_code_http_unknown;
    }
}

} // namespace scopuli::http
