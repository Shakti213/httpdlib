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

public:
    request();

    /**
     * @brief Checks if the request will accept the specified media type
     * @param Media type to check if the request will accept.
     * @return  True if accepted, otherwise false.
     */
    bool accepts_media_type(std::string media_type);

    /**
     * @brief Checks if the request will accept a specified charset.
     * @param charset The charset to check.
     * @return True if the charset is accepted, otherwise false.
     */
    bool accepts_charset(std::string charset);

    /**
     * @brief Checks if the request will accept a specified language
     * @param language The language to check. Can be a fully specified language
     * (eg. "en-gb") or without tag (eg. "en").
     * @return True if accepted, otherwise false.
     */
    bool accepts_language(std::string language);

    /**
     * @brief Checks if the request will accepted the specified encoding.
     * @param encoding The encoding to check.
     * @return True if accepted, otherwise false.
     */
    bool accepts_encoding(std::string encoding);
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
    std::string method() const;
    /**
     * @brief The URI from the request line.
     * @return
     *
     * This does not include the query string or the fragment string.
     */
    std::string uri() const;
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
    bool has_header(std::string header_name);
    /**
     * @brief Retrieves the header value
     * @param Name of the header to get the value of
     * @return Value if available, otherwise empty string.
     */
    std::string header_value(std::string header_name);

    /**
     * @brief Gets the query values.
     *
     * @return Key-value store where values are stored as strings.
     */
    query_values_t &query_values();
    const query_values_t &query_values() const;

    const std::string &fragment() const;

    /**
     * @brief Add a buffer of data with explicit length.
     * @param data The data to add.
     * @param length Length of the data to be added.
     */
    void add_data(const char *data, std::size_t length) {
        for (std::size_t i = 0; i < length; i++) {
            *this << data[i];
        }
    }

    /**
     * @brief Adds data from a null-terminated string of data
     * @param str The null-terminated string to add.
     * @return *this.
     */
    request &operator<<(const char *str) {
        for (const char *p = str; *p; p++) {
            *this << *p;
        }

        return *this;
    }

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
};
}

#endif // HTTPREQUEST_H
