#include "httpdlib/stream_response.h"

namespace httpdlib
{

stream_response::stream_response(std::unique_ptr<std::istream> stream)
    : m_stream(std::move(stream)) {
    m_size = size();
}

stream_response::stream_response(std::istream *stream) : m_stream(stream) {
    m_size = size();
}

size_t stream_response::size() {
    m_stream->seekg(0, m_stream->end);
    std::size_t size = static_cast<std::size_t>(m_stream->tellg());
    m_stream->seekg(0, m_stream->beg);

    return size;
}

void stream_response::prepare_write() {

    maybe_set_code204_or_content_length(m_size);
    m_stream->read(m_buffer, sizeof(m_buffer));
    m_buffer_length = static_cast<std::size_t>(m_stream->gcount());
    m_buffer_offset = 0;
}

size_t stream_response::write_payload_part(writer_t writer, size_t) {
    std::size_t bytes_written = write_bytes(
        m_buffer + m_buffer_offset, m_buffer_length - m_buffer_offset, writer);
    if (bytes_written > 0) {
        m_buffer_offset += bytes_written;
        if (m_buffer_offset >= m_buffer_length) {
            m_buffer_offset = m_buffer_length;
            if (m_stream->good()) {
                m_buffer_offset = 0;
                m_stream->read(m_buffer, sizeof(m_buffer));
                m_buffer_length =
                    static_cast<decltype(m_buffer_length)>(m_stream->gcount());
            }
        }
    }

    return bytes_written;
}

bool stream_response::payload_done(size_t payload_bytes_written) const {
    return payload_bytes_written >= m_size;
}

} // namespace httpdlib
