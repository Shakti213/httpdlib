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
#include <Windows.h>
#include <algorithm>
#include <iterator>
#include <string>
#include <urlmon.h>

#ifdef _MSC_VER
#pragma comment(lib, "urlmon")
#pragma comment(lib, "ole32")
#endif

namespace httpdlib
{
namespace util
{
namespace platform
{

std::string mime_from_type(const std::string &type) {
    std::string retval = "application/unknown";
    if (type.length() > 0) {
        std::wstring wstr;
        std::copy(type.begin(), type.end(), std::back_inserter(wstr));
        LPWSTR pwzMimeOut = NULL;

        HRESULT hr = FindMimeFromData(NULL, wstr.c_str(), NULL, 0, NULL,
                                      FMFD_URLASFILENAME, &pwzMimeOut, 0x00);
        if (SUCCEEDED(hr)) {
            wstr = std::wstring(pwzMimeOut);
            CoTaskMemFree(pwzMimeOut);
            retval = "";
            for (auto c : wstr) {
                retval += static_cast<char>(c);
            }
        }
    }

    return retval;
}

} // namespace platform
} // namespace util
} // namespace httpdlib
