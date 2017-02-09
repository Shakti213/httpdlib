TEMPLATE = app
TARGET = tests_header_collection
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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

INCLUDEPATH += ../../include
INCLUDEPATH += ../common

SOURCES += main.cpp \
    ../../src/httpdlib/string_util/string_util.cpp \
    ../../src/httpdlib/header_collection.cpp \
    tests/tst_header_collection.cpp
