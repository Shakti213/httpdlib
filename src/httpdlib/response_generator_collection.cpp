#include "httpdlib/response_generator_collection.h"

namespace httpdlib
{

response_generator_collection::storage_t::iterator
response_generator_collection::get_generator_iter(const request &request) {
    return std::find_if(
        std::begin(m_response_generators), std::end(m_response_generators),
        [&](auto &elem) { return elem->is_handler_for_request(request); });
}

std::unique_ptr<interface::response>
response_generator_collection::get_response(const request &request) {
    auto iter = get_generator_iter(request);
    if (iter != std::end(m_response_generators)) {
        return (*iter)->get_response(request);
    }

    return nullptr;
}

void response_generator_collection::add_response_generator(
    std::unique_ptr<interface::response_generator> generator) {
    m_response_generators.push_back(std::move(generator));
}

void response_generator_collection::add_response_generator(
    interface::response_generator *generator) {
    add_response_generator(
        std::unique_ptr<interface::response_generator>(generator));
}

void response_generator_collection::clear_response_generators() {
    m_response_generators.clear();
}

bool response_generator_collection::priv_is_handler_for_request(
    const request &request) {
    auto iter = get_generator_iter(request);

    return iter != std::end(m_response_generators);
}
}
