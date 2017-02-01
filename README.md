# httpdlib

## What is it?
httpdlib is a simple way to read HTTP/1.1 requests and generate and send
responses to these requests. It makes no assumptions on the data transport so
can easily be used by Boost.Asio, Qt, native sockets or by any other means of
reading and writing data, even serial ports and files can easily be used to
request and receive data.

## What is it not?
It is not a library that provides a complete server solution. As previously
stated it will not make any assumptions with regards to the data transport used,
or single/multithreading or any such decisions one might want to make. It
provides easy capability of parsing HTTP requests and generating HTTP responses.
The transportation of these requests and responses is not done by this library.

## Structure
The file and folder structure is rather easy to understand. The
*include/httpdlib* folder is the root include folder, and everything in this
folder is part of the public API. Everything in this folder is available in the
httpdlib interface and sub folders also mark sub-namespaces. For instance
interface classes that can not be instantiated directly are all available in the
`httpdlib::interface` namespace and will be located in the
*include/httpdlib/interface* folder. The implementation files are located in the
corresponding *src/httpdlib* folder. So the implementation for
*include/httpdlib/request.h* is can be found in the *src/httpdlib/request.cpp*
file.

The folder (and thus also the namespace) __*platform*__ is a special folder that
is dedicated for platform specific code. This folder does not have a public API
and thus no include files are available in any folder under the *include*
directory. All files in the platform directory will have a name on the form
*__platform__\_filename.cpp* and an include file for *filename.cpp* is also
available. For example on windows *src/httpdlib/util/content_type.cpp* uses
implementations available in *src/httpdlib/util/platform/win32_content_type.cpp*.
Inside *src/httpdlib/util/platform* a header called *content_type.h* is located
which declares all the platform specific functions available.


## Usage
Everything available by httpdlib is wrapped inside the httpdlib namespace, or a
sub-namespace to this. Some basic implementations are available that allows you
start serving files from a directory, or in-memory content immediately, but for
more advanced use cases the library is very easy to build on.

### Requests
Requests are parsed using the httpdlib::request class, and the usage is pretty
much the same as when writing data to std::cout:
``` c++
std::string request_line = "GET / HTTP/1.1\r\n";
QByteArray qtByteArray = "qt-header: qt-value\r\n";
std::vector<char> vector_header{ 'v', ':', ' ', 'h', '\r', '\n'};
auto *loop_demo = "hello: world\r\n";
auto *partial_add = "\r\n  ";
httpdlib::request req;
// These are all pretty much equivalent...
req << request_line;
req << "header: value\r\n";
req << qtByteArray << vector_header;
for(auto c:loop_demo)
    req << c;
req.add_data(partial_add, 2);
```
Any container that can be used in a `for(auto element:container)` style loop can
be used with the stream operator, provided a conversion to the `char` type
exists.

To check if the request has been completely parsed the following can be used:
``` c++
if(req) {
    // Done
}
else if(req.parse_result() == httpdlib::request::Finished) {
    // Also done, equivalent
}
else if(req.parse_result() != httpdlib::request::NotFinished) {
    // An error has occurred!
}
else if(req.error()) {
    // Equivalent to parse_result() != NotFinished && parse_result() != Finished
}
```

Use `req.method()` to get the requested method and use `req.uri()` to get the
URI of request. The URI does not include any query string values or fragment,
these have seperate methods to retrieve. For details and more information about
the available methods check the `httpdlib/request.h` header file.

### Responses
Once a request has been made a response is expected. httpdlib has
a very flexible way of sending responses which is not dependant on the transport
or on serializing everything in memory.

The base class for all responses is the `httpdlib::interface::response`. As can
be inferred from the fact that it is in the interface namespace it is a base
class with at least one pure virtual function. Thus it can not be instantiated
directly but it does provides a couple of helper functions for any derived
classes.

Any derived class must implement at least these three methods: ``` c++ virtual
void response::prepare_write() = 0; virtual std::size_t
response::write_payload_part(writer_t writer, std::size_t offset) = 0; virtual
bool payload_done(std::size_t payload_bytes_written) const = 0; ```
`prepare_write()` is called by the default `response::write_next(writer_t)`
function before any writes are made. Thus any changes to the response code, or
setting of any headers based on content etc. can be made here.
`write_payload_part(writer_t writer, std::size_t offset)` It can be called
multiple times by the default write_next implementation. The offset is the
number of payload bytes that has been written up until now.
`payload_done(std::size_t payload_bytes_written) const` is used to determine if
the complete payload has been written or not.

`writer_t is a typedef inside the request class that looks like this: typedef
`std::function<std::size_t(const char*, std::size_t)> writer_t. So writer_t is a
`callable object with the signature std::size_t callable(const char*,
`std::size_t);. The first argument of this callable is a pointer to the data
`that should be written. The second argument is the length of the data to be
`written. The return value is the number of bytes actually written.

This makes the response output flexible and we can even output to several
sources at once (for instance to both the client that requested the resource as
well as to a logger).

For examples of how the response class can be derived from see
`memory_response.cpp`, `pointer_response.cpp` and stream_response.cpp in
`src/httpdlib`
