#ifndef STREAM_RESPONSE_H
#define STREAM_RESPONSE_H

#include "httpdlib/interface/response.h"
#include <istream>
#include <memory>

namespace httpdlib
{

class stream_response : public interface::response
{
    std::unique_ptr<std::istream> m_stream;
    char m_buffer[150000];
    std::size_t m_buffer_offset;
    std::size_t m_buffer_length;
    std::size_t m_size;

public:
    stream_response(std::unique_ptr<std::istream> stream);
    stream_response(std::istream *stream);
    std::size_t size();

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

#endif // STREAM_RESPONSE_H