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
#include "httpdlib/request.h"
#include "httpdlib/string_util/string_util.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <tuple>

namespace httpdlib
{

using namespace string_util;

std::vector<std::pair<double, std::string>>
parse_header_with_qvalues(std::string header_value) {

    using stored_t = std::pair<double, std::string>;
    std::vector<stored_t> retval;

    auto individual_values = string_util::split_all(header_value, ',');
    for (auto &v : individual_values) {
        auto value_and_param = string_util::split_once(v, ';');
        auto value = string_util::trim_both(value_and_param.first);
        auto param = string_util::trim_both(value_and_param.second);

        auto q_start = std::find(std::begin(param), std::end(param), 'q');
        double q_value = 1.0;
        if (q_start != std::end(param)) {
            q_start++;
            if (*q_start == '=') {
                q_start++;
                auto q_end = std::end(param);
                std::string q_value_string = "";
                while (q_start != q_end) {
                    if (*q_start == '.' ||
                        (*q_start >= '0' && *q_start <= '9')) {
                        q_value_string += *q_start++;
                    }
                    else {
                        break;
                    }
                }

                if (q_value_string.length() > 0) {
                    q_value = std::stod(q_value_string);
                }
            }
        }

        retval.push_back(stored_t(q_value, value));
    }

    std::sort(std::begin(retval), std::end(retval),
              [](const auto &a, const auto &b) { return a.first > b.first; });

    return retval;
}

std::map<std::string, std::string> parse_query_string(const std::string &data) {
    std::map<std::string, std::string> retval;
    std::string key_value;
    for (std::size_t i = 0; i < data.size(); i++) {
        if (data[i] == '&' || data[i] == ';' || i == data.size() - 1) {
            if (data[i] != '&' && data[i] != ';') {
                key_value += data[i];
            }
            auto key_value_pair = split_once(key_value, '=');
            bool ok;
            retval[url_decode(std::move(key_value_pair.first), ok)] =
                url_decode(std::move(key_value_pair.second), ok);
            key_value = "";
        }
        else {
            key_value += data[i];
        }
    }

    return retval;
}

request::ParseResult request::parse_result() const {
    return m_parse_result;
}

std::string request::allowed_methods_string() const {
    std::string retval = "";
    for (std::size_t i = 0; i < m_allowed_methods.size(); i++) {
        if (i != 0) {
            retval += ", ";
        }

        retval += m_allowed_methods[i];
    }

    return retval;
}

request::operator bool() const {
    return m_parse_result == Finished;
}

const std::vector<std::string> &request::allowed_methods() const {
    return m_allowed_methods;
}

void request::set_allowed_methods(
    const std::vector<std::string> &allowed_methods) {
    m_allowed_methods = allowed_methods;
}

std::size_t request::max_uri_length() const {
    return m_max_uri;
}

void request::set_max_uri_length(const std::size_t &max_uri) {
    m_max_uri = max_uri;
}

void request::set_parse_result(const ParseResult &parse_result) {
    m_parse_result = parse_result;
}

bool request::check_accepts(std::string value, std::string header_value,
                            std::string always_accepts) {
    auto lower_case_value = string_util::to_lower(value);

    auto all_accepted_values = parse_header_with_qvalues(header_value);

    if (all_accepted_values.size() == 0) {
        return always_accepts == value;
    }

    auto value_iter =
        std::find_if(all_accepted_values.begin(), all_accepted_values.end(),
                     [&](const auto &data) -> bool {
                         if (data.second == lower_case_value) {
                             return true;
                         }
                         return false;
                     });

    if (value_iter != all_accepted_values.end()) {
        return value_iter->first > 0.0001;
    }
    else {
        // the specified value was not found
        // so look for the * value
        auto catch_all_value_iter =
            std::find_if(all_accepted_values.begin(), all_accepted_values.end(),
                         [&](const auto &data) -> bool {
                             if (data.second == "*") {
                                 return true;
                             }
                             return false;
                         });

        if (catch_all_value_iter != all_accepted_values.end()) {
            return catch_all_value_iter->first > 0.0001;
        }
    }

    // The catch all value wasn't found either, so
    // neither the queried encoding nor the catch all was
    // found so check if encoding == always_accepts in which case
    // it is accepted.
    if (always_accepts.length() > 0) {
        return lower_case_value == always_accepts;
    }

    return false;
}

void request::log(int level, const std::string &data) {
    if (m_log_level >= level) {
        std::cout << data << std::endl;
    }
}

int request::log_level() const {
    return m_log_level;
}

void request::set_log_level(int log_level) {
    m_log_level = log_level;
}

bool request::accepts_media_type(const std::string &media_type) {
    std::string accept_header = header_value("accept");
    if (accept_header.length() == 0) {
        return true;
    }

    auto media_type_split = string_util::split_once(media_type, '/');
    auto queried_type = media_type_split.first;
    auto queried_subtype = media_type_split.second;

    auto all_accepted_mediatypes = parse_header_with_qvalues(accept_header);

    auto type_subtype_iter = std::find_if(
        all_accepted_mediatypes.begin(), all_accepted_mediatypes.end(),
        [&](auto &v) { return v.second == media_type; });

    if (type_subtype_iter != all_accepted_mediatypes.end()) {
        // Found the full media type
        return type_subtype_iter->first > 0.0001;
    }
    else {
        auto to_find = queried_type + "/*";
        auto type_any_subtype = std::find_if(
            all_accepted_mediatypes.begin(), all_accepted_mediatypes.end(),
            [&](auto &v) { return v.second == to_find; });
        if (type_any_subtype != all_accepted_mediatypes.end()) {
            // found the media type with any sub type
            return type_any_subtype->first > 0.0001;
        }
        else {
            auto any_type = std::find_if(
                all_accepted_mediatypes.begin(), all_accepted_mediatypes.end(),
                [&](auto &v) { return v.second == "*/*"; });

            if (any_type != all_accepted_mediatypes.end()) {
                // Found the catch-all media type
                return type_any_subtype->first > 0.0001;
            }
        }
    }

    return false;
}

bool request::accepts_charset(const std::string &charset) {
    return check_accepts(std::move(charset), header_value("accept-charset"),
                         "iso-8859-1");
}

bool request::accepts_language(const std::string &language) {
    auto language_header = header_value("accept-laguage");
    if (check_accepts(language, language_header, "") == false) {
        // split and check the prefix of the tag.
        auto tag_prefix = string_util::split_once(language, '-').first;
        return check_accepts(tag_prefix, language_header, "");
    }

    return true;
}

bool request::accepts_encoding(const std::string &encoding) {
    return check_accepts(std::move(encoding), header_value("accept-encoding"),
                         "identity");
}

request::request() : m_allowed_methods{"GET", "POST", "PUT"} {
    reset();
}

void request::reset() {
    m_request_collector = "";
    m_headers.clear();
    m_majorVersion = -1;
    m_minorVersion = -1;
    m_method = "";
    m_query_string_values.clear();
    m_state = WaitingMethodStart;
    m_uri = "";
    m_request_data_to_read = 0;
    m_request_data.clear();
    m_parse_result = NotFinished;
    m_query_string_length = 0;
    m_fragment = "";
}

const std::string &request::method() const {
    return m_method;
}

const std::string &request::uri() const {
    return m_uri;
}

size_t request::content_length() const {
    auto content_length_header = m_headers.value("content-length");
    if (content_length_header.size() != 0) {
        return std::stoul(content_length_header);
    }
    return 0;
}

bool request::has_header(const std::string &header_name) const {
    return m_headers.contains(header_name);
}

std::string request::header_value(const std::string &header_name) const {
    return m_headers.value(header_name);
}

const header_collection &request::headers() const {
    return m_headers;
}

request::query_values_t &request::query_values() {
    return m_query_string_values;
}

const request::query_values_t &request::query_values() const {
    return m_query_string_values;
}

const std::string &request::fragment() const {
    return m_fragment;
}

bool request::error() const {
    return (m_parse_result != NotFinished) && (m_parse_result != Finished);
}

request &request::operator<<(char c) {
    if (m_state >= WaitingMethodStart && m_state < WaitingData) {
        if (c <= 0) {
            m_state = ResetRequired;
            m_parse_result = BadRequest;
        }
    }
    switch (m_state) {
    case WaitingMethodStart:
        if (c >= 'A' && c <= 'Z') {
            // Always reset at the start of a new cycle
            reset();
            m_method = c;
            m_state = CollectingMethod;
        }
        break;

    case CollectingMethod:
        if (c == ' ') {
            if (std::find(m_allowed_methods.begin(), m_allowed_methods.end(),
                          m_method) == m_allowed_methods.end()) {
                m_state = ResetRequired;
                m_parse_result = MethodNotAllowed;
                log(1, "Method not allowed: " + m_method);
            }
            else {
                log(3,
                    "Ending CollectingMethod m_method = \"" + m_method + "\"");
                m_state = WaitingUriStart;
            }
        }
        else {
            m_method += c;
            auto max_allowed_method =
                std::max_element(std::begin(m_allowed_methods),
                                 std::end(m_allowed_methods),
                                 [](const auto &a, const auto &b) {
                                     return a.length() < b.length();
                                 })
                    ->length();
            if (m_method.size() > max_allowed_method) {
                log(1, "Method not allowed: " + m_method);
                m_parse_result = MethodNotAllowed;
                m_state = ResetRequired;
            }
        }
        break;

    case WaitingUriStart:
        if (c != ' ') {
            m_state = CollectingUri;
            m_request_collector = c;
        }
        break;

    case CollectingUri:
        if (c == '?' || c == '#' || c == ' ') {
            bool uri_ok = true;
            m_uri = url_decode(m_request_collector, uri_ok);
            m_request_collector = "";
            log(3, "Ending CollectingUri, URI = \"" + m_uri + "\"");
            if (uri_ok == false) {
                m_state = ResetRequired;
                m_parse_result = BadRequest;
            }
            else if (c == '?') {
                m_state = CollectingQuery;
            }
            else if (c == '#') {
                m_state = CollectingFragment;
            }
            else {
                m_state = WaitingVersionStart;
            }
        }
        else {
            m_request_collector += c;
            if (m_request_collector.size() > m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case CollectingQuery:
        if (c == '#' || c == ' ') {
            log(1, "Query string received: " + m_request_collector);
            m_query_string_values = parse_query_string(m_request_collector);
            m_request_collector = "";
            for (auto c : m_query_string_values) {
                log(3, "\t\"" + c.first + "\" = \"" + c.second + "\"");
            }
            if (c == '#') {
                m_state = CollectingFragment;
            }
            else {
                m_state = WaitingVersionStart;
            }
        }
        else {
            m_request_collector += c;
            m_query_string_length++;
            if (m_request_collector.size() + m_uri.size() > m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case CollectingFragment:
        if (c == ' ') {
            m_state = WaitingVersionStart;
            m_fragment = m_request_collector;
            m_request_collector = "";
        }
        else {
            m_request_collector += c;
            if (m_request_collector.size() + m_uri.size() +
                    m_query_string_length >
                m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case WaitingVersionStart:
        if (c != ' ') {
            m_request_collector = c;
            m_state = CollectingVersion;
        }
        break;

    case CollectingVersion:
        if (c == '\r') {
            m_state = WaitingHeader;
            std::regex version_regex("HTTP/(\\d+)\\.(\\d+)");
            std::smatch matches;
            bool valid_version = true;
            if (std::regex_match(m_request_collector, matches, version_regex)) {
                if (matches.size() == 3) {
                    m_majorVersion = std::stoi(matches[1]);
                    m_minorVersion = std::stoi(matches[2]);

                    log(3, "HTTP version = " + std::to_string(m_majorVersion) +
                               "." + std::to_string(m_minorVersion));
                    if (m_majorVersion != 1 || m_minorVersion != 1) {
                        m_state = ResetRequired;
                        m_parse_result = UnsupportedVersion;
                    }
                }
                else {
                    valid_version = false;
                }
            }
            else {
                valid_version = false;
            }
            if (valid_version) {
                m_request_collector = c;
            }
            else {
                m_state = WaitingMethodStart;
                m_request_collector = c;
            }
        }
        else {
            m_request_collector += c;
        }
        break;

    case WaitingHeader:
        m_request_collector += c;
        if (ends_with(m_request_collector, "\r\n\r\n")) {
            log(1, "Ending WaitingHeader...");
            m_headers.parse(m_request_collector);

            for (auto i : m_headers) {
                log(3, "\tHeader: \"" + m_headers.key(i) + "\" = \"" +
                           m_headers.value(i) + "\"");
            }

            m_request_data_to_read = content_length();
            if (m_request_data_to_read != 0) {
                m_state = WaitingData;
                log(3, "Waiting data: " +
                           std::to_string(m_request_data_to_read) + " bytes");
                m_request_data.reserve(m_request_data_to_read);
                log(3, "m_request_data.size() = " +
                           std::to_string(m_request_data.size()));
            }
            else {
                m_parse_result = Finished;
                log(2, "Finished");
                m_state = ResetRequired;
            }
        }
        else {
            if (m_request_collector.size() >= 3 &&
                (m_request_collector[0] == ' ' ||
                 m_request_collector[1] == ' ' ||
                 m_request_collector[2] == ' ')) {
                // Ignore everything!
                m_state = ResetRequired;
                m_parse_result = BadRequest;
                log(1, "Bad request detected...bad header format");
            }
        }
        break;

    case WaitingData:
        m_request_data.push_back(c);
        if (m_request_data.size() == m_request_data_to_read) {
            log(3, "Finished receiving data. Received " +
                       std::to_string(m_request_data.size()) + " bytes");
            m_state = ResetRequired;
            m_parse_result = Finished;
        }
        break;

    case ResetRequired:
        // Some error. Must manually reset, does nothing!
        break;
    }

    return *this;
}

} // namespace httpdlib
