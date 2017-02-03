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
#include "httpdlib/pointer_response.h"

namespace httpdlib
{

pointer_response::pointer_response(int code, const char *pointer,
                                   size_t length) {
    set_code(code);
    set_pointer(pointer, length);
}

void pointer_response::set_pointer(const char *pointer, size_t length) {
    m_pointer = pointer;
    m_length = length;
}

void pointer_response::prepare_write() {
    maybe_set_code204_or_content_length(m_length);
}

size_t
pointer_response::write_payload_part(interface::response::writer_t writer,
                                     size_t offset) {
    return write_bytes(m_pointer + offset, m_length - offset, writer);
}

} // namespace httpdlib
