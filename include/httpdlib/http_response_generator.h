#ifndef HTTP_RESPONSE_GENERATOR_H
#define HTTP_RESPONSE_GENERATOR_H

#include <memory>
#include "http_request.h"
#include "http_response.h"

namespace httpdlib
{

class http_response_generator
{
public:
    virtual bool is_handler_for_request(const http_request &request) = 0;
    virtual std::unique_ptr<http_response> response(const http_request &request) = 0;
};

}

#endif // HTTP_RESPONSE_GENERATOR_H
