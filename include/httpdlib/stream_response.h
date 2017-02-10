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
#ifndef STREAM_RESPONSE_H
#define STREAM_RESPONSE_H

#include "httpdlib/interface/response.h"
#include <array>
#include <istream>
#include <memory>

namespace httpdlib
{

/**
 * @brief Builds a response based on an istream
 *
 * The istream can be a file or any other implementation of std::istream.
 * This uses an internal buffer of 128 kB to read into and use as a
 * storage to send from. This allows for very big files to be served
 * without loading them into RAM first.
 *
 */
class stream_response : public interface::response
{
    std::unique_ptr<std::istream> m_stream;
    // 128kB buffer
    std::array<char, 131072> m_buffer;
    std::size_t m_buffer_offset;
    std::size_t m_buffer_length;
    std::size_t m_size;

    void read_next_into_buffer();

public:
    stream_response(std::unique_ptr<std::istream> stream);
    stream_response(std::istream *stream);
    std::size_t size();

protected:
    void prepare_write() override;
    size_t write_payload_part(writer_t writer, size_t offset) override;
};

} // namespace httpdlib

#endif // STREAM_RESPONSE_H
