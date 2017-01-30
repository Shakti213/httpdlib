#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <algorithm>
#include <functional>
#include <iterator>
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
    // and finally write_payload is called.
    virtual void prepare_write() = 0;
    virtual std::size_t write_payload_part(writer_t writer,
                                           std::size_t offset) = 0;
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
