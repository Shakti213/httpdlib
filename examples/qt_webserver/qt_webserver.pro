QT += core network gui

CONFIG += c++11
CONFIG -= use_platform_content_types

TARGET = httpdlib_example_qt_webserver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

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
    ../../src/httpdlib/stream_response.cpp

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
}

DISTFILES +=
