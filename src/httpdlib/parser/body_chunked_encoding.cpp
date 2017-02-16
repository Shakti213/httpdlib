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
#include "httpdlib/parser/body_chunked_encoding.h"
#include "httpdlib/string_util/string_util.h"

std::size_t parse_chunk_size(const std::string &data) {
    using namespace httpdlib::string_util;
    auto size_data = to_lower(trim_both(split_once(data, ';').first));
    std::size_t retval = 0;
    for (const auto &c : size_data) {
        retval *= 16;
        if (c >= '0' && c <= '9') {
            retval += static_cast<std::size_t>(c - '0');
        }
        else if (c >= 'a' && c <= 'f') {
            retval += static_cast<std::size_t>(10 + (c - 'a'));
        }
    }

    return retval;
}

namespace httpdlib
{
namespace parser
{

body_chunked_encoding::body_chunked_encoding(std::vector<char> &storage,
                                             header_collection &headers)
    : m_header_parser(headers), m_storage(storage) {
}

bool body_chunked_encoding::done() const {
    return m_state == body_done;
}

bool body_chunked_encoding::add_data(char data) {
    bool retval = false;
    switch (m_state) {
    case waiting_chunk_size:
        if (data == '\n') {
            m_bytes_left_of_chunk = parse_chunk_size(m_collector);
            if (m_bytes_left_of_chunk == 0) {
                m_state = waiting_trailer;
            }
            else {
                m_state = waiting_chunk_data;
            }
            m_collector = "";
        }
        else {
            m_collector += data;
        }
        break;

    case waiting_chunk_data:
        if (m_bytes_left_of_chunk > 0) {
            retval = true;
            m_byte_count++;
            m_storage.push_back(data);
            m_bytes_left_of_chunk--;
        }
        else if (data == '\n') {
            m_state = waiting_chunk_size;
        }
        break;

    case waiting_trailer:
        m_header_parser.add_data(data);
        if (m_header_parser.done()) {
            m_state = body_done;
        }
        break;

    case body_done:
        break;
    }

    return retval;
}

size_t body_chunked_encoding::size() const {
    return m_byte_count;
}
}
}
