#include "httpdlib/http_header_collection.h"
#include "httpdlib/string_util.h"
#include <numeric>

namespace httpdlib
{

using namespace strutil;

http_header_collection::http_header_collection()
{

}

bool http_header_collection::has_header(std::string name)
{
    auto header = m_headers.find(to_lower(std::move(name)));
    return header != m_headers.end();
}

std::string http_header_collection::value(std::string name) const
{
    auto header = m_headers.find(to_lower(std::move(name)));
    if(header != m_headers.end()) {
        return header->second;
    }

    return "";
}

void http_header_collection::add(std::string key, std::string value)
{
    m_headers[to_lower(std::move(key))] = value;
}

void http_header_collection::remove(std::string key)
{
    m_headers.erase(to_lower(std::move(key)));
}

void http_header_collection::parse(std::string str)
{
    auto all_rows = split_all(str, '\n');
    for(const auto& row: all_rows) {
        auto split_at_colon = split_string(row, ':');
        auto key = trim_both(split_at_colon.first);
        auto value = trim_both(split_at_colon.second);
        if(key.size() > 0) {
            m_headers[key] = value;
        }
    }
}

void http_header_collection::clear()
{
    m_headers.clear();
}

const std::string &http_header_collection::key(http_header_collection::iterator iter)
{
    return iter->first;
}

std::string &http_header_collection::value(http_header_collection::iterator iter)
{
    return iter->second;
}

const std::string &http_header_collection::key(http_header_collection::const_iterator iter)
{
    return iter->first;
}

const std::string &http_header_collection::value(http_header_collection::const_iterator iter)
{
    return iter->second;
}

const std::string &http_header_collection::key(const foreach_t &iter)
{
    return iter.first;
}

const std::string &http_header_collection::value(const foreach_t &iter)
{
    return iter.second;
}

http_header_collection::iterator http_header_collection::begin()
{
    return m_headers.begin();
}

http_header_collection::iterator http_header_collection::end()
{
    return m_headers.end();
}

http_header_collection::const_iterator http_header_collection::begin() const
{
    return m_headers.begin();
}

http_header_collection::const_iterator http_header_collection::end() const
{
    return m_headers.end();
}

http_header_collection::const_iterator http_header_collection::cbegin() const
{
    return m_headers.cbegin();
}

http_header_collection::const_iterator http_header_collection::cend() const
{
    return m_headers.cend();
}

std::string http_header_collection::to_string() const
{
    if(m_headers.size() == 0) {
        return "\r\n";
    }
    return std::accumulate(begin(), end(), std::string(""), [&](const std::string &acc, const foreach_t& iter) {
        return acc + key(iter) + ": " + value(iter) + "\r\n";
    });
}


} // namespace httpdlib
