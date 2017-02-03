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
#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "httpdlib/header_collection.h"

namespace httpdlib
{

namespace interface
{

class response
{
protected:
    header_collection m_headers;
    int m_code;

    enum write_next_state { state_write_headers, state_write_payload };

    write_next_state m_state = state_write_headers;
    std::size_t m_write_next_offset = 0;
    std::string m_serialized_status_and_headers;

public:
    enum AbortPolicy { AbortOnZeroBytesWritten, ContinueOnZeroBytesWritten };

    typedef std::function<std::size_t(const char *, std::size_t)> writer_t;
    virtual ~response();

    virtual int code() const;
    virtual void set_code(int code);
    virtual void set_header(std::string header, std::string value);
    virtual std::size_t write_next(writer_t writer);

    virtual bool done() const;

protected:
    static std::string code_to_reason(int code);
    // Must be implemented by all.
    // prepare_write is called by write before any other
    // writes are made. The status line and the headers are written
    // and finally write_payload_part is called once or multiple times
    virtual void prepare_write() = 0;
    // Writes a part of the payload. The offset is the number of bytes
    // of the payload already written.
    virtual std::size_t write_payload_part(writer_t writer,
                                           std::size_t offset) = 0;
    // Function used to check if the entire payload has been written or not.
    virtual bool payload_done(std::size_t payload_bytes_written) const = 0;
    // Helper functions available for conveniance.
    // If content_length is 0 and code is 200 then this function
    // will change code to 204 and remove Content-Length header. Otherwise
    // it will set Content-Length header to content_length.
    void maybe_set_code204_or_content_length(std::size_t content_length);
    std::size_t write_bytes(const char *bytes, std::size_t length,
                            writer_t writer) const;
    std::string get_date_time() const;
};
}
}

#endif // HTTPRESPONSE_H
