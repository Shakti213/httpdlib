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
#include "httpdlib/response_generator_collection.h"

namespace httpdlib
{

response_generator_collection::storage_t::iterator
response_generator_collection::get_generator_iter(const request &request) {
    return std::find_if(
        std::begin(m_response_generators), std::end(m_response_generators),
        [&](auto &elem) { return elem->is_handler_for_request(request); });
}

std::unique_ptr<interface::response>
response_generator_collection::get_response(const request &request) {
    auto iter = get_generator_iter(request);
    if (iter != std::end(m_response_generators)) {
        return (*iter)->get_response(request);
    }

    return nullptr;
}

void response_generator_collection::add_response_generator(
    std::unique_ptr<interface::response_generator> generator) {
    m_response_generators.push_back(std::move(generator));
}

void response_generator_collection::add_response_generator(
    interface::response_generator *generator) {
    add_response_generator(
        std::unique_ptr<interface::response_generator>(generator));
}

void response_generator_collection::clear_response_generators() {
    m_response_generators.clear();
}

bool response_generator_collection::priv_is_handler_for_request(
    const request &request) {
    auto iter = get_generator_iter(request);

    return iter != std::end(m_response_generators);
}
}
