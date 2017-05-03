# This does not produce an actual usable library, however to enable
# build testing we specify this as a library
INCLUDEPATH += ../include

TEMPLATE = lib

CONFIG += c++14
unix: LIBS += -lstdc++fs

HEADERS += \
           ../include/httpdlib/filesystem_response_generator.h \
           ../include/httpdlib/header_collection.h \
           ../include/httpdlib/memory_response.h \
           ../include/httpdlib/pointer_response.h \
           ../include/httpdlib/request.h \
           ../include/httpdlib/response_generator_collection.h \
           ../include/httpdlib/stream_response.h \
           ../include/httpdlib/codes/codes.h \
           ../include/httpdlib/interface/response.h \
           ../include/httpdlib/interface/response_generator.h \
           ../include/httpdlib/string_util/string_util.h \
           ../include/httpdlib/util/content_type.h \
           httpdlib/util/platform/content_type.h \
            ../include/httpdlib/interface/data_parser.h \
    ../include/httpdlib/parser/request_line.h \
    ../include/httpdlib/parser/header.h \
    ../include/httpdlib/parser/body_identity_encoding.h \
    ../include/httpdlib/parser/body_chunked_encoding.h \
    ../include/httpdlib/buffer_response.h \
    ../include/httpdlib/buffer/adapter/istream_adapter.h \
    ../include/httpdlib/buffer/double_buffer.h \
    ../include/httpdlib/buffer/adapter/adapter.h
SOURCES +=  \
           httpdlib/filesystem_response_generator.cpp \
           httpdlib/header_collection.cpp \
           httpdlib/memory_response.cpp \
           httpdlib/pointer_response.cpp \
           httpdlib/request.cpp \
           httpdlib/response_generator_collection.cpp \
           httpdlib/stream_response.cpp \
           httpdlib/interface/response.cpp \
           httpdlib/interface/response_generator.cpp \
           httpdlib/string_util/string_util.cpp \
           httpdlib/util/content_type.cpp \
    httpdlib/interface/data_parser.cpp \
    httpdlib/parser/request_line.cpp \
    httpdlib/parser/header.cpp \
    httpdlib/parser/body_identity_encoding.cpp \
    httpdlib/parser/body_chunked_encoding.cpp

use_platform_content_types {
    DEFINES *= USE_PLATFORM_CONTENT_TYPES
    win32:SOURCES *= httpdlib/util/platform/win32_content_type.cpp
    !win32:SOURCES *= httpdlib/util/platform/stub_content_type.cpp
}
