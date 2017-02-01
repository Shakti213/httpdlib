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

std::string url_decode(const std::string &data, bool &ok);
std::string url_encode(const std::string &data);

std::string trim_start(const std::string &data);
std::string trim_end(const std::string &data);
std::string trim_both(const std::string &data);

std::vector<std::string> split_all(const std::string &str, char delimiter,
                                   bool include_empty_strings = true);
std::pair<std::string, std::string> split_once(const std::string &str,
                                               char delimiter);

std::string to_lower(std::string str);

bool ends_with(const std::string &string, const std::string &ending);
bool starts_with(const std::string &string, const std::string &start);

} // namespace httpdlib::string_util

} // namespace httpdlib

#endif // STRING_UTIL_H
