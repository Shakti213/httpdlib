/*
 * MIT License
 *
 * Copyright (c) 2017 Andreas Wass
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef HEADERCOLLECTION_H
#define HEADERCOLLECTION_H

#include <map>
#include <string>

namespace httpdlib
{

class header_collection
{
    typedef std::map<std::string, std::string> storage_t;

public:
    typedef storage_t::iterator iterator;
    typedef storage_t::const_iterator const_iterator;
    typedef std::pair<std::string, std::string> foreach_t;

private:
    storage_t m_headers;

public:
    bool contains(const std::string &name) const;
    std::string value(const std::string &name) const;
    void parse(const std::string &str);
    void add(const std::string &key, const std::string &value);
    void remove(const std::string &key);

    void clear();

    // Extract key and value from iterators without
    // knowing anything of the underlying storage
    static const std::string &key(iterator iter);
    static std::string &value(iterator iter);

    static const std::string &key(const_iterator iter);
    static const std::string &value(const_iterator iter);

    static const std::string &key(const foreach_t &iter);
    static const std::string &value(const foreach_t &iter);

    // Make this iteratable like any other container
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
