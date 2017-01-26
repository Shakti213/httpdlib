#ifndef RESPONSE_GENERATOR_COLLECTION_H
#define RESPONSE_GENERATOR_COLLECTION_H

#include <vector>
#include <memory>
#include "httpdlib/interface/response_generator.h"


#include <algorithm>

namespace httpdlib
{

class response_generator_collection: public interface::response_generator
{

    typedef std::vector<std::unique_ptr<interface::response_generator>> storage_t;
    storage_t m_response_generators;

    storage_t::iterator get_generator_iter(const request &request);

    // response_generator interface
public:
    std::unique_ptr<interface::response> get_response(const request &request) override;

    void add_response_generator(std::unique_ptr<interface::response_generator> generator);
    void clear_response_generators();

protected:
    bool priv_is_handler_for_request(const request &request) override;
};


}

#endif // RESPONSE_GENERATOR_COLLECTION_H
