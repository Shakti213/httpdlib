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
#ifndef ISTREAM_BUFFER_H
#define ISTREAM_BUFFER_H

#include "buffer_impl/double_buffer.h"

namespace httpdlib
{
namespace buffer
{

template <typename T>
auto double_buffer(T *adapter)
    -> std::unique_ptr<buffer_impl::double_buffer<T>> {
    return std::unique_ptr<double_buffer<T>>(
        new buffer_impl::double_buffer<T>(adapter));
}
template <typename T>
auto double_buffer(std::unique_ptr<T> adapter)
    -> std::unique_ptr<buffer_impl::double_buffer<T>> {
    return std::unique_ptr<buffer_impl::double_buffer<T>>(
        new buffer_impl::double_buffer<T>(std::move(adapter)));
}
}
}

#endif // DOUBLE_BUFFER_H
