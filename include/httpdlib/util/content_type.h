#ifndef CONTENT_TYPE_H
#define CONTENT_TYPE_H

#include <string>

namespace httpdlib
{

namespace util
{

std::string content_type_from_file_type(std::string type);

} // namespace util

} // namespace httpdlib

#endif // CONTENT_TYPE_H
