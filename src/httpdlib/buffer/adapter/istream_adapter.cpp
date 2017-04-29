#include "httpdlib/buffer/adapter/istream_adapter.h"

namespace httpdlib
{
namespace buffer
{
namespace adapter
{

std::unique_ptr<istream_adapter> make_istream_adapter(std::istream *stream) {
    return std::unique_ptr<istream_adapter>(new istream_adapter(stream));
}
}
}
}
