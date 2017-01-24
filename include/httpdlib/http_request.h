#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include <vector>
#include "http_header_collection.h"

namespace httpdlib
{

class http_request
{
public:
    enum ParseState {
        NotFinished,
        Finished,
        BadRequest = 400,
        NotImplemented = 501,
        UriTooLong = 414
    };

private:

    enum State
    {
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
    std::map<std::string, std::string>  m_query_string_values;
    std::size_t m_query_string_length;
    int m_majorVersion;
    int m_minorVersion;

    http_header_collection    m_headers;


    std::size_t         m_request_data_to_read;
    std::vector<char>   m_request_data;

    ParseState  m_parse_result;

    std::vector<std::string>    m_allowed_methods;

    std::size_t         m_max_uri = 16000;

public:
    http_request();
    void reset();

    std::size_t content_length() const;
    bool        has_header(std::string header_name);
    std::string header_value(std::string header_name);

    void add_data(const char *data, std::size_t length) {
        for(std::size_t i=0; i<length; i++) {
            *this << data[i];
        }
    }

    http_request& operator<<(const char *str) {
        for(const char *p = str; *p; p++) {
            *this << *p;
        }

        return *this;
    }

    template<typename Container>
    http_request& operator<<(const Container &c) {
        for(auto i: c) {
            *this << i;
        }

        return *this;
    }

    http_request& operator<<(char c);
    ParseState parse_state() const;
    void setParse_state(const ParseState &parse_state);
};

}

#endif // HTTPREQUEST_H
