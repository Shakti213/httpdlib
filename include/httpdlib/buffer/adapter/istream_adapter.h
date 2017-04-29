#ifndef ISTREAM_ADAPTER_H
#define ISTREAM_ADAPTER_H

#include <istream>
#include <memory>

namespace httpdlib
{
namespace buffer
{
namespace adapter
{

class istream_adapter
{
    std::unique_ptr<std::istream> m_stream;

public:
    istream_adapter(std::unique_ptr<std::istream> stream)
        : m_stream(std::move(stream)) {
    }
    istream_adapter(std::istream *stream) : m_stream(stream) {
    }

    std::size_t size() {
        auto current_pos = m_stream->tellg();
        m_stream->seekg(0, m_stream->end);
        std::size_t retval = static_cast<std::size_t>(m_stream->tellg());
        m_stream->seekg(current_pos, m_stream->beg);
        return retval;
    }

    std::size_t read(char *data, std::size_t max_size) {
        m_stream->read(data, static_cast<std::streamsize>(max_size));
        auto bytes_read = m_stream->gcount();
        return static_cast<std::size_t>((bytes_read < 0 ? 0 : bytes_read));
    }

    bool error() {
        return !m_stream->good();
    }
};

std::unique_ptr<istream_adapter> make_istream_adapter(std::istream *stream);
}
}
}

#endif // ISTREAM_ADAPTER_H
