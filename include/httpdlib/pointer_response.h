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
#ifndef POINTER_RESPONSE_H
#define POINTER_RESPONSE_H

#include "httpdlib/interface/response.h"

namespace httpdlib
{

/**
 * @brief An implementation that doesn't copy data
 *
 * This implementation doesn't copy data, but simply
 * stores a pointer to the raw data and a length.
 * When sending
 */
class pointer_response : public interface::response
{
    const char *m_pointer;
    std::size_t m_length;

public:
    pointer_response(int code, const char *pointer, std::size_t length);

    void set_pointer(const char *pointer, std::size_t length);

    // response interface
protected:
    void prepare_write() override;
    // response interface
protected:
    size_t write_payload_part(writer_t writer, size_t offset) override;
};

} // namespace httpdlib

#endif // POINTER_RESPONSE_H
