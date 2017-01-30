#include <algorithm>

#include "httpdlib/interface/response_generator.h"

namespace httpdlib
{

namespace interface
{

response_generator::~response_generator() {
    // Do nothing
}

size_t response_generator::try_write_response(const request &request,
                                              response::writer_t writer) {
    if (is_handler_for_request(request)) {
        auto response = get_response(request);
        if (response) {
            return response->write(writer);
        }
    }

    return 0;
}

void response_generator::add_filter(response_generator_filter *filter) {
    add_filter(std::unique_ptr<response_generator_filter>(filter));
}

void response_generator::add_filter(
    std::unique_ptr<response_generator_filter> filter) {
    m_filters.push_back(std::move(filter));
}

void response_generator::clear_filters() {
    m_filters.clear();
}

bool response_generator::check_filters(const request &request) {
    return std::all_of(std::begin(m_filters), std::end(m_filters),
                       [&request](const auto &filter_ptr) {
                           return filter_ptr->passes_filter(request);
                       });
}

bool response_generator::priv_is_handler_for_request(const request &) {
    return true;
}

bool httpdlib::interface::response_generator::is_handler_for_request(
    const request &request) {
    return check_filters(request) && priv_is_handler_for_request(request);
}

} // namespace interface

} // namespace httpdlib
