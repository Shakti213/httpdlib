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
public:
    typedef std::function<bool(const request &)> filter_t;

protected:
    std::vector<filter_t> m_filters;

public:
    /**
     * @brief Gets a response for the specified request.
     * @param The request to generate the response to.
     * @return  A response or nullptr if the response_generator cannot satisfy
     * the request.
     */
    virtual std::unique_ptr<response> get_response(const request &request) = 0;

    virtual ~response_generator();
    /**
     * @brief Checks if a response_generator can satisfy a request
     * @param request The request to check
     * @return True if the response_generator can satisfy a request.
     *
     * This checks the filters AND priv_can_satisfy to make sure the
     * response_generator can satisfy the request.
     */
    bool can_satisfy(const request &request);
    /**
     * @brief Adds a filter to the response_generator.
     * @param filter The filter to add
     */
    void add_filter(filter_t filter);
    /**
     * @brief Removes all filters.
     */
    void clear_filters();

protected:
    /**
     * @brief Checks all filters and makes sure request passes all of them
     * @param request The request to check
     * @return True if request passes all filters, otherwise false.
     */
    virtual bool check_filters(const request &request);
    /**
     * @brief This method is always used in checks to see if a
     * response_generator can satisfy a request.
     * @param request The request to check
     * @return  True if the private check is ok. Otherwise false.
     *
     * This should only be used for checks that a user should not be able to
     * skip by clearing filters etc.
     *
     * Normally filters should be good enough, see response_generator_collection
     * for an example of when this makes sense to override.
     */
    virtual bool priv_can_satisfy(const request &request);
};

} // namespace interface

} // namespace httpdlib

#endif // RESPONSE_GENERATOR_H
