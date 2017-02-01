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
