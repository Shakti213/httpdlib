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
namespace strutil
{


std::string url_decode(const std::string &data);
std::string url_encode(const std::string &data);

std::string trim_start(const std::string &data);
std::string trim_end(const std::string &data);
std::string trim_both(const std::string &data);
std::vector<std::string> split_all(std::string str, char delimiter);
std::pair<std::string, std::string> split_string(std::string str, char delimiter);
std::string to_lower(std::string str);

bool ends_with(std::string string, std::string ending);

} // namespace httpdlib::strutil

} // namespace httpdlib


#endif // STRING_UTIL_H
