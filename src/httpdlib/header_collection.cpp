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
#include "httpdlib/header_collection.h"
#include "httpdlib/string_util/string_util.h"
#include <algorithm>
#include <numeric>

namespace httpdlib
{

using namespace string_util;

std::size_t header_collection::size() const {
    return m_headers.size();
}

bool header_collection::contains(const std::string &key) const {
    auto header = m_headers.find(to_lower(key));
    return header != m_headers.end();
}

std::string header_collection::value(const std::string &key) const {
    auto header = m_headers.find(to_lower(key));
    if (header != m_headers.end()) {
        return header->second;
    }

    return "";
}

void header_collection::add(const std::string &key, const std::string &value) {
    m_headers[to_lower(key)] = value;
}

void header_collection::remove(const std::string &key) {
    m_headers.erase(to_lower(key));
}

void header_collection::parse(const std::string &str) {
    auto all_rows = split_all(str, '\n');
    for (const auto &row : all_rows) {
        auto split_at_colon = split_once(row, ':');
        auto key = trim_both(split_at_colon.first);
        auto value = trim_both(split_at_colon.second);
        if (key.size() > 0) {
            m_headers[key] = value;
        }
    }
}

void header_collection::clear() {
    m_headers.clear();
}

const std::string &header_collection::key(header_collection::iterator iter) {
    return iter->first;
}

std::string &header_collection::value(header_collection::iterator iter) {
    return iter->second;
}

const std::string &
header_collection::key(header_collection::const_iterator iter) {
    return iter->first;
}

const std::string &
header_collection::value(header_collection::const_iterator iter) {
    return iter->second;
}

const std::string &header_collection::key(const foreach_t &iter) {
    return iter.first;
}

const std::string &header_collection::value(const foreach_t &iter) {
    return iter.second;
}

header_collection::iterator header_collection::begin() {
    return m_headers.begin();
}

header_collection::iterator header_collection::end() {
    return m_headers.end();
}

header_collection::const_iterator header_collection::begin() const {
    return m_headers.begin();
}

header_collection::const_iterator header_collection::end() const {
    return m_headers.end();
}

header_collection::const_iterator header_collection::cbegin() const {
    return m_headers.cbegin();
}

header_collection::const_iterator header_collection::cend() const {
    return m_headers.cend();
}

std::string header_collection::to_string() const {
    if (m_headers.size() == 0) {
        return "\r\n";
    }
    return std::accumulate(begin(), end(), std::string(""),
                           [&](const std::string &acc, const foreach_t &iter) {
                               return acc + key(iter) + ": " + value(iter) +
                                      "\r\n";
                           });
}

} // namespace httpdlib
