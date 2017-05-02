#ifndef DOUBLE_BUFFER_IMPL_H
#define DOUBLE_BUFFER_IMPL_H

#include <array>
#include <memory>

namespace httpdlib
{
namespace buffer
{
namespace buffer_impl
{
template <typename BufferAdapterT>
class double_buffer
{
    typedef BufferAdapterT buffer_adapter_t;
    typedef std::array<char, 128 * 1024> buffer_type_t;
    std::unique_ptr<buffer_adapter_t> m_adapter;

    buffer_type_t m_buffers[2];
    buffer_type_t *current_ptr = nullptr, *next_ptr = nullptr;
    std::size_t current_ptr_offset = 0;
    std::size_t current_ptr_end = 0;
    std::size_t next_end = 0;
    std::size_t total_bytes_written = 0;
    std::size_t file_size = 0;

public:
    double_buffer(std::unique_ptr<buffer_adapter_t> adapter)
        : m_adapter(std::move(adapter)), file_size(m_adapter->size()) {
    }
    double_buffer(buffer_adapter_t *adapter)
        : m_adapter(adapter), file_size(m_adapter->size()) {
    }

    void initial_read() {
        current_ptr = &m_buffers[0];
        next_ptr = &m_buffers[1];

        current_ptr_offset = 0;
        current_ptr_end =
            m_adapter->read(current_ptr->data(), current_ptr->size());
        next_end = 0;
        maybe_load_next_chunk();
    }

    char *data() {
        return current_ptr->data() + current_ptr_offset;
    }

    std::size_t size() {
        return file_size;
    }

    std::size_t chunk_size() {
        return current_ptr_end - current_ptr_offset;
    }

    void bytes_written(std::size_t bytes) {
        total_bytes_written += bytes;
        current_ptr_offset += bytes;
        if (current_ptr_offset >= current_ptr_end) {
            std::swap(current_ptr, next_ptr);
            current_ptr_offset = 0;
            current_ptr_end = next_end;
            next_end = 0;
        }
    }

    void maybe_load_next_chunk() {
        if (!m_adapter->error() && next_end == 0) {
            next_end = m_adapter->read(next_ptr->data(), next_ptr->size());
        }
    }
};
}
}
}

#endif // DOUBLE_BUFFER_IMPL_H
