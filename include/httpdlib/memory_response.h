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
 * current program state.
 *
 * For large buffers of data an implementation that does't copy
 * to an internal buffer should be considered instead.
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
    // response interface
protected:
    void prepare_write() override;
    // response interface
protected:
    size_t write_payload_part(writer_t writer, size_t offset) override;

    // response interface
protected:
    bool payload_done(size_t payload_bytes_written) const override;
};

} // namespace httpdlib

#endif // MEMORY_RESPONSE_H
