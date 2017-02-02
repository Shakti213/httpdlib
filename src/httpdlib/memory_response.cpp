#include "httpdlib/memory_response.h"
#include <cstring>

namespace httpdlib
{

memory_response::memory_response(int code) {
    set_code(code);
}

void memory_response::set_data(const char *str) {
    std::size_t length = std::strlen(str);
    set_data(str, length);
}

void memory_response::set_data(const char *data, size_t length) {
    m_data.clear();
    m_data.reserve(length);
    std::copy(data, data + length, std::back_inserter(m_data));
}

void memory_response::set_data(std::vector<char> &&data) {
    m_data = std::move(data);
}

void memory_response::clear_data() {
    m_data.clear();
}

memory_response memory_response::default_for_code(int code) {
    static std::string default_page_head = "<html><head><title>";
    static std::string default_page_body = "</title></head><body><h1>";
    static std::string default_page_footer = "</h1></body></html>";
    memory_response retval(code);
    if (code != 204 && code != 304) {
        auto code_string = std::to_string(code);
        retval.set_data(default_page_head + code_string + default_page_body +
                        code_string + " - " + response::code_to_reason(code) +
                        default_page_footer);

        retval.set_header("content-type", "text/html;charset=utf-8");
    }

    return retval;
}

void memory_response::prepare_write() {
    maybe_set_code204_or_content_length(m_data.size());
}

size_t memory_response::write_payload_part(interface::response::writer_t writer,
                                           size_t offset) {
    return write_bytes(m_data.data() + offset, m_data.size() - offset, writer);
}

bool memory_response::payload_done(std::size_t payload_bytes_written) const {
    return payload_bytes_written >= m_data.size();
}
}
