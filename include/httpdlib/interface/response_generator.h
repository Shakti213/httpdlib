#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <memory>
#include <vector>

#include "httpdlib/interface/response.h"
#include "httpdlib/request.h"

namespace httpdlib
{

namespace interface
{

class response_generator
{
protected:
    std::vector<std::function<bool(const request &)>> m_filters;

public:
    // public pure virtuals
    virtual std::unique_ptr<response> get_response(const request &request) = 0;

    // public virtuals
    virtual ~response_generator();

    bool is_handler_for_request(const request &request);

    void add_filter(std::function<bool(const request &)> filter);
    void clear_filters();

protected:
    // default implementatiosn is probably sane for most cases
    virtual bool check_filters(const request &request);
    virtual bool priv_is_handler_for_request(const request &);
};

} // namespace interface

} // namespace httpdlib

#endif // RESPONSE_GENERATOR_H
