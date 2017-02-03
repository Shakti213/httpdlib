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
