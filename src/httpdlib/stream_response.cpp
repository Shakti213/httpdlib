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
#include "httpdlib/stream_response.h"

namespace httpdlib
{

struct stream_response::buffer
{
    std::unique_ptr<std::istream> m_stream;

    std::array<char, 128 * 1024> m_buffers[2];
    std::array<char, 128 * 1024> *current_ptr = nullptr, *next_ptr = nullptr;
    std::size_t current_ptr_offset = 0;
    std::size_t current_ptr_end = 0;
    std::size_t next_end = 0;
    std::size_t total_bytes_written = 0;
    std::size_t file_size = 0;

    buffer(std::unique_ptr<std::istream> stream) : m_stream(std::move(stream)) {
        file_size = size();
    }

    void initial_read() {
        current_ptr = &m_buffers[0];
        next_ptr = &m_buffers[1];

        current_ptr_offset = 0;
        m_stream->read(current_ptr->data(), current_ptr->size());
        current_ptr_end = m_stream->gcount();
        next_end = 0;
        maybe_load_next_chunk();
    }

    char *data() {
        return current_ptr->data() + current_ptr_offset;
    }

    std::size_t size() {
        auto current_pos = m_stream->tellg();
        m_stream->seekg(0, m_stream->end);
        std::size_t retval = static_cast<std::size_t>(m_stream->tellg());
        m_stream->seekg(current_pos, m_stream->beg);
        return retval;
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
        if (m_stream->good() && next_end == 0) {
            m_stream->read(next_ptr->data(), next_ptr->size());
            next_end = m_stream->gcount();
        }
    }
};

stream_response::stream_response(std::unique_ptr<std::istream> stream) {
    m_buffer.reset(new buffer(std::move(stream)));
}

stream_response::stream_response(std::istream *stream) {
    m_buffer.reset(new buffer(std::unique_ptr<std::istream>(stream)));
}

size_t stream_response::size() const {
    return m_buffer->file_size;
}

void stream_response::prepare_write() {
    maybe_set_code204_or_content_length(m_buffer->file_size);
    m_buffer->initial_read();
}

size_t stream_response::write_payload_part(writer_t writer, size_t) {
    std::size_t bytes_written =
        write_bytes(m_buffer->data(), m_buffer->chunk_size(), writer);
    async_payload_written(bytes_written);

    return bytes_written;
}

void stream_response::async_payload_written(std::size_t bytes_written) {
    m_buffer->bytes_written(bytes_written);
    m_buffer->maybe_load_next_chunk();
}

} // namespace httpdlib
