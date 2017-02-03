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
#include "httpdlib/string_util/string_util.h"
#include <algorithm>
#include <cctype>
#include <iterator>

namespace httpdlib
{

namespace string_util
{

namespace priv
{

bool safe_isspace(int c) {
    return std::isspace(c & 0x00ff) != 0;
}

char tolower(char ch) {
    return static_cast<char>(std::tolower(ch));
}
}

char hex_to_ch(char ch, bool &ok) {
    ok = true;
    if (ch >= 'A' && ch <= 'F') {
        return ch - 'A' + 10;
    }
    else if (ch >= 'a' && ch <= 'f') {
        return ch - 'a' + 10;
    }
    else if (ch >= '0' && ch <= '9') {
        return ch - '0';
    }
    ok = false;
    // Fail so return 0!
    return 0;
}

std::string char_to_percent_hex(char ch) {
    static const char *hex_table = "0123456789abcdef";
    std::string retval = "%";
    retval += hex_table[(ch >> 4) & 0x0f];
    retval += hex_table[ch & 0x0f];

    return retval;
}

std::string url_decode(const std::string &data, bool &ok) {
    std::string retval = "";
    std::size_t i = 0;

    ok = true;

    while (i < data.length()) {
        if (data[i] == '%' && i <= data.length() - 3) {
            bool ok_high, ok_low;
            char ch = hex_to_ch(data[i + 1], ok_high) * 16 +
                      hex_to_ch(data[i + 2], ok_low);
            ok = ok && ok_high && ok_low;
            if (!ok) {
                // Failure!
                return retval;
            }
            retval += ch;
            i += 3;
        }
        else if (data[i] == '%') {
            // malformed string, fail!
            ok = false;
            return retval;
        }
        else {
            retval += data[i++];
        }
    }

    return retval;
}

std::string url_encode(const std::string &data) {
    std::string retval = "";
    for (auto c : data) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            (c == '-' || c == '.' || c == '_' || c == '~')) {
            retval += c;
        }
        else {
            retval += char_to_percent_hex(c);
        }
    }

    return retval;
}

std::string trim_start(const std::string &data) {
    auto start = std::find_if_not(data.begin(), data.end(), priv::safe_isspace);
    if (start == data.end()) {
        // Pure whitespace string, so return an empty string instead
        return "";
    }

    return std::string(start, data.end());
}

std::string trim_end(const std::string &data) {
    if (data.size() == 0) {
        return "";
    }
    auto back = data.end() - 1;
    while (priv::safe_isspace(*back) && back != data.begin()) {
        back--;
    }
    // Make it 1 past the end
    back++;
    return std::string(data.begin(), back);
}

std::string trim_both(const std::string &data) {
    auto retval = trim_start(data);
    retval = trim_end(retval);

    return retval;
}

std::vector<std::string> split_all(const std::string &str, char delimiter,
                                   bool include_empty_strings) {
    std::string collector;
    collector.reserve(str.size() / 2);
    std::vector<std::string> retval;
    for (auto c : str) {
        if (c == delimiter) {
            if (collector.size() > 0 || include_empty_strings) {
                retval.push_back(collector);
            }
            collector = "";
        }
        else {
            collector += c;
        }
    }

    if (collector.size() > 0 || (include_empty_strings && str.length() > 0)) {
        retval.push_back(collector);
    }

    return retval;
}

std::pair<std::string, std::string> split_once(const std::string &str,
                                               char delimiter) {

    std::pair<std::string, std::string> retval;

    auto split_point = std::find(str.begin(), str.end(), delimiter);
    if (split_point != str.end()) {
        std::copy(str.begin(), split_point, std::back_inserter(retval.first));
        std::copy(split_point + 1, str.end(),
                  std::back_inserter(retval.second));
    }
    else {
        retval.first = str;
    }

    return retval;
}

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), priv::tolower);
    return str;
}

bool ends_with(const std::string &string, const std::string &ending) {
    bool retval = false;
    if (string.size() >= ending.size()) {
        auto string_riter = string.rbegin();
        auto ending_riter = ending.rbegin();
        retval = true;
        while (ending_riter != ending.rend()) {
            if (*string_riter != *ending_riter) {
                retval = false;
                break;
            }

            string_riter++;
            ending_riter++;
        }
    }

    return retval;
}

bool starts_with(const std::string &string, const std::string &start) {
    bool retval = false;
    if (string.size() >= start.size()) {
        auto string_iter = string.begin();
        auto start_iter = start.begin();

        while (start_iter != start.end()) {
            if (*string_iter != *start_iter) {
                return false;
            }

            string_iter++;
            start_iter++;
        }

        retval = true;
    }

    return retval;
}

} // namespace httpdlib::string_util

} // namespace httpdlib
