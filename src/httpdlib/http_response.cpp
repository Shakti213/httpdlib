#include "httpdlib/http_response.h"
#include <ctime>
#include <algorithm>
#include <iterator>

namespace httpdlib
{

namespace helper
{

std::string code_to_reason(int code) {
    std::string retval = " ";
    switch(code)
    {
    case 100: retval = "Continue"; break;
    case 101: retval = "Switching Protocols"; break;
    case 200: retval = "OK"; break;
    case 201: retval = "Created"; break;
    case 202: retval = "Accepted"; break;
    case 203: retval = "Non-Authoritative Information"; break;
    case 204: retval = "No Content"; break;
    case 205: retval = "Reset Content"; break;
    case 206: retval = "Partial Content"; break;
    case 300: retval = "Multiple Choices"; break;
    case 301: retval = "Moved Permanently"; break;
    case 302: retval = "Found"; break;
    case 303: retval = "See Other"; break;
    case 304: retval = "Not Modified"; break;
    case 305: retval = "Use Proxy"; break;
    case 307: retval = "Temporary Redirect"; break;
    case 400: retval = "Bad Request"; break;
    case 401: retval = "Unauthorized"; break;
    case 402: retval = "Payment Required"; break;
    case 403: retval = "Forbidden"; break;
    case 404: retval = "Not Found"; break;
    case 405: retval = "Method Not Allowed"; break;
    case 406: retval = "Not Acceptable"; break;
    case 407: retval = "Proxy Authentication Required"; break;
    case 408: retval = "Request Timeout"; break;
    case 409: retval = "Conflict"; break;
    case 410: retval = "Gone"; break;
    case 411: retval = "Length Required"; break;
    case 412: retval = "Precondition Failed"; break;
    case 413: retval = "Payload Too Large"; break;
    case 414: retval = "URI Too Long"; break;
    case 415: retval = "Unsupported Media Type"; break;
    case 416: retval = "Range Not Satisfiable"; break;
    case 417: retval = "Expectation Failed"; break;
    case 426: retval = "Upgrade Required"; break;
    case 500: retval = "Internal Server Error"; break;
    case 501: retval = "Not Implemented"; break;
    case 502: retval = "Bad Gateway"; break;
    case 503: retval = "Service Unavailable"; break;
    case 504: retval = "Gateway Timeout"; break;
    case 505: retval = "HTTP Version Not Supported"; break;

    default: retval = " "; break;
    }

    return retval;
}

} // namespace httpdlib::helper


int http_response::code() const
{
    return m_code;
}

void http_response::set_code(int code)
{
    if(helper::code_to_reason(code).length() > 0) {
        m_code = code;
    }
}

void http_response::set_header(std::string header, std::string value)
{
    m_headers.add(header, value);
}

size_t http_response::write(http_response::writer_t writer)
{
    if(m_code == 200 && m_data.size() == 0) {
        m_code = 204;
        m_headers.remove("content-length");
    }
    else {
        m_headers.add("content-length", std::to_string(m_data.size()));
    }

    auto retval = write_status_and_headers(writer);
    if(m_data.size() > 0) {
        retval += writer(m_data.data(), m_data.size());
    }

    return retval;
}

void http_response::set_data(const char *str)
{
    auto length = std::strlen(str);
    m_data.clear();
    m_data.reserve(length);
    std::copy(str, str+length, std::back_inserter(m_data));
}

void http_response::clear_data()
{
    m_data.clear();
}

http_response http_response::default_for_code(int code)
{
    static std::string default_page_head = "<html><head><title>";
    static std::string default_page_body = "</title></head><body><h1>";
    static std::string default_page_footer = "</h1></body></html>";
    http_response retval(code);
    if(code != 204 && code != 304) {
        auto code_string = std::to_string(code);
        retval.set_data(default_page_head + code_string + default_page_body + code_string + " - " + helper::code_to_reason(code) + default_page_footer);
        retval.set_header("content-type", "text/html;charset=utf-8");
    }

    return retval;
}

std::size_t http_response::write_std_string(const std::string &str, http_response::writer_t writer)
{
    return writer(str.data(), str.length());
}

std::size_t http_response::write_status_line(http_response::writer_t writer)
{
    std::string line = "HTTP/1.1 " + std::to_string(m_code) + " " + helper::code_to_reason(m_code) + "\r\n";
    return write_std_string(std::move(line), std::move(writer));
}

std::size_t http_response::write_headers(http_response::writer_t writer)
{
    m_headers.add("Date", get_date_time());
    auto to_write = m_headers.to_string();
    return write_std_string(std::move(to_write), std::move(writer));
}

size_t http_response::write_headers_end(http_response::writer_t writer)
{
    return writer("\r\n", 2);
}

size_t http_response::write_status_and_headers(http_response::writer_t writer)
{
    auto retval = write_status_line(writer);
    retval += write_headers(writer);
    retval += write_headers_end(writer);

    return retval;
}

std::string http_response::get_date_time()
{
    static std::string weekdays[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    static std::string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    static auto int_to_zero_leading_string = [](int n) {
        auto retval = std::to_string(n);
        if(retval.length() == 1) {
            retval = std::string("0") + retval;
        }

        return retval;
    };

    auto wall_now = std::time(nullptr);
    auto utc_now = *std::gmtime(&wall_now);

    std::string day_string = int_to_zero_leading_string(utc_now.tm_mday);
    std::string retval = weekdays[utc_now.tm_wday] + ", " + day_string + " " + months[utc_now.tm_mon] + " ";
    retval += int_to_zero_leading_string(utc_now.tm_hour) + ":" + int_to_zero_leading_string(utc_now.tm_min) + ":" +
            int_to_zero_leading_string(utc_now.tm_sec) + " GMT";
    return retval;
}

http_response::http_response(int code):
    m_code(404)
{
    // Assume code is invalid so set it using the setter instead.
    set_code(code);
}

} // namespace httpdlib
