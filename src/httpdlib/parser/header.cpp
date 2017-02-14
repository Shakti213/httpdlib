#include "httpdlib/parser/header.h"
#include "httpdlib/codes/codes.h"
#include "httpdlib/string_util/string_util.h"

namespace httpdlib
{
namespace parser
{

header::header(header_collection &header_storage)
    : m_header_storage(header_storage) {
}

bool header::done() const {
    return m_done;
}

bool header::add_data(char data) {
    m_collector += data;
    m_byte_count++;
    if (string_util::ends_with(m_collector, "\r\n")) {
        if (m_collector == "\r\n") {
            m_done = true;
        }
        else {
            m_header_storage.parse(m_collector);
            m_collector = "";
        }
    }
    else {
        if (m_header_storage.size() == 0 && m_collector.size() >= '3') {
            if (m_collector[0] == ' ' || m_collector[1] == ' ' ||
                m_collector[2] == ' ') {
                m_error_code = codes::bad_request;
            }
        }
    }

    return true;
}

size_t header::size() const {
    return m_byte_count;
}
}
}
