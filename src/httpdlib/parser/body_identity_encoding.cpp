#include "httpdlib/parser/body_identity_encoding.h"

namespace httpdlib
{
namespace parser
{

body_identity_encoding::body_identity_encoding(size_t byte_count,
                                               std::vector<char> &storage)
    : m_byte_count(byte_count), m_storage(storage) {
}

bool body_identity_encoding::done() const {
    return m_byte_read == m_byte_count;
}

bool body_identity_encoding::add_data(char data) {
    m_storage.push_back(data);
    return true;
}

size_t body_identity_encoding::size() const {
    return m_byte_read;
}
}
}
