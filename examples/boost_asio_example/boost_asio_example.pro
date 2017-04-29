TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

unix:DEFINES+=PUB_HTML=\\\"$$(HOME)/public_html\\\"
win32:DEFINES+=PUB_HTML=\\\"c:/www-data\\\"

unix:LIBS += -lboost_system -lpthread

DEFINES += USE_EXPERIMENTAL_FS
unix: LIBS += -lstdc++fs

INCLUDEPATH += ../../include

win32 {
    INCLUDEPATH += c:/Boost
    LIBS += -Lc:/Boost/x86_64
}

SOURCES += main.cpp \
    ../../src/httpdlib/request.cpp \
    ../../src/httpdlib/header_collection.cpp \
    ../../src/httpdlib/memory_response.cpp \
    ../../src/httpdlib/interface/data_parser.cpp \
    ../../src/httpdlib/interface/response_generator.cpp \
    ../../src/httpdlib/interface/response.cpp \
    ../../src/httpdlib/parser/body_chunked_encoding.cpp \
    ../../src/httpdlib/parser/body_identity_encoding.cpp \
    ../../src/httpdlib/parser/header.cpp \
    ../../src/httpdlib/parser/request_line.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    ../../src/httpdlib/util/content_type.cpp \
    ../../src/httpdlib/filesystem_response_generator.cpp \
    ../../src/httpdlib/stream_response.cpp \
    ../../src/httpdlib/buffer/adapter/istream_adapter.cpp
