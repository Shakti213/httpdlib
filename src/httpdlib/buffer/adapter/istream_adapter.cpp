#include "httpdlib/buffer/adapter/istream_adapter.h"

namespace httpdlib
{
namespace buffer
{
namespace adapter
{

std::unique_ptr<istream_adapter> adapter(std::istream *stream) {
    return std::unique_ptr<istream_adapter>(new istream_adapter(stream));
}

istream_adapter::istream_adapter(std::unique_ptr<std::istream> stream)
    : m_stream(std::move(stream)) {
}

istream_adapter::istream_adapter(std::istream *stream) : m_stream(stream) {
}

size_t istream_adapter::size() {
    auto current_pos = m_stream->tellg();
    m_stream->seekg(0, m_stream->end);
    std::size_t retval = static_cast<std::size_t>(m_stream->tellg());
    m_stream->seekg(current_pos, m_stream->beg);
    return retval;
}

size_t istream_adapter::read(char *data, size_t max_size) {
    m_stream->read(data, static_cast<std::streamsize>(max_size));
    auto bytes_read = m_stream->gcount();
    return static_cast<std::size_t>((bytes_read < 0 ? 0 : bytes_read));
}

bool istream_adapter::error() {
    return !m_stream->good();
}
}
}
}
