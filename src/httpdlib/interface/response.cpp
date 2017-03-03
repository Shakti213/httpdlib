/*
 * MIT License
 *
 * Copyright (c) 2017 Andreas Wass
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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

bool response::payload_done(size_t payload_bytes_written) const {
    return payload_bytes_written >= m_expected_payload_size;
}

void response::async_payload_written(std::size_t)
{

}

void response::maybe_set_code204_or_content_length(size_t content_length) {
    if (m_code == 200 && content_length == 0) {
        m_code = 204;
        m_headers.remove("content-length");
    }
    else {
        m_headers.add("content-length", std::to_string(content_length));
    }

    m_expected_payload_size = content_length;
}

void response::set_header(const std::string &header, const std::string &value) {
    m_headers.add(header, value);
}

size_t response::write_next(response::writer_t writer) {
    if (m_state == state_write_headers) {
        if (m_serialized_status_and_headers.size() == 0) {
            prepare_write();
            m_serialized_status_and_headers = "HTTP/1.1 " +
                                              std::to_string(m_code) + " " +
                                              code_to_reason(m_code) + "\r\n";
            m_headers.add("Date", get_date_time());
            m_serialized_status_and_headers += m_headers.to_string();
            m_serialized_status_and_headers += "\r\n";
            m_write_next_offset = 0;
        }

        auto bytes_written = write_bytes(
            m_serialized_status_and_headers.data() + m_write_next_offset,
            m_serialized_status_and_headers.length() - m_write_next_offset,
            writer);

        m_write_next_offset += bytes_written;

        if (m_write_next_offset >= m_serialized_status_and_headers.length()) {
            m_state = state_write_payload;
            m_write_next_offset = 0;
        }

        return bytes_written;
    }

    auto bytes_written = write_payload_part(writer, m_write_next_offset);
    m_write_next_offset += bytes_written;
    return bytes_written;
}

void response::async_bytes_written(std::size_t bytes_written) {
    m_write_next_offset += bytes_written;
    if(m_state == state_write_headers &&
            m_write_next_offset >= m_serialized_status_and_headers.length()) {
        m_state = state_write_payload;
        m_write_next_offset = 0;
    }
    else if(m_state == state_write_payload) {
        async_payload_written(bytes_written);
    }
}

bool response::done() const {
    return m_state == state_write_payload && payload_done(m_write_next_offset);
}

size_t response::write_bytes(const char *bytes, size_t length,
                             response::writer_t writer) const {
    return writer(bytes, length);
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
