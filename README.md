# httpdlib

## What is it?
httpdlib is a simple way to add HTTP/1.1 server functionality to a C++ program. It makes no assumptions on the data transport so can easily be used by Boost.Asio, Qt, native sockets or by any other means of reading and writing data, even serial ports and files can easily be used to request and receive data.

## Usage
Everything available by httpdlib is wrapped inside the httpdlib namespace, or a sub-namespace to this. Some basic implementations are available that allows you start serving files from a directory, or in-memory content immediately, but for more advanced use cases the library is very easy to build on.

### Requests
Requests are parsed using the httpdlib::request class, and the usage is pretty much the same as when writing data to std::cout:
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
Any container that can be used in a `for(auto element:container)` style loop can be used with the stream operator.

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
```

Use `req.method()` to get the requested method and use `req.uri()` to get the URI of request. The URI does not include any query string values or fragment, these have seperate methods to retrieve. For details check the `httpdlib/request.h` header file.

### Responses
Once a request has been made a response is expected. httpdlib has a very flexible way of sending responses which is not dependant on the transport or on serializing everything in memory.

The base class for all responses is the `httpdlib::interface::response`. As can be inferred from the fact that it is in the interface namespace it is a base class with at least one pure virtual function. Thus it can not be instantiated directly but it does provides a couple of helper functions for any derived classes.

Any derived class must implement at least these two methods:
``` c++
virtual void response::prepare_write() = 0;
virtual std::size_t response::write_payload(writer_t writer) = 0;
```
`prepare_write()` is called by the default `response::write(writer_t)` function before any writes are made. Thus any changes to the response code, or setting of any headers based on content etc. can be made here.
`write_payload(writer_t writer)` performs the actual writing of the payload and is the last write function called by the default `response::write(writer_t)`.

`writer_t` is a typedef inside the `request` class that looks like this: `typedef std::function<std::size_t(const char*, std::size_t)> writer_t`. So writer_t is a callable object with the signature `std::size_t callable(const char*, std::size_t);`. The first argument of this callable is a pointer to the data that should be written. The second argument is the length of the data to be written. The return value is the number of bytes actually written.

This makes the response output flexible and we can even output to several sources at once (for instance to both the client that requested the resource as well as to a logger).

For examples of how the response class can be derived from see `memory_response.cpp` and `pointer_response.cpp` in `src/httpdlib`


