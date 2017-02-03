CONFIG += c++11

TARGET = tests_string_util
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += ../../include

TEMPLATE = app

run_target.target = run
unix:run_target.depends = run_test
win32:run_target.recurse_target = run_test
win32:run_target.CONFIG = recursive
win32:run_target.recurse = "Debug"

debug_run_target.target = run_test
unix:debug_run_target.commands = ./$(TARGET)
unix:debug_run_target.depends = $(TARGET)
win32:debug_run_target.commands = $(DESTDIR_TARGET)
win32:debug_run_target.depends = $(DESTDIR_TARGET)

QMAKE_EXTRA_TARGETS += run_target debug_run_target


SOURCES += \
    string_util/trim_tests.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    main.cpp \
    string_util/split_tests.cpp \
    string_util/url_encode_decode.cpp \
    string_util/to_lower.cpp \
    string_util/starts_ends_with.cpp

HEADERS += \
    ../../include/httpdlib/string_util/string_util.h \
    test_macros.h

INCLUDEPATH += ../include
