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
    typedef std::function<int(const char*, std::size_t)> writer_t;
    virtual ~response();

    virtual int code() const;
    virtual void set_code(int code);
    virtual void set_header(std::string header, std::string value);
    virtual std::size_t write(writer_t writer) = 0;

    static std::string code_to_reason(int code);
protected:
    // Helper functions available for conveniance.
    std::size_t write_std_string(const std::string &str, writer_t writer);
    std::size_t write_status_line(writer_t writer);
    std::size_t write_headers(writer_t writer);
    std::size_t write_headers_end(writer_t writer);
    std::size_t write_status_and_headers(writer_t writer);
    std::string get_date_time();
};

}


}

#endif // HTTPRESPONSE_H
