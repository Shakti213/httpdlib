CONFIG += c++11

TARGET = tst_httpdlib
CONFIG += console
CONFIG -= app_bundle

SOURCES += \
    string_util/trim_tests.cpp \
    ../src/httpdlib/string_util.cpp \
    main.cpp \
    string_util/split_tests.cpp \
    string_util/url_encode_decode.cpp \
    string_util/to_lower.cpp \
    string_util/starts_ends_with.cpp

HEADERS += \
    ../include/httpdlib/string_util.h \
    test_macros.h

INCLUDEPATH += ../include
