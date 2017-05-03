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
#ifndef ISTREAM_ADAPTER_H
#define ISTREAM_ADAPTER_H

#include "adapter.h"
#include <istream>

namespace httpdlib
{
namespace buffer
{
namespace adapter
{

/**
 * @brief adapter   Creates an adapter from a stream
 * @param stream    The stream pointer will be alive until the buffer is
 * destroyed.
 */
inline auto adapter(std::istream *stream) {
    auto read_callable = [stream](char *buf, std::size_t max_len) {
        stream->read(buf, static_cast<std::streamsize>(max_len));
        auto bytes_read = stream->gcount();
        return static_cast<std::size_t>(bytes_read < 0 ? 0 : bytes_read);
    };
    auto size_callable = [stream]() {
        auto current_pos = stream->tellg();
        stream->seekg(0, stream->end);
        std::size_t retval = static_cast<std::size_t>(stream->tellg());
        stream->seekg(current_pos, stream->beg);
        return retval;
    };
    auto error_callable = [stream]() { return !stream->good(); };

    return adapter(stream, size_callable, read_callable, error_callable);
}
}
}
}

#endif // ISTREAM_ADAPTER_H
