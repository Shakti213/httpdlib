CONFIG += c++14

TARGET = httpdlib_example_stdin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

unix: LIBS += -lstdc++fs

INCLUDEPATH += ../../include

SOURCES += \
    main.cpp \
    ../../src/httpdlib/header_collection.cpp \
    ../../src/httpdlib/request.cpp \
    ../../src/httpdlib/string_util/string_util.cpp

