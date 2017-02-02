#include <string>

#include "platform/content_type.h"
#include <algorithm>

#include <map>
static std::map<std::string, std::string> builtin_types{
    {".gif", "image/gif"},
    {".jpeg", "image/jpeg"},
    {".jpg", "image/jpg"},
    {".svg", "image/svg+xml"},
    {".webp", "image/webp"},
    {".txt", "text/plain"},
    {".css", "text/css"},
    {".html", "text/html"},
    {".htm", "text/htm"},
    {".jsx", "text/javascript"},
    {".js", "application/javascript"},
    {".pdf", "application/pdf"},
    {".xhtml", "application/xhtml+xml"},
    {".xml", "application/xml"},
    {".midi", "audio/midi"},
    {".mpeg", "audio/mpeg"},
    {".mpg", "audio/mpeg"},
    {".ogg", "audio/ogg"},
    {".oga", "audio/ogg"},
    {".spx", "audio/ogg"},
    {".wav", "audio/wav"},
    {".webm", "video/webm"},
    {".ogv", "video/ogg"},
    {".mp4", "video/mp4"}};

namespace httpdlib
{

namespace util
{

/**
 * @brief Provides ability to register new file types to the built-in list
 * @param File ending, including the leading dot (".css" for instance).
 * @param The content type that should be associated ("text/html" for instance).
 */
void content_type_register(std::string file_ending,
                           const std::string content_type) {
    if (file_ending.size() > 0) {
        if (file_ending[0] != '.') {
            file_ending += ".";
        }
        builtin_types[file_ending] = content_type;
    }
}

/**
 * @brief Retrieves the content type header to use for a certain file type
 * @param The type of file (eg ".png" for png which will return "image/png".
 * @return Content type for a certain file
 *
 * By default a small built-in list is used to generate the content type
 * but it is possible to use some platform dependant way to get the content
 * type.
 * For instance Windows has a builtin database that can be queried.
 *
 * To enable platform-specific support USE_PLATFORM_CONTENT_TYPES must be
 * defined
 * at build-time. Otherwise "application/octet-stream" will be returned for
 * unknown
 * file-types.
 */
std::string content_type_from_file_type(std::string type) {
    if (type.size() > 0) {
        if (type[0] != '.') {
            type = "." + type;
        }
    }

    auto builtin_candidate = builtin_types.find(type);
    if (builtin_candidate != builtin_types.end()) {
        return builtin_candidate->second;
    }

#ifdef USE_PLATFORM_CONTENT_TYPES
    return platform::mime_from_type(type);
#else
    return "application/octet-stream";
#endif
}
}

} // namespace httpdlib
