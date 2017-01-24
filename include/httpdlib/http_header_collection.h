#ifndef HEADERCOLLECTION_H
#define HEADERCOLLECTION_H

#include <map>
#include <string>

namespace httpdlib
{

class http_header_collection
{
    typedef std::map<std::string, std::string> storage_t;

public:
    typedef storage_t::iterator iterator;
    typedef storage_t::const_iterator const_iterator;
    typedef std::pair<std::string, std::string> foreach_t;

private:
    storage_t m_headers;
public:
    http_header_collection();

    bool has_header(std::string name);
    std::string value(std::string name) const;
    void parse(std::string str);
    void add(std::string key, std::string value);
    void remove(std::string key);

    void clear();

    // Extract key and value from iterators without
    // without knowing anything of the underlying storage
    static const std::string& key(iterator iter);
    static std::string& value(iterator iter);

    static const std::string& key(const_iterator iter);
    static const std::string& value(const_iterator iter);

    static const std::string& key(const foreach_t& iter);
    static const std::string& value(const foreach_t& iter);

    // Make this iteratable
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;

    std::string to_string() const;
};

} // namespace httpdlib

#endif // HEADERCOLLECTION_H
