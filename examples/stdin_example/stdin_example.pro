CONFIG += c++14

TARGET = httpdlib_example_stdin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

DEFINES += USE_EXPERIMENTAL_FS

unix: LIBS += -lstdc++fs

INCLUDEPATH += ../../include

SOURCES += \
    main.cpp \
    ../../src/httpdlib/header_collection.cpp \
    ../../src/httpdlib/request.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    ../../src/httpdlib/interface/data_parser.cpp \
    ../../src/httpdlib/parser/header.cpp \
    ../../src/httpdlib/parser/request_line.cpp \
    ../../src/httpdlib/parser/body_chunked_encoding.cpp \
    ../../src/httpdlib/parser/body_identity_encoding.cpp

