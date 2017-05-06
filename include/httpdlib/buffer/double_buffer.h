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
#ifndef DOUBLE_BUFFER_H
#define DOUBLE_BUFFER_H

#include <memory>
#include <vector>

namespace httpdlib
{
namespace buffer
{

namespace impl
{
/**
 * @brief Provides a double buffer that can be used with buffer_response
 *
 * Internally a vector is used to store the actual data, and the maximum
 * capacity can be set when constructing the double_buffer.
 * Note that the maximum capacity might be more than what is asked, depending on
 * std::vector<char>::reserve() implementation.
 * BufferAdapterT is a small wrapper around the actual data that provides
 * at minimum the following functions (or implicit convertible to these):
 * std::size_t size() const
 * std::size_t read(char *data_buffer, std::size_t max_length_to_read)
 * bool error() const;
 */
template <typename BufferAdapterT>
class double_buffer
{
    typedef BufferAdapterT buffer_adapter_t;
    typedef std::vector<char> buffer_type_t;
    std::unique_ptr<buffer_adapter_t> m_adapter;

    buffer_type_t m_buffers[2];
    buffer_type_t *current_ptr = nullptr, *next_ptr = nullptr;
    std::size_t current_ptr_offset = 0;
    std::size_t current_ptr_end = 0;
    std::size_t next_end = 0;
    std::size_t total_bytes_written = 0;
    std::size_t file_size = 0;

public:
    double_buffer(std::unique_ptr<buffer_adapter_t> adapter,
                  std::size_t max_buf_size = 128 * 1024)
        : m_adapter(std::move(adapter)), file_size(m_adapter->size()) {
        m_buffers[0].reserve(max_buf_size);
        m_buffers[1].reserve(max_buf_size);
    }
    double_buffer(buffer_adapter_t *adapter,
                  std::size_t max_buf_size = 128 * 1024)
        : m_adapter(adapter), file_size(m_adapter->size()) {
        m_buffers[0].reserve(max_buf_size);
        m_buffers[1].reserve(max_buf_size);
    }

    void initial_read() {
        current_ptr = &m_buffers[0];
        next_ptr = &m_buffers[1];

        current_ptr_offset = 0;
        current_ptr_end =
            m_adapter->read(current_ptr->data(), current_ptr->capacity());
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
            next_end = m_adapter->read(next_ptr->data(), next_ptr->capacity());
        }
    }
};
}
template <typename T>
auto double_buffer(T *adapter, std::size_t max_buf_size = 128 * 1024)
    -> std::unique_ptr<impl::double_buffer<T>> {
    return std::unique_ptr<impl::double_buffer<T>>(
        new impl::double_buffer<T>(adapter), max_buf_size);
}
template <typename T>
auto double_buffer(std::unique_ptr<T> adapter,
                   std::size_t max_buf_size = 128 * 1024)
    -> std::unique_ptr<impl::double_buffer<T>> {
    return std::unique_ptr<impl::double_buffer<T>>(
        new impl::double_buffer<T>(std::move(adapter), max_buf_size));
}
}
}

#endif // DOUBLE_BUFFER_H
