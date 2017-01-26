#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include <memory>
#include <vector>

#include "httpdlib/request.h"
#include "httpdlib/interface/response.h"
#include "httpdlib/interface/response_generator_filter.h"

namespace httpdlib
{

namespace interface
{

class response_generator
{
protected:
    std::vector<std::unique_ptr<response_generator_filter>> m_filters;
public:
    virtual ~response_generator();
    bool is_handler_for_request(const request &request);
    virtual std::size_t try_write_response(const request &request, response::writer_t writer);
    virtual std::unique_ptr<response> get_response(const request &request) = 0;

    void add_filter(response_generator_filter *filter);
    void add_filter(std::unique_ptr<response_generator_filter> filter);
    void clear_filters();
protected:
    virtual bool check_filters(const request &request);
    virtual bool priv_is_handler_for_request(const request &request);
};

} // namespace interface

} // namespace httpdlib

#endif // RESPONSE_GENERATOR_H
