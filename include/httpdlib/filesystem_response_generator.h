#ifndef FILESYSTEM_RESPONSE_GENERATOR_H
#define FILESYSTEM_RESPONSE_GENERATOR_H

#include <string>

#include "httpdlib/response_generator.h"

namespace httpdlib
{

class filesystem_response_generator: public response_generator
{
    std::string m_directory;
public:
    filesystem_response_generator(std::string directory);

    // response_generator interface
public:
    bool is_handler_for_request(const request &request) override;
    std::unique_ptr<response> get_response(const request &req) override;
};

}

#endif // FILESYSTEM_RESPONSE_GENERATOR_H
