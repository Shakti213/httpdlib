#ifndef FILESYSTEM_RESPONSE_GENERATOR_H
#define FILESYSTEM_RESPONSE_GENERATOR_H

#include <string>

#include "httpdlib/interface/response_generator.h"

namespace httpdlib
{

/**
 * @brief Example implementation of a response_generator
 *
 * This implementation is very basic, the constructor takes a
 * directory path and all files in this directory will be served.
 *
 * Note that the serving method is very naive, it will load the entire
 * file to RAM and then serve it using memory_response.
 * A better implementation would probably used memorymapped files together
 * with a response implementation that could serve these memory-mapped files
 * without copying them to RAM first.
 *
 * Uses the file type to get the content type. All html files are
 * assumed to be utf-8 files and charset will be set to this.
 *
 */
class filesystem_response_generator: public interface::response_generator
{
    std::string m_directory;
public:
    filesystem_response_generator(std::string directory);

    // response_generator interface
public:
    std::unique_ptr<interface::response> get_response(const request &req) override;
};

}

#endif // FILESYSTEM_RESPONSE_GENERATOR_H
