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
#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <tuple>
#include <vector>

namespace httpdlib
{

/**
 * Contains utility functions used by httpdlib.
 */
namespace string_util
{

/**
 * @brief Decodes url-encoded data
 * @param data The string of data to decode
 * @param ok    Set to true if decoding was ok, otherwise false.
 * @return  The decoded string. If ok == false then this might be partial.
 */
std::string url_decode(const std::string &data, bool &ok);
/**
 * @brief Encodes a string to percentage-encoding.
 * @param data  The string to encode.
 * @return  Data transformed to percentage-encoding.
 */
std::string url_encode(const std::string &data);

/**
 * @brief Removes white-space chars from the beginning of a string
 * @param data The data to remove whitespace chars from
 * @return Data without any white-space chars in the beginning.
 */
std::string trim_start(const std::string &data);
/**
 * @brief Removes white-space chars from the end of a string
 * @param data The data to remove whitespace chars from
 * @return Data without any trailing white-space chars.
 */
std::string trim_end(const std::string &data);
/**
 * @brief Removes white-space chars from the beginning and end of a string
 * @param data The data to remove whitespace chars from
 * @return Data without any white-space chars in the beginning or end.
 */
std::string trim_both(const std::string &data);

/**
 * @brief Splits a string at all instances of the delimiter.
 * @param str The string to split
 * @param delimiter The delimiter to use
 * @param include_empty_strings Set to true if empty strings should be included.
 * @return A collection of strings. The delimiter is not included.
 */
std::vector<std::string> split_all(const std::string &str, char delimiter,
                                   bool include_empty_strings = true);
/**
 * @brief Splits a string at the first instance of the delimiter.
 * @param str The string to split
 * @param delimiter The delimiter to split at.
 * @return A pair of strings made up of the split parts.
 */
std::pair<std::string, std::string> split_once(const std::string &str,
                                               char delimiter);
/**
 * @brief Converts a string to lower-case.
 * @param str The string to convert.
 * @return  The string converted to lower-case.
 */
std::string to_lower(std::string str);
/**
 * @brief Checks if a string ends with a specified string or not.
 * @param string The string to check
 * @param ending The ending to check for.
 * @return  True if string has the specified ending.
 */
bool ends_with(const std::string &string, const std::string &ending);
/**
 * @brief Checks if a string begins with a specified string
 * @param string The string to check
 * @param start The start to look for
 * @return True if string begins with start
 */
bool begins_with(const std::string &string, const std::string &start);

} // namespace httpdlib::string_util

} // namespace httpdlib

#endif // STRING_UTIL_H
