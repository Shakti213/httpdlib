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
#include <algorithm>

#include "httpdlib/interface/response_generator.h"

namespace httpdlib
{

namespace interface
{

response_generator::~response_generator() {
    // Do nothing
}

void response_generator::clear_filters() {
    m_filters.clear();
}

bool response_generator::check_filters(const request &request) {
    // If any of the filters return false std::any_of will return true,
    return !std::any_of(
        std::begin(m_filters), std::end(m_filters),
        [&request](const auto &f) { return f(request) == false; });
}

bool response_generator::priv_is_handler_for_request(const request &) {
    return true;
}

bool httpdlib::interface::response_generator::is_handler_for_request(
    const request &request) {
    return check_filters(request) && priv_is_handler_for_request(request);
}

void response_generator::add_filter(
    std::function<bool(const request &)> filter) {
    m_filters.push_back(filter);
}

} // namespace interface

} // namespace httpdlib
