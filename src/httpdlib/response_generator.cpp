#include "httpdlib/response_generator.h"

namespace httpdlib
{

namespace interface
{


response_generator::~response_generator()
{
    // Do nothing
}

size_t response_generator::try_write_response(const request &request, response::writer_t writer)
{
    auto response = get_response(request);
    if(response) {
        return response->write(writer);
    }

    return 0;
}

} // namespace interface

} // namespace httpdlib
