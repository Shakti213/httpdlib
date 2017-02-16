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
#include "httpdlib/parser/request_line.h"
#include "httpdlib/codes/codes.h"
#include "httpdlib/string_util/string_util.h"

namespace httpdlib
{
namespace parser
{
bool request_line::done() const {
    return m_state == line_done;
}

bool request_line::add_data(char data) {
    bool retval = false;
    if (data != ' ' && data != '\r' && data != '\n') {
        m_byte_count++;
        retval = true;
    }
    if (data == '\r' || data == '\n') {
        if (m_state != collecting_version && m_state != line_done &&
            m_state != line_error) {
            m_state = line_error;
            m_error_code = codes::bad_request;
        }
    }
    switch (m_state) {
    case waiting_method_start:
        if (data >= 'A' && data <= 'Z') {
            m_method = data;
            m_state = collecting_method;
        }
        break;

    case collecting_method:
        if (data == ' ') {
            m_state = waiting_uri_start;
        }
        else {
            m_method += data;
        }
        break;

    case waiting_uri_start:
        if (data != ' ') {
            m_state = collecting_uri;
            m_uri = data;
        }
        break;

    case collecting_uri:
        if (data == '?' || data == '#' || data == ' ') {
            bool ok = true;
            m_uri = string_util::url_decode(m_uri, ok);
            if (!ok) {
                m_uri = "";
                m_state = line_error;
                m_error_code = codes::bad_request;
            }
            else if (data == '?') {
                m_state = collecting_query;
            }
            else if (data == '#') {
                m_state = collecting_fragment;
            }
            else {
                m_state = waiting_version_start;
            }
        }
        else {
            m_uri += data;
        }
        break;

    case collecting_query:
        if (data == '#') {
            m_state = collecting_fragment;
        }
        else if (data == ' ') {
            m_state = waiting_version_start;
        }
        else {
            m_query_string += data;
        }
        break;

    case collecting_fragment:
        if (data == ' ') {
            m_state = waiting_version_start;
        }
        else {
            m_fragment += data;
        }
        break;

    case waiting_version_start:
        if (data != ' ') {
            m_version = data;
            m_state = collecting_version;
        }
        break;

    case collecting_version:
        if (data == '\r') {
            m_state = line_done;
        }
        else {
            m_version += data;
        }
        break;

    case line_done:
    // fall through here
    case line_error:
        break;
    }

    return retval;
}

size_t request_line::size() const {
    return m_byte_count;
}

std::string request_line::method() const {
    return m_method;
}

std::string request_line::uri() const {
    return m_uri;
}

std::string request_line::query_string() const {
    return m_query_string;
}

std::string request_line::fragment() const {
    return m_fragment;
}

std::string request_line::version() const {
    return m_version;
}
}
}
