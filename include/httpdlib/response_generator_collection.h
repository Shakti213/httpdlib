/*
 * MIT License
 *
 * Copyright (c) 2017 Andreas Wass
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef RESPONSE_GENERATOR_COLLECTION_H
#define RESPONSE_GENERATOR_COLLECTION_H

#include "httpdlib/interface/response_generator.h"
#include <memory>
#include <vector>

#include <algorithm>

namespace httpdlib
{

class response_generator_collection : public interface::response_generator
{

    typedef std::vector<std::unique_ptr<interface::response_generator>>
        storage_t;
    storage_t m_response_generators;

    storage_t::iterator get_generator_iter(const request &request);

    // response_generator interface
public:
    std::unique_ptr<interface::response>
    get_response(const request &request) override;

    void add_response_generator(
        std::unique_ptr<interface::response_generator> generator);
    void add_response_generator(interface::response_generator *generator);
    void clear_response_generators();

protected:
    bool priv_is_handler_for_request(const request &request) override;
};
}

#endif // RESPONSE_GENERATOR_COLLECTION_H
