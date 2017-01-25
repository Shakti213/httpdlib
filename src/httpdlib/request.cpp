#include <regex>
#include <tuple>
#include "httpdlib/request.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <string>
#include "httpdlib/string_util.h"

namespace httpdlib
{

using namespace string_util;

std::pair<std::string, std::string> parse_uri_and_query(const std::string& data) {
    std::pair<std::string, std::string> retval;

    auto uri_query_pair = split_string(data, '?');
    if(uri_query_pair.second.length() > 0) {
        retval.first = url_decode(std::move(uri_query_pair.first));
        retval.second = std::move(uri_query_pair.second);
    }
    else {
        retval.first = url_decode(data);
    }


    return retval;
}

std::map<std::string, std::string> parse_query_string(const std::string& data) {
    std::map<std::string, std::string> retval;
    std::string key_value;
    for(std::size_t i=0; i<data.size(); i++) {
        if(data[i] == '&' || data[i] == ';' || i == data.size()-1) {
            if(data[i] != '&' && data[i] != ';') {
                 key_value += data[i];
            }
            auto key_value_pair = split_string(key_value, '=');
            retval[url_decode(std::move(key_value_pair.first))] = url_decode(std::move(key_value_pair.second));
            key_value = "";
        }
        else {
            key_value += data[i];
        }
    }

    return retval;
}

request::ParseState request::parse_state() const
{
    return m_parse_result;
}

void request::set_parse_state(const ParseState &parse_state)
{
    m_parse_result = parse_state;
}

request::request():
    m_allowed_methods{"GET","POST", "PUT"}
{
    reset();
}

void request::reset()
{
    m_request_collector = "";
    m_headers.clear();
    m_majorVersion = -1;
    m_minorVersion = -1;
    m_method = "";
    m_query_string_values.clear();
    m_state = WaitingMethodStart;
    m_uri = "";
    m_request_data_to_read = 0;
    m_request_data.clear();
    m_parse_result = NotFinished;
    m_query_string_length = 0;
}

std::string request::method() const
{
    return m_method;
}

std::string request::uri() const
{
    return m_uri;
}

size_t request::content_length() const
{
    auto content_length_header = m_headers.value("content-length");
    if(content_length_header.size() != 0) {
        return std::stoul(content_length_header);
    }
    return 0;
}

bool request::has_header(std::string header_name)
{
    return m_headers.has_header(std::move(header_name));
}

std::string request::header_value(std::string header_name)
{
    return m_headers.value(std::move(header_name));
}

request &request::operator<<(char c)
{
    switch(m_state)
    {
    case WaitingMethodStart:
        if(c >= 'A' && c <= 'Z') {
            // Always reset at the start of a new cycle
            reset();
            m_method = c;
            m_state = CollectingMethod;
        }
        break;

    case CollectingMethod:
        if(c == ' ') {
            if(std::find(m_allowed_methods.begin(), m_allowed_methods.end(), m_method) == m_allowed_methods.end()) {
                m_state = ResetRequired;
                m_parse_result = BadRequest;
                std::cout << "Method not allowed: " << m_method << std::endl;
            }
            else {
                std::cout << "Ending CollectingMethod m_method = \"" << m_method << "\"" << std::endl;
                m_state = WaitingUriStart;
            }
        }
        else {
            m_method += c;
            if(m_method.size() > 4) {
                std::cout << "Method not implemented..." << std::endl;
                m_parse_result = NotImplemented;
                m_state = ResetRequired;
            }
        }
        break;

    case WaitingUriStart:
        if(c != ' ') {
            m_state = CollectingUri;
            m_request_collector = c;
        }
        break;

    case CollectingUri:
        if(c == '?' || c == '#' || c == ' ') {
            m_uri = std::move(url_decode(m_request_collector));
            m_request_collector = "";
            std::cout << "Ending CollectingUri, URI = \"" << m_uri << "\"" << std::endl;
            if(c == '?') {
                m_state = CollectingQuery;
            }
            else if(c == '#') {
                m_state = CollectingFragment;
            }
            else {
                m_state = WaitingVersionStart;
            }
        }
        else {
            m_request_collector += c;
            if(m_request_collector.size() > m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case CollectingQuery:
        if(c == '#' || c == ' ') {
            std::cout << "Query string received: " << m_request_collector << std::endl;
            m_query_string_values = std::move(parse_query_string(m_request_collector));
            m_request_collector = "";
            for(auto c: m_query_string_values) {
                std::cout << "\t\"" << c.first << "\" = \"" << c.second << "\"" << std::endl;
            }
            if(c == '#') {
                m_state = CollectingFragment;
            }
            else {
                m_state = WaitingVersionStart;
            }
        }
        else {
            m_request_collector += c;
            m_query_string_length++;
            if(m_request_collector.size() + m_uri.size() > m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case CollectingFragment:
        if(c == ' ') {
            m_state = WaitingVersionStart;
            std::cout << "Fragment received: \"" << m_request_collector << "\"" << std::endl;
        }
        else {
            m_request_collector += c;
            if(m_request_collector.size() + m_uri.size() + m_query_string_length > m_max_uri) {
                m_state = ResetRequired;
                m_parse_result = UriTooLong;
            }
        }
        break;

    case WaitingVersionStart:
        if(c != ' ') {
            m_request_collector = c;
            m_state = CollectingVersion;
        }
        break;

    case CollectingVersion:
        if(c == '\r') {
            m_state = WaitingHeader;
            std::regex version_regex("HTTP/(\\d+)\\.(\\d+)");
            std::smatch matches;
            bool valid_version = true;
            if(std::regex_match(m_request_collector, matches, version_regex)) {
                if(matches.size() == 3) {
                    m_majorVersion = std::stoi(matches[1]);
                    m_minorVersion = std::stoi(matches[2]);

                    std::cout << "HTTP version = " << m_majorVersion << "." << m_minorVersion << std::endl;
                    if(m_majorVersion != 1 || m_minorVersion != 1) {
                        m_state = ResetRequired;
                        m_parse_result = UnsupportedVersion;
                    }
                }
                else {
                    valid_version = false;
                }
            }
            else {
                valid_version = false;
            }
            if(valid_version) {
                m_request_collector = c;
            }
            else {
                m_state = WaitingMethodStart;
                m_request_collector = c;
            }
        }
        else {
            m_request_collector += c;
        }
        break;

    case WaitingHeader:
        m_request_collector += c;
        if(ends_with(m_request_collector, "\r\n\r\n")) {
            std::cout << "Ending WaitingHeader..." << std::endl;
            m_headers.parse(m_request_collector);

            for(auto i: m_headers) {
                std::cout << "\tHeader: \"" << m_headers.key(i) << "\" = \"" << m_headers.value(i) << "\"" << std::endl;
            }

            m_request_data_to_read = content_length();
            if(m_request_data_to_read != 0) {
                m_state = WaitingData;
                std::cout << "Waiting data: " << m_request_data_to_read << " bytes" << std::endl;
                m_request_data.reserve(m_request_data_to_read);
                std::cout << "m_request_data.size() = " << m_request_data.size() << std::endl;
            }
            else {
                m_parse_result = Finished;
                std::cout << "Finished" << std::endl;
                m_state = ResetRequired;
            }
        }
        else {
            if(m_request_collector.size() >= 3 && (m_request_collector[0] == ' ' || m_request_collector[1] == ' ' || m_request_collector[2] == ' ')) {
                // Ignore everything!
                m_state = ResetRequired;
                m_parse_result = BadRequest;
                std::cout << "Bad request detected...bad header format" << std::endl;
            }
        }
        break;

    case WaitingData:
        m_request_data.push_back(c);
        if(m_request_data.size() == m_request_data_to_read) {
            std::cout << "Finished receiving data. Received "  << m_request_data.size() << " bytes" << std::endl;
            for(auto c: m_request_data) {
                std::cout << "\"" << c << "\" ";
            }

            std::cout << std::endl;
            m_state = ResetRequired;
            m_parse_result = Finished;
        }
        break;

    case ResetRequired:
        // Some error. Must manually reset, does nothing!
        break;

    default:
        reset();
        break;
    }

    return *this;
}


} // namespace httpdlib
