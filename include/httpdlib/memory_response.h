#ifndef MEMORY_RESPONSE_H
#define MEMORY_RESPONSE_H

#include "response.h"

namespace httpdlib
{

class memory_response: public response
{
    std::vector<char>   m_data;
public:
    memory_response(int code = 200);

    void set_data(const char *str);
    void set_data(const char *data, std::size_t length);

    void set_data(std::vector<char> &&data);
    template<typename T>
    void set_data(const T &data) {
        m_data.clear();
        m_data.reserve(std::size(data));
        std::copy(std::begin(data), std::end(data), std::back_inserter(m_data));
    }

    void clear_data();

    static memory_response default_for_code(int code);
    // response interface
public:
    size_t write(writer_t writer) override;
};

} // namespace httpdlib

#endif // MEMORY_RESPONSE_H
