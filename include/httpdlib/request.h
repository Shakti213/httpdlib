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
#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "httpdlib/header_collection.h"

namespace httpdlib
{

class request
{
public:
    enum ParseResult {
        NotFinished,
        Finished,
        BadRequest = 400,
        MethodNotAllowed = 405,
        NotImplemented = 501,
        UriTooLong = 414,
        UnsupportedVersion = 505
    };

    typedef std::map<std::string, std::string> query_values_t;

private:
    enum State {
        WaitingMethodStart,
        CollectingMethod,
        WaitingUriStart,
        CollectingUri,
        CollectingQuery,
        CollectingFragment,
        WaitingVersionStart,
        CollectingVersion,
        WaitingHeader,
        WaitingData,
        ResetRequired
    };

    State m_state;

    std::string m_request_collector;

    std::string m_method;
    std::string m_uri;
    std::string m_fragment;

    query_values_t m_query_string_values;
    std::size_t m_query_string_length;

    int m_majorVersion;
    int m_minorVersion;

    header_collection m_headers;

    std::size_t m_request_data_to_read;
    std::vector<char> m_request_data;

    ParseResult m_parse_result;

    std::vector<std::string> m_allowed_methods;

    std::size_t m_max_uri = 16000;

    void set_parse_result(const ParseResult &parse_result);

    bool check_accepts(std::string value, std::string header_value,
                       std::string always_accepts);

    int m_log_level = 0;

    void log(int level, const std::string &data);

public:
    request();

    /**
     * @brief Checks if the request will accept the specified media type
     * @param Media type to check if the request will accept.
     * @return  True if accepted, otherwise false.
     */
    bool accepts_media_type(const std::string &media_type);

    /**
     * @brief Checks if the request will accept a specified charset.
     * @param charset The charset to check.
     * @return True if the charset is accepted, otherwise false.
     */
    bool accepts_charset(const std::string &charset);

    /**
     * @brief Checks if the request will accept a specified language
     * @param language The language to check. Can be a fully specified language
     * (eg. "en-gb") or without tag (eg. "en").
     * @return True if accepted, otherwise false.
     */
    bool accepts_language(const std::string &language);

    /**
     * @brief Checks if the request will accepted the specified encoding.
     * @param encoding The encoding to check.
     * @return True if accepted, otherwise false.
     */
    bool accepts_encoding(const std::string &encoding);
    /**
     * @brief Resets the request objects state
     *
     * This is used to allow a single request object
     * to parse multiple requests after one another.
     */
    void reset();

    /**
     * @brief Method parsed from the request line.
     * @return method;
     */
    const std::string &method() const;
    /**
     * @brief The URI from the request line.
     * @return
     *
     * This does not include the query string or the fragment string.
     */
    const std::string &uri() const;
    /**
     * @brief The content-length. Currently only determined by the
     * Content-Length header.
     * @return
     */
    std::size_t content_length() const;
    /**
     * @brief Checks if the header with header_name is available
     * @param The header name to look for.
     * @return True if the header is available.
     */
    bool has_header(const std::string &header_name) const;
    /**
     * @brief Retrieves the header value
     * @param Name of the header to get the value of
     * @return Value if available, otherwise empty string.
     */
    std::string header_value(const std::string &header_name) const;

    /**
     * @brief   Gets all the headers
     * @return  The header collection
     */
    const header_collection &headers() const;

    /**
     * @brief Gets the query values.
     *
     * @return Key-value store where values are stored as strings.
     */
    query_values_t &query_values();
    const query_values_t &query_values() const;
    /**
     * @brief   Gets the fragment string of the request
     * @return  The fragment string (empty if not available)
     */
    const std::string &fragment() const;

    /**
     * @brief Add a single byte of data to the request
     * @param data The data byte to add
     */
    void add_data(char data);
    /**
     * @brief Add a buffer of data with explicit length.
     * @param data The data to add.
     * @param length Length of the data to be added.
     */
    void add_data(const char *data, std::size_t length);

    /**
     * @brief Adds data from a null-terminated string of data
     * @param str The null-terminated string to add.
     * @return *this.
     */
    request &operator<<(const char *str);

    /**
     * @brief Adds data from a container.
     * @param c The container to add.
     * @return *this
     * The container must be iterable using for(auto i: c).
     */
    template <typename Container>
    request &operator<<(const Container &c) {
        for (auto i : c) {
            *this << i;
        }

        return *this;
    }

    /**
     * @brief The main parsing is done here. Operates on single bytes only.
     * @param c The character to add and parse.
     * @return *this.
     */
    request &operator<<(char c);

    /**
     * @brief Checks if an error has occurred.
     * @return True if an error has occurred, otherwise false.
     */
    bool error() const;
    /**
     * @brief Gets the parse result.
     * @return NotFinished if more data is needed, Finished if a request is
     * completed, any other value means a parse error occurred.
     */
    ParseResult parse_result() const;

    /**
     * @brief Can be used to check if a request has been fully parsed.
     */
    explicit operator bool() const;

    /**
     * @brief Returns a string with all the allowed methods.
     * @return A comma-separated string with all allowed methods.
     */
    std::string allowed_methods_string() const;

    /**
     * @brief Gets the allowed methods.
     * @return Collection of allowed methods.
     */
    const std::vector<std::string> &allowed_methods() const;
    /**
     * @brief Sets the allowed HTTP methods.
     * @param New collection of allowed methods.
     */
    void set_allowed_methods(const std::vector<std::string> &allowed_methods);
    /**
     * @brief Gets the maximum URI length. Default is 16000 bytes.
     * @return Maximum URI length.
     *
     * The length includes URL, query string and fragment.
     */
    std::size_t max_uri_length() const;
    /**
     * @brief Sets the maximum URI length.
     * @param The new maximum URI length.
     */
    void set_max_uri_length(const std::size_t &max_uri);
    int log_level() const;
    void set_log_level(int log_level);
};
}

#endif // HTTPREQUEST_H
