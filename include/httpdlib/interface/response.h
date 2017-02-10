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
    /**
     * @brief Typedefinition for the writer_t type.
     */
    typedef std::function<std::size_t(const char *, std::size_t)> writer_t;
    virtual ~response();
    /**
     * @brief code
     * @return
     */
    virtual int code() const;
    /**
     * @brief Sets the response code.
     * @param code The code to set.
     *
     * See httpdlib/codes/codes.h for codes.
     */
    virtual void set_code(int code);
    /**
     * @brief Sets a header to the specified value.
     * @param header The header key
     * @param value The header value
     */
    virtual void set_header(const std::string &header,
                            const std::string &value);
    /**
     * @brief Should be called repeatedly while done() returns false.
     * @param writer The writer to use.
     * @return Number of bytes written at the time of the call.
     */
    virtual std::size_t write_next(writer_t writer);
    /**
     * @brief  Checks if the entire response has been written or not.
     * @return True if response has been completely written. Otherwise false.
     */
    virtual bool done() const;

protected:
    static std::string code_to_reason(int code);
    /**
     * @brief Called by write before any other writes are made
     * Must be implemented. prepare_write is called before any other
     * writes are made. After prepare_write is called the status line
     * and headers are written. After this, one or multiple calls to
     * write_payload_part is done.
     */
    virtual void prepare_write() = 0;
    /**
     * @brief Writes a part of the payload
     * @param writer The writer to use
     * @param offset The number of payload bytes already written.
     * @return  Number of bytes written by this call.
     *
     * This must be implemented by all responses.
     */
    virtual std::size_t write_payload_part(writer_t writer,
                                           std::size_t offset) = 0;
    /**
     * @brief Checks if the entire payload is written or not.
     * @param The number of payload bytes written.
     * @return  True if all bytes are written. Otherwise false.
     */
    virtual bool payload_done(std::size_t payload_bytes_written) const = 0;
    /**
     * @brief Convenience function that sets either code 200 or 204.
     * @param content_length If content_length == 0 this will set code 204.
     */
    void maybe_set_code204_or_content_length(std::size_t content_length);
    /**
     * @brief Wrapper function to write using the writer.
     * @param bytes The data buffer to write from.
     * @param length Number of bytes to write.
     * @param writer The actual writer to use.
     * @return  The number of bytes written.
     */
    std::size_t write_bytes(const char *bytes, std::size_t length,
                            writer_t writer) const;
    /**
     * @brief Gets a string that can be used as value for Date header field.
     * @return  Current UTC time as string.
     */
    std::string get_date_time() const;
};
}
}

#endif // HTTPRESPONSE_H
