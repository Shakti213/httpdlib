#include "httpdlib/header_collection.h"
#include "httpdlib/string_util.h"
#include <numeric>

namespace httpdlib
{

using namespace string_util;

header_collection::header_collection()
{

}

bool header_collection::has_header(std::string name)
{
    auto header = m_headers.find(to_lower(std::move(name)));
    return header != m_headers.end();
}

std::string header_collection::value(std::string name) const
{
    auto header = m_headers.find(to_lower(std::move(name)));
    if(header != m_headers.end()) {
        return header->second;
    }

    return "";
}

void header_collection::add(std::string key, std::string value)
{
    m_headers[to_lower(std::move(key))] = value;
}

void header_collection::remove(std::string key)
{
    m_headers.erase(to_lower(std::move(key)));
}

void header_collection::parse(std::string str)
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

void header_collection::clear()
{
    m_headers.clear();
}

const std::string &header_collection::key(header_collection::iterator iter)
{
    return iter->first;
}

std::string &header_collection::value(header_collection::iterator iter)
{
    return iter->second;
}

const std::string &header_collection::key(header_collection::const_iterator iter)
{
    return iter->first;
}

const std::string &header_collection::value(header_collection::const_iterator iter)
{
    return iter->second;
}

const std::string &header_collection::key(const foreach_t &iter)
{
    return iter.first;
}

const std::string &header_collection::value(const foreach_t &iter)
{
    return iter.second;
}

header_collection::iterator header_collection::begin()
{
    return m_headers.begin();
}

header_collection::iterator header_collection::end()
{
    return m_headers.end();
}

header_collection::const_iterator header_collection::begin() const
{
    return m_headers.begin();
}

header_collection::const_iterator header_collection::end() const
{
    return m_headers.end();
}

header_collection::const_iterator header_collection::cbegin() const
{
    return m_headers.cbegin();
}

header_collection::const_iterator header_collection::cend() const
{
    return m_headers.cend();
}

std::string header_collection::to_string() const
{
    if(m_headers.size() == 0) {
        return "\r\n";
    }
    return std::accumulate(begin(), end(), std::string(""), [&](const std::string &acc, const foreach_t& iter) {
        return acc + key(iter) + ": " + value(iter) + "\r\n";
    });
}


} // namespace httpdlib
