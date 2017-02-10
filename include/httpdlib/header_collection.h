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
    /**
     * @brief Iterator types as typedefs.
     */
    typedef storage_t::iterator iterator;
    typedef storage_t::const_iterator const_iterator;
    /**
     * @brief Depending on storage_t the foreach_t might not be an iterator.
     *
     * This is the type type of n in the following:
     * for(auto n: *this) {
     * }
     */
    typedef std::pair<std::string, std::string> foreach_t;

private:
    storage_t m_headers;

public:
    /**
     * @brief Checks if the header collection contains a key
     * @param key The key to check for
     * @return True if the header contains key, otherwise false.
     */
    bool contains(const std::string &key) const;
    /**
     * @brief Getter for the number of keys stored.
     * @return  The number of keys stored.
     */
    std::size_t size() const;
    /**
     * @brief Get the value stored with specified key.
     * @param key The key to look for.
     * @return The value as a string, an empty string means either key doesn't
     * exist or key holds an empty value.
     */
    std::string value(const std::string &key) const;
    /**
     * @brief parse Parses one or multiple lines and adds the key value pairs.
     * @param str   The string to parse. Can be single or multi-line
     */
    void parse(const std::string &str);
    /**
     * @brief Adds a key-value pair to the header collection.
     * @param key   The key to add.
     * @param value The value to add.
     */
    void add(const std::string &key, const std::string &value);
    /**
     * @brief Removes a key-value pair from the header collection.
     * @param key The key to remove
     */
    void remove(const std::string &key);

    /**
     * @brief Removes all key-value pairs.
     */
    void clear();

    /**
     * @brief Extracts the key given an iterator.
     * @param iter The iterator to extract the key from
     * @return  The key as a string
     */
    static const std::string &key(iterator iter);
    /**
     * @brief Extracts the value given an iterator.
     * @param iter The iterator to extract the value from.
     * @return  The value as a string.
     */
    static std::string &value(iterator iter);

    static const std::string &key(const_iterator iter);
    static const std::string &value(const_iterator iter);

    static const std::string &key(const foreach_t &iter);
    static const std::string &value(const foreach_t &iter);

    /**
     * @brief Begin and end methods are used to allow for iteration as other STL
     * containers.
     * @return Iterator to begin or end.
     */
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    const_iterator cbegin() const;
    const_iterator cend() const;
    /**
     * @brief Formats the header_collection into a std::string
     * @return The string of the header collection
     *
     * This can be used when sending a HTTP response.
     * This method will place a trailing <cr><lf> in the string.
     * An empty header collection will return a single <cr><lf>
     */
    std::string to_string() const;
};

} // namespace httpdlib

#endif // HEADERCOLLECTION_H
