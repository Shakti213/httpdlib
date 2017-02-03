#include <string>

namespace httpdlib
{
namespace util
{
namespace platform
{

std::string mime_from_type(const std::string &) {
    std::string retval = "application/octet-stream";
    return retval;
}
}
}
}
