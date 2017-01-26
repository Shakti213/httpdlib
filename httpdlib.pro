QT += core network gui

CONFIG += c++11

TARGET = httpdlib
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    webserver.cpp \
    src/httpdlib/string_util.cpp \
    src/httpdlib/request.cpp \
    src/httpdlib/header_collection.cpp \
    src/httpdlib/memory_response.cpp \
    src/httpdlib/filesystem_response_generator.cpp \
    src/httpdlib/content_type.cpp \
    src/httpdlib/pointer_response.cpp \
    src/httpdlib/response_generator_collection.cpp \
    src/httpdlib/interface/response.cpp \
    src/httpdlib/interface/response_generator.cpp

HEADERS += \
    webserver.h \
    include/httpdlib/string_util.h \
    include/httpdlib/request.h \
    include/httpdlib/header_collection.h \
    include/httpdlib/codes.h \
    include/httpdlib/memory_response.h \
    include/httpdlib/filesystem_response_generator.h \
    include/httpdlib/content_type.h \
    include/httpdlib/pointer_response.h \
    include/httpdlib/response_generator_collection.h \
    include/httpdlib/interface/response.h \
    include/httpdlib/interface/response_generator.h \
    include/httpdlib/interface/response_generator_filter.h

INCLUDEPATH += include/

DISTFILES +=
