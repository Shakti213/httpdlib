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
#ifndef BODY_CHUNKED_ENCODING_H
#define BODY_CHUNKED_ENCODING_H

#include "httpdlib/header_collection.h"
#include "httpdlib/interface/data_parser.h"
#include "httpdlib/parser/header.h"
#include <vector>

namespace httpdlib
{
namespace parser
{
class body_chunked_encoding : public interface::data_parser
{
    enum state {
        waiting_chunk_size,
        waiting_chunk_data,
        waiting_trailer,
        body_done
    };

    state m_state = waiting_chunk_size;
    header m_header_parser;
    std::vector<char> &m_storage;
    std::size_t m_bytes_left_of_chunk = 0;
    std::string m_collector;
    std::size_t m_byte_count = 0;

public:
    body_chunked_encoding(std::vector<char> &storage,
                          header_collection &headers);
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
};
}
}

#endif // BODY_CHUNKED_ENCODING_H
