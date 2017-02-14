#ifndef BODY_IDENTITY_ENCODING_H
#define BODY_IDENTITY_ENCODING_H

#include "httpdlib/interface/data_parser.h"
#include <vector>

namespace httpdlib
{

namespace parser
{
class body_identity_encoding : public interface::data_parser
{
    std::size_t m_byte_count;
    std::vector<char> &m_storage;
    std::size_t m_byte_read = 0;

public:
    body_identity_encoding(std::size_t byte_count, std::vector<char> &storage);
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
};
}
}

#endif // BODY_IDENTITY_ENCODING_H
