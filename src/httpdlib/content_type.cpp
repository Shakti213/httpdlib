#include <string>

#ifdef WIN32
#include <Windows.h>
#include <urlmon.h>
#include <algorithm>
#include <iterator>

#ifdef _MSC_VER
#pragma comment( lib, "urlmon" )
#pragma comment( lib, "ole32" )
#endif

std::string impl_mime_from_type(std::string type) {
    std::string retval = "application/unknown";
    if(type.length() > 0) {
        std::wstring wstr;
        std::copy(type.begin(), type.end(), std::back_inserter(wstr));
        LPWSTR pwzMimeOut = NULL;

        HRESULT hr = FindMimeFromData(NULL, wstr.c_str(), NULL, 0, NULL, FMFD_URLASFILENAME, &pwzMimeOut, 0x00);
        if(SUCCEEDED(hr)) {
            wstr = std::wstring(pwzMimeOut);
            CoTaskMemFree(pwzMimeOut);
            retval = "";
            for(auto c: wstr) {
                retval += (char)c;
            }
        }
    }

    return retval;
}

#endif

// TODO add implementations for linux etc.

namespace httpdlib
{


std::string content_type_from_file_type(std::string type) {
    if(type.size() > 0) {
        if(type[0] != '.') {
            type = "." + type;
        }
    }

    return impl_mime_from_type(std::move(type));
}


} // namespace httpdlib

