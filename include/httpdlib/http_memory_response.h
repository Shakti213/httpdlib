#ifndef HTTP_MEMORY_RESPONSE_H
#define HTTP_MEMORY_RESPONSE_H

#include "http_response.h"

namespace httpdlib
{

class http_memory_response: public http_response
{
    std::vector<char>   m_data;
public:
    http_memory_response(int code = 200);

    void set_data(const char *str);

    template<typename T>
    void set_data(const T &data) {
        m_data.clear();
        m_data.reserve(std::size(data));
        std::copy(std::begin(data), std::end(data), std::back_inserter(m_data));
    }

    void clear_data();

    static http_memory_response default_for_code(int code);
    // http_response interface
public:
    size_t write(writer_t writer) override;
};

} // namespace httpdlib

#endif // HTTP_MEMORY_RESPONSE_H
