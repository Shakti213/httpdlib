#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <memory>
#include "httpdlib/request.h"
#include "httpdlib/response.h"

namespace httpdlib
{

class response_generator
{
public:
    virtual bool is_handler_for_request(const request &request) = 0;
    virtual std::unique_ptr<response> get_response(const request &request) = 0;
};

}

#endif // RESPONSE_GENERATOR_H
