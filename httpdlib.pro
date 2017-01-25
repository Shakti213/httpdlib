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
    src/httpdlib/response.cpp \
    src/httpdlib/response_generator.cpp \
    src/httpdlib/memory_response.cpp \
    src/httpdlib/filesystem_response_generator.cpp \
    src/httpdlib/content_type.cpp

HEADERS += \
    webserver.h \
    include/httpdlib/string_util.h \
    include/httpdlib/request.h \
    include/httpdlib/header_collection.h \
    include/httpdlib/response.h \
    include/httpdlib/response_generator.h \
    include/httpdlib/codes.h \
    include/httpdlib/memory_response.h \
    include/httpdlib/filesystem_response_generator.h \
    include/httpdlib/content_type.h

INCLUDEPATH += include/

DISTFILES +=
