#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>

namespace httpdlib
{

namespace util
{
// clang-format off
// clang format behaves badly on short namespaces
namespace platform
{
std::string mime_from_type(const std::string &type);
} // namespace platform
// turn on clang format again...
// clang-format on
} // namespace util
} // namespace httpdlib

#endif // PLATFORM_H
