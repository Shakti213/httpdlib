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
    istream_adapter(std::unique_ptr<std::istream> stream);
    istream_adapter(std::istream *stream);

    std::size_t size();

    std::size_t read(char *data, std::size_t max_size);

    bool error();
};

std::unique_ptr<istream_adapter> adapter(std::istream *stream);
}
}
}

#endif // ISTREAM_ADAPTER_H
