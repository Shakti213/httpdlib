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
#ifndef CONTENT_TYPE_H
#define CONTENT_TYPE_H
#include <string>

namespace httpdlib
{

namespace util
{
/**
 * @brief Provides ability to register new file types to the built-in list
 * @param File ending, including the leading dot (".css" for instance).
 * @param The content type that should be associated ("text/html" for instance).
 */
void content_type_register(const std::string &file_ending,
                           const std::string &content_type);
/**
 * @brief Retrieves the content type header to use for a certain file type
 * @param The type of file (eg ".png" for png which will return "image/png".
 * @return Content type for a certain file
 *
 * By default a small built-in list is used to generate the content type
 * but it is possible to use some platform dependant way to get the content
 * type.
 * For instance Windows has a builtin database that can be queried.
 *
 * To enable platform-specific support USE_PLATFORM_CONTENT_TYPES must be
 * defined
 * at build-time. Otherwise "application/octet-stream" will be returned for
 * unknown
 * file-types.
 */
std::string content_type_from_file_type(const std::string &type);
} // namespace util

} // namespace httpdlib

#endif // CONTENT_TYPE_H
