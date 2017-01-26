#ifndef RESPONSE_GENERATOR_FILTER_H
#define RESPONSE_GENERATOR_FILTER_H

#include "httpdlib/request.h"

namespace httpdlib
{

namespace interface
{

class response_generator_filter
{
public:
    virtual bool    passes_filter(const request &request) const = 0;
};

} // namespace interface
} // namespace httpdlib

#endif // RESPONSE_GENERATOR_FILTER_H
