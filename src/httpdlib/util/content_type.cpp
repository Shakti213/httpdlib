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
#include <string>

#include "platform/content_type.h"
#include <algorithm>

#include <map>
static std::map<std::string, std::string> builtin_types{
    {".gif", "image/gif"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpg"},
    {".svg", "image/svg+xml"},
    {".webp", "image/webp"},
    {".txt", "text/plain"},
    {".css", "text/css"},
    {".html", "text/html"},
    {".htm", "text/htm"},
    {".jsx", "text/javascript"},
    {".js", "application/javascript"},
    {".pdf", "application/pdf"},
    {".xhtml", "application/xhtml+xml"},
    {".xml", "application/xml"},
    {".midi", "audio/midi"},
    {".mpeg", "audio/mpeg"},
    {".mpg", "audio/mpeg"},
    {".ogg", "audio/ogg"},
    {".oga", "audio/ogg"},
    {".spx", "audio/ogg"},
    {".wav", "audio/wav"},
    {".webm", "video/webm"},
    {".ogv", "video/ogg"},
    {".mp4", "video/mp4"}};

namespace httpdlib
{

namespace util
{

void content_type_register(const std::string &file_ending,
                           const std::string &content_type) {
    if (file_ending.size() > 0) {
        builtin_types[file_ending] = content_type;
    }
}
std::string content_type_from_file_type(const std::string &type) {
    auto builtin_candidate = builtin_types.find(type);
    if (builtin_candidate != builtin_types.end()) {
        return builtin_candidate->second;
    }

#ifdef USE_PLATFORM_CONTENT_TYPES
    return platform::mime_from_type(type);
#else
    return "application/octet-stream";
#endif
}
}

} // namespace httpdlib
