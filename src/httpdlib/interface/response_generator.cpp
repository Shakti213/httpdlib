#include <algorithm>

#include "httpdlib/interface/response_generator.h"

namespace httpdlib
{

namespace interface
{

response_generator::~response_generator() {
    // Do nothing
}

void response_generator::clear_filters() {
    m_filters.clear();
}

bool response_generator::check_filters(const request &request) {
    // If any of the filters return false std::any_of will return true,
    return !std::any_of(
        std::begin(m_filters), std::end(m_filters),
        [&request](const auto &f) { return f(request) == false; });
}

bool response_generator::priv_is_handler_for_request(const request &) {
    return true;
}

bool httpdlib::interface::response_generator::is_handler_for_request(
    const request &request) {
    return check_filters(request) && priv_is_handler_for_request(request);
}

void response_generator::add_filter(
    std::function<bool(const request &)> filter) {
    m_filters.push_back(filter);
}

} // namespace interface

} // namespace httpdlib
