#ifndef HEADER_H
#define HEADER_H

#include "httpdlib/header_collection.h"
#include "httpdlib/interface/data_parser.h"

namespace httpdlib
{

namespace parser
{
class header : public interface::data_parser
{
    header_collection &m_header_storage;
    bool m_done = false;
    std::string m_collector;

    int m_error_code = 0;
    std::size_t m_byte_count = 0;

public:
    header(header_collection &header_storage);
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
};
}
}

#endif // HEADER_H
