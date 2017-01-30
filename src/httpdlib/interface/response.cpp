#include "httpdlib/interface/response.h"
#include <algorithm>
#include <ctime>
#include <iterator>

namespace httpdlib
{

namespace interface
{

response::~response() {
    // Do nothing
}

int response::code() const {
    return m_code;
}

void response::set_code(int code) {
    if (code_to_reason(code).length() > 0) {
        m_code = code;
    }
}

std::string response::code_to_reason(int code) {
    std::string retval = " ";
    switch (code) {
    case 100:
        retval = "Continue";
        break;
    case 101:
        retval = "Switching Protocols";
        break;
    case 200:
        retval = "OK";
        break;
    case 201:
        retval = "Created";
        break;
    case 202:
        retval = "Accepted";
        break;
    case 203:
        retval = "Non-Authoritative Information";
        break;
    case 204:
        retval = "No Content";
        break;
    case 205:
        retval = "Reset Content";
        break;
    case 206:
        retval = "Partial Content";
        break;
    case 300:
        retval = "Multiple Choices";
        break;
    case 301:
        retval = "Moved Permanently";
        break;
    case 302:
        retval = "Found";
        break;
    case 303:
        retval = "See Other";
        break;
    case 304:
        retval = "Not Modified";
        break;
    case 305:
        retval = "Use Proxy";
        break;
    case 307:
        retval = "Temporary Redirect";
        break;
    case 400:
        retval = "Bad Request";
        break;
    case 401:
        retval = "Unauthorized";
        break;
    case 402:
        retval = "Payment Required";
        break;
    case 403:
        retval = "Forbidden";
        break;
    case 404:
        retval = "Not Found";
        break;
    case 405:
        retval = "Method Not Allowed";
        break;
    case 406:
        retval = "Not Acceptable";
        break;
    case 407:
        retval = "Proxy Authentication Required";
        break;
    case 408:
        retval = "Request Timeout";
        break;
    case 409:
        retval = "Conflict";
        break;
    case 410:
        retval = "Gone";
        break;
    case 411:
        retval = "Length Required";
        break;
    case 412:
        retval = "Precondition Failed";
        break;
    case 413:
        retval = "Payload Too Large";
        break;
    case 414:
        retval = "URI Too Long";
        break;
    case 415:
        retval = "Unsupported Media Type";
        break;
    case 416:
        retval = "Range Not Satisfiable";
        break;
    case 417:
        retval = "Expectation Failed";
        break;
    case 426:
        retval = "Upgrade Required";
        break;
    case 500:
        retval = "Internal Server Error";
        break;
    case 501:
        retval = "Not Implemented";
        break;
    case 502:
        retval = "Bad Gateway";
        break;
    case 503:
        retval = "Service Unavailable";
        break;
    case 504:
        retval = "Gateway Timeout";
        break;
    case 505:
        retval = "HTTP Version Not Supported";
        break;

    default:
        retval = " ";
        break;
    }

    return retval;
}

void response::maybe_set_code204_or_content_length(size_t content_length) {
    if (m_code == 200 && content_length == 0) {
        m_code = 204;
        m_headers.remove("content-length");
    }
    else {
        m_headers.add("content-length", std::to_string(content_length));
    }
}

void response::set_header(std::string header, std::string value) {
    m_headers.add(header, value);
}

size_t response::write(response::writer_t writer, AbortPolicy abort_policy) {
    prepare_write();
    std::size_t retval = write_status_and_headers(writer, abort_policy);
    retval += write_payload(writer, abort_policy);
    return retval;
}

std::size_t response::write_std_string(const std::string &str,
                                       response::writer_t writer,
                                       AbortPolicy abort_policy) const {
    return write_bytes(str.data(), str.length(), writer, abort_policy);
}

std::size_t response::write_status_line(response::writer_t writer,
                                        AbortPolicy abort_policy) const {
    std::string line = "HTTP/1.1 " + std::to_string(m_code) + " " +
                       code_to_reason(m_code) + "\r\n";
    return write_std_string(std::move(line), std::move(writer), abort_policy);
}

std::size_t response::write_headers(response::writer_t writer,
                                    AbortPolicy abort_policy) {
    m_headers.add("Date", get_date_time());
    auto to_write = m_headers.to_string();
    return write_std_string(std::move(to_write), std::move(writer),
                            abort_policy);
}

size_t response::write_headers_end(response::writer_t writer,
                                   AbortPolicy abort_policy) const {
    return write_bytes("\r\n", 2, writer, abort_policy);
}

size_t response::write_status_and_headers(response::writer_t writer,
                                          AbortPolicy abort_policy) {
    auto retval = write_status_line(writer, abort_policy);
    retval += write_headers(writer, abort_policy);
    retval += write_headers_end(writer, abort_policy);

    return retval;
}

size_t response::write_bytes(const char *bytes, size_t length,
                             response::writer_t writer,
                             AbortPolicy abort_policy) const {
    std::size_t bytes_written = 0;
    while (bytes_written < length) {
        std::size_t temp_bytes_written =
            writer(bytes + bytes_written, length - bytes_written);
        if (temp_bytes_written == 0 &&
            abort_policy == AbortOnZeroBytesWritten) {
            throw std::runtime_error(
                "httpdlib - writer returned 0 bytes written");
        }
        bytes_written += writer(bytes + bytes_written, length - bytes_written);
    }

    return bytes_written;
}

std::string response::get_date_time() const {
    static std::string weekdays[] = {"Sun", "Mon", "Tue", "Wed",
                                     "Thu", "Fri", "Sat"};
    static std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    static auto int_to_zero_leading_string = [](int n) {
        auto retval = std::to_string(n);
        if (retval.length() == 1) {
            retval = std::string("0") + retval;
        }

        return retval;
    };

    auto wall_now = std::time(nullptr);
    auto utc_now = *std::gmtime(&wall_now);

    std::string day_string = int_to_zero_leading_string(utc_now.tm_mday);
    std::string retval = weekdays[utc_now.tm_wday] + ", " + day_string + " " +
                         months[utc_now.tm_mon] + " ";
    retval += int_to_zero_leading_string(utc_now.tm_hour) + ":" +
              int_to_zero_leading_string(utc_now.tm_min) + ":" +
              int_to_zero_leading_string(utc_now.tm_sec) + " GMT";
    return retval;
}

} // namespace interface

} // namespace httpdlib
