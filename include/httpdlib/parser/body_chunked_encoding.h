#ifndef BODY_CHUNKED_ENCODING_H
#define BODY_CHUNKED_ENCODING_H

#include "httpdlib/header_collection.h"
#include "httpdlib/interface/data_parser.h"
#include "httpdlib/parser/header.h"
#include <vector>

namespace httpdlib
{
namespace parser
{
class body_chunked_encoding : public interface::data_parser
{
    enum state {
        waiting_chunk_size,
        waiting_chunk_data,
        waiting_trailer,
        body_done
    };

    state m_state = waiting_chunk_size;
    header m_header_parser;
    std::vector<char> &m_storage;
    std::size_t m_bytes_left_of_chunk = 0;
    std::string m_collector;
    std::size_t m_byte_count = 0;

public:
    body_chunked_encoding(std::vector<char> &storage,
                          header_collection &headers);
    bool done() const override;
    bool add_data(char data) override;
    size_t size() const override;
};
}
}

#endif // BODY_CHUNKED_ENCODING_H
