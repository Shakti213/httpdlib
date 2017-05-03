#ifndef ADAPTER_H
#define ADAPTER_H

#include <memory>

namespace httpdlib
{
namespace buffer
{
namespace adapter
{
namespace impl
{
template <typename AdaptedObjectT, typename SizeCallableT,
          typename ReadCallableT, typename ErrorCallableT>
class adapter
{
    std::unique_ptr<AdaptedObjectT> adapted_object;
    SizeCallableT size_callable;
    ReadCallableT read_callable;
    ErrorCallableT error_callable;

public:
    adapter(AdaptedObjectT *obj, SizeCallableT size_callable,
            ReadCallableT read_callable, ErrorCallableT error_callable)
        : adapted_object(obj), size_callable(size_callable),
          read_callable(read_callable), error_callable(error_callable) {
    }
    std::size_t size() const {
        return size_callable();
    }
    std::size_t read(char *output, std::size_t max_length) const {
        return read_callable(output, max_length);
    }
    bool error() const {
        return error_callable();
    }
};
}

template <typename AdaptedObjectT, typename SizeCallableT,
          typename ReadCallableT, typename ErrorCallableT>
auto adapter(AdaptedObjectT *obj, SizeCallableT size_callable,
             ReadCallableT read_callable, ErrorCallableT error_callable)
    -> std::unique_ptr<impl::adapter<AdaptedObjectT, SizeCallableT,
                                     ReadCallableT, ErrorCallableT>> {
    return std::unique_ptr<impl::adapter<AdaptedObjectT, SizeCallableT,
                                         ReadCallableT, ErrorCallableT>>(
        new impl::adapter<AdaptedObjectT, SizeCallableT, ReadCallableT,
                          ErrorCallableT>(obj, size_callable, read_callable,
                                          error_callable));
}
}
}
}

#endif // ADAPTER_H
