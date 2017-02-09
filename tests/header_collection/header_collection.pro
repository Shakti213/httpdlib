TEMPLATE = app
TARGET = tests_header_collection
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../include
INCLUDEPATH += ../common

SOURCES += main.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    ../../src/httpdlib/header_collection.cpp \
    tests/tst_header_collection.cpp
