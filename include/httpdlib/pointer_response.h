#ifndef POINTER_RESPONSE_H
#define POINTER_RESPONSE_H

#include "httpdlib/interface/response.h"

namespace httpdlib
{

/**
 * @brief An implementation that doesn't copy data
 *
 * This implementation doesn't copy data, but simply
 * stores a pointer to the raw data and a length.
 * When sending
 */
class pointer_response : public interface::response
{
    const char *m_pointer;
    std::size_t m_length;

public:
    pointer_response(int code, const char *pointer, std::size_t length);

    void set_pointer(const char *pointer, std::size_t length);

    // response interface
protected:
    void prepare_write() override;
    size_t write_payload(writer_t writer, AbortPolicy abort_policy) override;
};

} // namespace httpdlib

#endif // POINTER_RESPONSE_H
