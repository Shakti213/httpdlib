#include <string>

#include "platform/content_type.h"

namespace httpdlib
{

namespace util
{

std::string content_type_from_file_type(std::string type) {
    if (type.size() > 0) {
        if (type[0] != '.') {
            type = "." + type;
        }
    }

    return platform::mime_from_type(type);
}
}

} // namespace httpdlib
