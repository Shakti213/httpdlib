#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <memory>
#include "httpdlib/request.h"
#include "httpdlib/response.h"

namespace httpdlib
{

namespace interface
{

class response_generator
{
public:
    virtual ~response_generator();
    virtual bool is_handler_for_request(const request &request) = 0;
    virtual std::unique_ptr<response> get_response(const request &request) = 0;
};

} // namespace interface

} // namespace httpdlib

#endif // RESPONSE_GENERATOR_H
