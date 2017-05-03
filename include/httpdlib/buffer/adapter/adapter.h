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
