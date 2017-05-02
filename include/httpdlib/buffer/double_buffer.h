#ifndef ISTREAM_BUFFER_H
#define ISTREAM_BUFFER_H

#include "buffer_impl/double_buffer.h"

namespace httpdlib
{
namespace buffer
{

template <typename T>
auto double_buffer(T *adapter)
    -> std::unique_ptr<buffer_impl::double_buffer<T>> {
    return std::unique_ptr<double_buffer<T>>(
        new buffer_impl::double_buffer<T>(adapter));
}
template <typename T>
auto double_buffer(std::unique_ptr<T> adapter)
    -> std::unique_ptr<buffer_impl::double_buffer<T>> {
    return std::unique_ptr<buffer_impl::double_buffer<T>>(
        new buffer_impl::double_buffer<T>(std::move(adapter)));
}
}
}

#endif // DOUBLE_BUFFER_H
