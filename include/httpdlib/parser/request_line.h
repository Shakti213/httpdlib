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
#ifndef REQUEST_LINE_H
#define REQUEST_LINE_H

#include "httpdlib/interface/data_parser.h"
#include <string>

namespace httpdlib
{

namespace parser
{

class request_line : public interface::data_parser
{
    enum state {
        waiting_method_start,
        collecting_method,
        waiting_uri_start,
        collecting_uri,
        collecting_query,
        collecting_fragment,
        waiting_version_start,
        collecting_version,
        line_done,
        line_error
    };

    state m_state = waiting_method_start;
    std::size_t m_byte_count = 0;

    std::string m_method;
    std::string m_uri;
    std::string m_query_string;
    std::string m_fragment;
    std::string m_version;

public:
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
    std::string method() const;
    std::string uri() const;
    std::string query_string() const;
    std::string fragment() const;
    std::string version() const;
};
}
}

#endif // REQUEST_LINE_H
