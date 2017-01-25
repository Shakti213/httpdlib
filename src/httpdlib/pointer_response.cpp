#include "httpdlib/pointer_response.h"


namespace httpdlib
{


pointer_response::pointer_response(int code, const char *pointer, size_t length)
{
    set_code(code);
    set_pointer(pointer, length);
}

void pointer_response::set_pointer(const char *pointer, size_t length)
{
    m_pointer = pointer;
    m_length = length;
}

void pointer_response::prepare_write()
{
    maybe_set_code204_or_content_length(m_length);
}

size_t pointer_response::write_payload(writer_t writer)
{
    return write_bytes(m_pointer, length, writer);
}



} // namespace httpdlib
