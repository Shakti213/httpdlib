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
#include "httpdlib/memory_response.h"
#include <cstring>

namespace httpdlib
{

memory_response::memory_response(int code) {
    set_code(code);
}

void memory_response::set_data(const char *str) {
    std::size_t length = std::strlen(str);
    set_data(str, length);
}

void memory_response::set_data(const char *data, size_t length) {
    m_data.clear();
    m_data.reserve(length);
    std::copy(data, data + length, std::back_inserter(m_data));
}

void memory_response::set_data(std::vector<char> &&data) {
    m_data = std::move(data);
}

void memory_response::clear_data() {
    m_data.clear();
}

std::unique_ptr<memory_response> memory_response::default_for_code(int code) {
    static std::string default_page_head = "<html><head><title>";
    static std::string default_page_body = "</title></head><body><h1>";
    static std::string default_page_footer = "</h1></body></html>";
    std::unique_ptr<memory_response> retval =
        std::make_unique<memory_response>(code);
    if (code != 204 && code != 304) {
        auto code_string = std::to_string(code);
        retval->set_data(default_page_head + code_string + default_page_body +
                         code_string + " - " + response::code_to_reason(code) +
                         default_page_footer);

        retval->set_header("content-type", "text/html;charset=utf-8");
    }

    return retval;
}

void memory_response::prepare_write() {
    maybe_set_code204_or_content_length(m_data.size());
}

size_t memory_response::write_payload_part(interface::response::writer_t writer,
                                           size_t offset) {
    return write_bytes(m_data.data() + offset, m_data.size() - offset, writer);
}
}
