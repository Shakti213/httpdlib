QT += core network gui

CONFIG += c++11

TARGET = QHttpServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    webserver.cpp \
    src/httpdlib/string_util.cpp \
    src/httpdlib/http_request.cpp \
    src/httpdlib/http_header_collection.cpp \
    src/httpdlib/http_response.cpp

HEADERS += \
    webserver.h \
    include/httpdlib/string_util.h \
    include/httpdlib/http_request.h \
    include/httpdlib/http_header_collection.h \
    include/httpdlib/http_response.h

INCLUDEPATH += include/

DISTFILES +=
