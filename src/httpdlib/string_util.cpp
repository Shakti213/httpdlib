#include "httpdlib/string_util.h"
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
    return std::isspace(c & 0x00ff);
}

}

char hex_to_ch(char ch) {
    if(ch >= 'A' && ch <= 'F'){
        return ch-'A' + 10;
    }
    else if(ch >= 'a' && ch <= 'f') {
        return ch-'a' + 10;
    }

    return ch - '0';
}

std::string char_to_percent_hex(char ch) {
    static const char *hex_table = "0123456789abcdef";
    std::string retval = "%";
    retval += hex_table[(ch >> 4) & 0x0f];
    retval += hex_table[ch & 0x0f];

    return retval;
}

std::string url_decode(const std::string &data) {
    std::string retval = "";
    std::size_t i=0;
    while(i<data.length()) {
        if(data[i] == '%' && i <= data.length()-3) {
            char ch = hex_to_ch(data[i+1])*16 + hex_to_ch(data[i+2]);
            retval += ch;
            i += 3;
        }
        else {
            retval += data[i++];
        }
    }

    return retval;
}

std::string url_encode(const std::string &data) {
    std::string retval = "";
    for(auto c: data) {
        if((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
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
    if(start == data.end()) {
        // Pure whitespace string, so return an empty string instead
        return "";
    }

    return std::string(start, data.end());
}

std::string trim_end(const std::string &data)  {
    if(data.size() == 0) {
        return "";
    }
    auto back = data.end()-1;
    while(priv::safe_isspace(*back) && back != data.begin()) {
        back--;
    }
    // Make it 1 past the end
    back++;
    return std::string(data.begin(), back);
}

std::string trim_both(const std::string &data) {
    auto retval = trim_start(data);
    retval = std::move(trim_end(std::move(retval)));

    return retval;
}

std::vector<std::string> split_all(std::string str, char delimiter) {
    std::string collector;
    collector.reserve(str.size()/2);
    std::vector<std::string> retval;
    for(auto c: str) {
        if(c == delimiter) {
            retval.push_back(collector);
            collector = "";
        }
        else {
            collector += c;
        }
    }

    if(collector.size() > 0) {
        retval.push_back(collector);
    }

    return retval;
}

std::pair<std::string, std::string> split_once(std::string str, char delimiter) {

    std::pair<std::string, std::string> retval;

    auto split_point = std::find(str.begin(), str.end(), delimiter);
    if(split_point != str.end()) {
        std::copy(str.begin(), split_point, std::back_inserter(retval.first));
        std::copy(split_point+1, str.end(), std::back_inserter(retval.second));
    }
    else {
        retval.first = std::move(str);
    }

    return retval;
}

std::string to_lower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), std::tolower);
    return std::move(str);
}

bool ends_with(std::string string, std::string ending)
{
    bool retval = false;
    if(string.size() >= ending.size()) {
        auto string_riter = string.rbegin();
        auto ending_riter = ending.rbegin();
        retval = true;
        while(ending_riter != ending.rend()) {
            if(*string_riter != *ending_riter) {
                retval = false;
                break;
            }

            string_riter++;
            ending_riter++;
        }
    }

    return retval;
}

} // namespace httpdlib::string_util

} // namespace httpdlib
