QT += core network gui

unix:DEFINES+=PUB_HTML=\\\"$$(HOME)/public_html\\\"
win32:DEFINES+=PUB_HTML=\\\"c:/www-data\\\"

CONFIG += c++14
CONFIG -= use_platform_content_types

TARGET = httpdlib_example_qt_webserver
CONFIG += console
CONFIG -= app_bundle

DEFINES += USE_EXPERIMENTAL_FS

TEMPLATE = app

unix: LIBS += -lstdc++fs

SOURCES += main.cpp \
    webserver.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    ../../src/httpdlib/request.cpp \
    ../../src/httpdlib/header_collection.cpp \
    ../../src/httpdlib/memory_response.cpp \
    ../../src/httpdlib/filesystem_response_generator.cpp \
    ../../src/httpdlib/util/content_type.cpp \
    ../../src/httpdlib/pointer_response.cpp \
    ../../src/httpdlib/response_generator_collection.cpp \
    ../../src/httpdlib/interface/response.cpp \
    ../../src/httpdlib/interface/response_generator.cpp \
    ../../src/httpdlib/stream_response.cpp \
    ../../src/httpdlib/parser/request_line.cpp \
    ../../src/httpdlib/interface/data_parser.cpp \
    ../../src/httpdlib/parser/header.cpp \
    ../../src/httpdlib/parser/body_chunked_encoding.cpp \
    ../../src/httpdlib/parser/body_identity_encoding.cpp \
    ../../src/httpdlib/buffer/adapter/istream_adapter.cpp

HEADERS += \
    webserver.h \
    ../../include/httpdlib/string_util/string_util.h \
    ../../include/httpdlib/request.h \
    ../../include/httpdlib/header_collection.h \
    ../../include/httpdlib/codes/codes.h \
    ../../include/httpdlib/memory_response.h \
    ../../include/httpdlib/filesystem_response_generator.h \
    ../../include/httpdlib/util/content_type.h \
    ../../include/httpdlib/pointer_response.h \
    ../../include/httpdlib/response_generator_collection.h \
    ../../include/httpdlib/interface/response.h \
    ../../include/httpdlib/interface/response_generator.h \
    ../../include/httpdlib/stream_response.h \
    ../../src/httpdlib/util/platform/content_type.h

INCLUDEPATH += ../../include/

use_platform_content_types {
    DEFINES *= USE_PLATFORM_CONTENT_TYPES

    win32:SOURCES *= ../../src/httpdlib/util/platform/win32_content_type.cpp
    !win32:SOURCES *= ../../src/httpdlib/util/platform/stub_content_type.cpp
}

DISTFILES +=
