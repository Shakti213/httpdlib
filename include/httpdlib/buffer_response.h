#ifndef BUFFER_RESPONSE_H
#define BUFFER_RESPONSE_H

#include "httpdlib/interface/response.h"
#include <memory>

namespace httpdlib
{

namespace impl
{
/**
 * @brief Class that uses a generic buffer
 * The buffer must provide the following functions:
 * std::size_t size() -> gives total size of data in buffer
 * void initial_read() -> performs an initial read of data
 * const char* data(); -> provides a pointer to the data
 * std::size_t chunk_size(); -> provides the size of currently available data
 * void bytes_written(std::size_t) -> informs the buffer of how many bytes has
 * been written
 * void maybe_load_next_chunk() -> maybe loads the next chunk if needed
 */
template <typename BufferT>
class buffer_response : public interface::response
{

    typedef BufferT buffer_t;
    std::unique_ptr<buffer_t> m_buffer;

public:
    buffer_response(std::unique_ptr<buffer_t> buffer)
        : m_buffer(std::move(buffer)) {
        set_code(200);
    }
    buffer_response(buffer_t *buffer) : m_buffer(buffer) {
        set_code(200);
    }

    std::size_t size() const {
        if (m_buffer) {
            return m_buffer->size();
        }
        return 0;
    }

    // response interface
protected:
    void prepare_write() override {
        maybe_set_code204_or_content_length(size());
        if (m_buffer) {
            m_buffer->initial_read();
        }
    }

    std::size_t write_payload_part(writer_t writer, std::size_t) {
        std::size_t bytes_written = 0;
        if (m_buffer) {
            bytes_written =
                write_bytes(m_buffer->data(), m_buffer->chunk_size(), writer);
            async_payload_written(bytes_written);
        }
        return bytes_written;
    }

    void async_payload_written(std::size_t bytes_written) {
        if (m_buffer) {
            m_buffer->bytes_written(bytes_written);
            m_buffer->maybe_load_next_chunk();
        }
    }
};
}

template <typename T>
auto buffer_response(std::unique_ptr<T> buffer)
    -> std::unique_ptr<impl::buffer_response<T>> {
    return std::unique_ptr<impl::buffer_response<T>>(
        new impl::buffer_response<T>(std::move(buffer)));
}

template <typename T>
auto buffer_response(T *buffer) -> std::unique_ptr<impl::buffer_response<T>> {
    return std::unique_ptr<impl::buffer_response<T>>(
        new impl::buffer_response<T>(buffer));
}
}
#endif // BUFFER_RESPONSE_H
