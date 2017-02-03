#include "httpdlib/request.h"
#include <iostream>
#include <string>

int main() {
    std::string line;
    httpdlib::request request;
    while (std::getline(std::cin, line)) {
        request << line << "\r\n";
    }

    if (!request.error()) {
        std::cout << "Parsed method: " << request.method() << std::endl;
        std::cout << "Parsed URI: " << request.uri() << std::endl;
        int i = 0;
        for (auto &qv : request.query_values()) {
            std::cout << "Query value[" << i++ << "]: " << qv.first << " = "
                      << qv.second << std::endl;
        }
        std::cout << "Parsed fragment: " << request.fragment() << std::endl;
        i = 0;
        for (auto &h : request.headers()) {
            std::cout << "Header value[" << i++ << "]: " << h.first << " = "
                      << h.second << std::endl;
        }
    }
    else {
        std::cout << "Parse error, code " << request.parse_result()
                  << std::endl;
    }
    return 0;
}
