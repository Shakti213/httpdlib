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
#ifndef MEMORY_RESPONSE_H
#define MEMORY_RESPONSE_H

#include "httpdlib/interface/response.h"
#include <iterator>

namespace httpdlib
{

/**
 * @brief Example implementation of a response
 *
 * This implementation is very basic, it copies data from
 * RAM to a private data storage (m_data) and simply serves
 * this data.
 *
 * It will set the Content-Length header automatically when writing
 * data. If no data should be written and code is set to 200 it will
 * respond with 204 (No data) instead.
 *
 * This implementation is nice to use for rather small data that is
 * programatically generated, for instance to send a response containing
 * current program state, or small in-built error pages etc.
 *
 * For large buffers of data an implementation that does't copy
 * to an internal buffer should be considered instead (see pointer_response
 * or stream_response).
 *
 */
class memory_response : public interface::response
{
    std::vector<char> m_data;

public:
    memory_response(int code = 200);

    void set_data(const char *str);
    void set_data(const char *data, std::size_t length);

    void set_data(std::vector<char> &&data);
    template <typename T>
    void set_data(const T &data) {
        m_data.clear();
        m_data.reserve(std::distance(std::begin(data), std::end(data)));
        std::copy(std::begin(data), std::end(data), std::back_inserter(m_data));
    }

    void clear_data();

    static memory_response default_for_code(int code);

protected:
    void prepare_write() override;
    size_t write_payload_part(writer_t writer, size_t offset) override;
};

} // namespace httpdlib

#endif // MEMORY_RESPONSE_H
