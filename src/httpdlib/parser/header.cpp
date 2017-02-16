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
#include "httpdlib/parser/header.h"
#include "httpdlib/codes/codes.h"
#include "httpdlib/string_util/string_util.h"

namespace httpdlib
{
namespace parser
{

header::header(header_collection &header_storage)
    : m_header_storage(header_storage) {
}

bool header::done() const {
    return m_done;
}

bool header::add_data(char data) {
    m_collector += data;
    m_byte_count++;
    if (string_util::ends_with(m_collector, "\r\n")) {
        if (m_collector == "\r\n") {
            m_done = true;
        }
        else {
            m_header_storage.parse(m_collector);
            m_collector = "";
        }
    }
    else {
        if (m_header_storage.size() == 0 && m_collector.size() >= '3') {
            if (m_collector[0] == ' ' || m_collector[1] == ' ' ||
                m_collector[2] == ' ') {
                m_error_code = codes::bad_request;
            }
        }
    }

    return true;
}

size_t header::size() const {
    return m_byte_count;
}
}
}
