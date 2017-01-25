#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <functional>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

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
public:
    typedef std::function<std::size_t(const char*, std::size_t)> writer_t;
    virtual ~response();

    virtual int code() const;
    virtual void set_code(int code);
    virtual void set_header(std::string header, std::string value);
    virtual std::size_t write(writer_t writer);

    static std::string code_to_reason(int code);
protected:
    // Must be implemented by all.
    // prepare_write is called by write before any other
    // writes are made. The status line and the headers are written
    // and finally write_payload is called.
    virtual void prepare_write() = 0;
    virtual std::size_t write_payload(writer_t writer) = 0;
    // Helper functions available for conveniance.
    // If content_length is 0 and code is 200 then this function
    // will change code to 204 and remove Content-Length header. Otherwise
    // it will set Content-Length header to content_length.
    void        maybe_set_code204_or_content_length(std::size_t content_length);
    std::size_t write_std_string(const std::string &str, writer_t writer) const;
    std::size_t write_status_line(writer_t writer) const;
    std::size_t write_headers(writer_t writer);
    std::size_t write_headers_end(writer_t writer) const;
    std::size_t write_status_and_headers(writer_t writer);
    std::size_t write_bytes(const char *bytes, std::size_t length, writer_t writer) const;
    std::string get_date_time() const;
};

}


}

#endif // HTTPRESPONSE_H
