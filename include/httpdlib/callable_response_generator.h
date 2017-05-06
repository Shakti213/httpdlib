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
#ifndef CALLABLE_RESPONSE_GENERATOR_H
#define CALLABLE_RESPONSE_GENERATOR_H

#include "httpdlib/interface/response_generator.h"
#include <functional>
#include <memory>

namespace httpdlib
{
namespace impl
{
/**
 * @brief Wrapper for a single callable call used to get a response.
 *
 * When getting the result the callable is called and the result is used
 * as a response.
 *
 * The callable must accept a const reference to request as a parameter and
 * must return a std::unique_ptr<interface::response> (or unique_ptr to derived)
 *
 */
template <typename CallableT>
class callable_response_generator
    : public httpdlib::interface::response_generator
{
    CallableT response_generator_function;

    // response_generator interface
public:
    callable_response_generator(CallableT callable)
        : response_generator_function(callable) {
    }
    std::unique_ptr<interface::response>
    get_response(const request &request) override {
        return response_generator_function(request);
    }
};
}

template <typename CallableT>
auto callable_response_generator(CallableT callable) {
    return std::unique_ptr<impl::callable_response_generator<CallableT>>(
        new impl::callable_response_generator<CallableT>(callable));
}
}

#endif // CALLABLE_RESPONSE_GENERATOR_H
