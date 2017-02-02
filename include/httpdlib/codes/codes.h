#ifndef CODES_H
#define CODES_H

namespace httpdlib
{

namespace codes
{


static const int continue_ = 100,
switching_protocol = 101,
ok = 200,
created = 201,
accepted = 202,
non_authoritative_info = 203,
no_content = 204,
reset_content = 205,
partial_content = 206,
multiple_choices = 300,
moved_permanently = 301,
found = 302,
see_other = 303,
not_modified = 304,
use_proxy = 305,
temporary_redirect = 307,
bad_request = 400,
unauthorized = 401,
payment_required = 402,
forbidden = 403,
not_found = 404,
method_not_allowed = 405,
not_acceptable = 406,
proxy_auth_required = 407,
request_timeout = 408,
conflict = 409,
gone = 410,
length_required = 411,
precondition_failed = 412,
payload_too_large = 413,
uri_too_long = 414,
unsupported_media_type = 415,
range_not_satisfiable = 416,
expectation_failed = 417,
upgrade_required = 426,
internal_server_error = 500,
not_implemented = 501,
bad_gateway = 502,
service_unavailable = 503,
gateway_timeout = 504,
unsupported_version = 505;

}


}


#endif // CODES_H