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
#include "httpdlib/buffer/adapter/istream_adapter.h"

namespace httpdlib
{
namespace buffer
{
namespace adapter
{

std::unique_ptr<istream_adapter> adapter(std::istream *stream) {
    return std::unique_ptr<istream_adapter>(new istream_adapter(stream));
}

istream_adapter::istream_adapter(std::unique_ptr<std::istream> stream)
    : m_stream(std::move(stream)) {
}

istream_adapter::istream_adapter(std::istream *stream) : m_stream(stream) {
}

size_t istream_adapter::size() {
    auto current_pos = m_stream->tellg();
    m_stream->seekg(0, m_stream->end);
    std::size_t retval = static_cast<std::size_t>(m_stream->tellg());
    m_stream->seekg(current_pos, m_stream->beg);
    return retval;
}

size_t istream_adapter::read(char *data, size_t max_size) {
    m_stream->read(data, static_cast<std::streamsize>(max_size));
    auto bytes_read = m_stream->gcount();
    return static_cast<std::size_t>((bytes_read < 0 ? 0 : bytes_read));
}

bool istream_adapter::error() {
    return !m_stream->good();
}
}
}
}
