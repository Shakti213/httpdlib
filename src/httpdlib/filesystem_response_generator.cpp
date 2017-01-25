#include "httpdlib/filesystem_response_generator.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/codes.h"
#include "httpdlib/content_type.h"
#include <filesystem>
#include <fstream>

// Handle different filesystem namespaces
#ifdef _MSC_VER
#define fs_namespace std::experimental::filesystem
#else
#define fs_namespace std::filesystem
#endif

bool file_exists(std::string file_and_path) {
    return fs_namespace::exists(file_and_path) && std::experimental::filesystem::is_regular_file(file_and_path);
}

std::size_t file_size(std::string file_and_path) {
    return static_cast<std::size_t>(fs_namespace::file_size(file_and_path));
}

std::vector<char> read_all(std::string file_and_path) {
    std::vector<char> retval;
    auto fsize = file_size(file_and_path);
    retval.resize(fsize);

    std::ifstream fin(file_and_path, std::ios_base::in | std::ios_base::binary);
    fin.read(retval.data(), fsize);

    return std::move(retval);
}

std::string file_type(std::string file_and_path) {
    fs_namespace::path p(file_and_path);
    return p.extension().string();
}

namespace httpdlib
{


filesystem_response_generator::filesystem_response_generator(std::string directory):
    m_directory(std::move(directory))
{

}

bool filesystem_response_generator::is_handler_for_request(const request &request)
{
    if(request.method() == "GET")
        return true;

    return false;
}

std::unique_ptr<interface::response> filesystem_response_generator::get_response(const request &req)
{
    std::unique_ptr<interface::response> retval(nullptr);
    std::string file = m_directory + req.uri();
    if(req.uri() == "/") {
        file = m_directory + "/index.html";
    }
    if(req.method() != "GET") {
        retval.reset(new memory_response(codes::method_not_allowed));
    }
    else {
        bool exists = file_exists(file);
        if(!exists) {
            retval.reset(new memory_response(memory_response::default_for_code(codes::not_found)));
        }
        else {
            memory_response *resp = new memory_response(codes::ok);
            resp->set_data(std::move(read_all(file)));
            auto ctype = content_type_from_file_type(file_type(file));
            if(ctype == "text/html") {
                ctype += ";charset=utf-8";
            }
            resp->set_header("content-type", ctype);
            retval.reset(resp);
        }
    }

    return retval;
}


}