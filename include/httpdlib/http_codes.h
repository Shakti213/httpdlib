#ifndef HTTP_CODES_H
#define HTTP_CODES_H

namespace httpdlib
{

static const int http_continue = 100,
http_switching_protocol = 101,
http_ok = 200,
http_created = 201,
http_accepted = 202,
http_non_authoritative_info = 203,
http_no_content = 204,
http_reset_content = 205,
http_partial_content = 206,
http_multiple_choices = 300,
http_moved_permanently = 301,
http_found = 302,
http_see_other = 303,
http_not_modified = 304,
http_use_proxy = 305,
http_temporary_redirect = 307,
http_bad_request = 400,
http_unauthorized = 401,
http_payment_required = 402,
http_forbidden = 403,
http_not_found = 404,
http_method_not_allowed = 405,
http_not_acceptable = 406,
http_proxy_auth_required = 407,
http_request_timeout = 408,
http_conflict = 409,
http_gone = 410,
http_length_required = 411,
http_precondition_failed = 412,
http_payload_too_large = 413,
http_uri_too_long = 414,
http_unsupported_media_type = 415,
http_range_not_satisfiable = 416,
http_expectation_failed = 417,
http_upgrade_required = 426,
http_internal_server_error = 500,
http_not_implemented = 501,
http_bad_gateway = 502,
http_service_unavailable = 503,
http_gateway_timeout = 504,
http_unsupported_version = 505;


}


#endif // HTTP_CODES_H
