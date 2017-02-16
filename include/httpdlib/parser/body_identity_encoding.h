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
#ifndef BODY_IDENTITY_ENCODING_H
#define BODY_IDENTITY_ENCODING_H

#include "httpdlib/interface/data_parser.h"
#include <vector>

namespace httpdlib
{

namespace parser
{
class body_identity_encoding : public interface::data_parser
{
    std::size_t m_byte_count;
    std::vector<char> &m_storage;
    std::size_t m_byte_read = 0;

public:
    body_identity_encoding(std::size_t byte_count, std::vector<char> &storage);
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
};
}
}

#endif // BODY_IDENTITY_ENCODING_H
