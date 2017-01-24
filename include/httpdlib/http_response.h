#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <functional>
#include "http_header_collection.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace httpdlib
{

class http_response
{
    http_header_collection m_headers;
    int m_code;

    std::vector<char>   m_data;
public:
    typedef std::function<int(const char*, std::size_t)> writer_t;
    http_response(int code=200);

    int code() const;
    void set_code(int code);

    void set_header(std::string header, std::string value);

    virtual std::size_t write(writer_t writer);

    void set_data(const char *str);

    template<typename T>
    void set_data(const T &data) {
        m_data.clear();
        m_data.reserve(std::size(data));
        std::copy(std::begin(data), std::end(data), std::back_inserter(m_data));
    }

    void clear_data();

    static http_response default_for_code(int code);

protected:
    std::size_t write_std_string(const std::string &str, writer_t writer);
    std::size_t write_status_line(writer_t writer);
    std::size_t write_headers(writer_t writer);
    std::size_t write_headers_end(writer_t writer);
    std::size_t write_status_and_headers(writer_t writer);
    std::string get_date_time();
};


}

#endif // HTTPRESPONSE_H
