#include "httpdlib/memory_response.h"


namespace httpdlib
{


memory_response::memory_response(int code)
{
    set_code(code);
}

void memory_response::set_data(const char *str)
{
    std::size_t length = std::strlen(str);
    set_data(str, length);
}

void memory_response::set_data(const char *data, size_t length)
{
    m_data.clear();
    m_data.reserve(length);
    std::copy(data, data+length, std::back_inserter(m_data));
}

void memory_response::set_data(std::vector<char> &&data)
{
    m_data = std::move(data);
}

void memory_response::clear_data()
{
    m_data.clear();
}

memory_response memory_response::default_for_code(int code)
{
    static std::string default_page_head = "<html><head><title>";
    static std::string default_page_body = "</title></head><body><h1>";
    static std::string default_page_footer = "</h1></body></html>";
    memory_response retval(code);
    if(code != 204 && code != 304) {
        auto code_string = std::to_string(code);
        retval.set_data(default_page_head +
                        code_string +
                        default_page_body +
                        code_string + " - " +
                        response::code_to_reason(code) +
                        default_page_footer);

        retval.set_header("content-type", "text/html;charset=utf-8");
    }

    return retval;
}

size_t memory_response::write(response::writer_t writer)
{
    if(m_code == 200 && m_data.size() == 0) {
        m_code = 204;
        m_headers.remove("content-length");
    }
    else {
        m_headers.add("content-length", std::to_string(m_data.size()));
    }

    auto retval = write_status_and_headers(writer);
    if(m_data.size() > 0) {
        retval += writer(m_data.data(), m_data.size());
    }

    return retval;
}


}