TEMPLATE = subdirs

SUBDIRS = \
    tests  \
    examples


HEADERS += \
           include/httpdlib/filesystem_response_generator.h \
           include/httpdlib/header_collection.h \
           include/httpdlib/memory_response.h \
           include/httpdlib/pointer_response.h \
           include/httpdlib/request.h \
           include/httpdlib/response_generator_collection.h \
           include/httpdlib/stream_response.h \
           include/httpdlib/codes/codes.h \
           include/httpdlib/interface/response.h \
           include/httpdlib/interface/response_generator.h \
           include/httpdlib/string_util/string_util.h \
           include/httpdlib/util/content_type.h \
           src/httpdlib/util/platform/content_type.h
SOURCES +=  \
           src/httpdlib/filesystem_response_generator.cpp \
           src/httpdlib/header_collection.cpp \
           src/httpdlib/memory_response.cpp \
           src/httpdlib/pointer_response.cpp \
           src/httpdlib/request.cpp \
           src/httpdlib/response_generator_collection.cpp \
           src/httpdlib/stream_response.cpp \
           src/httpdlib/interface/response.cpp \
           src/httpdlib/interface/response_generator.cpp \
           src/httpdlib/string_util/string_util.cpp \
           src/httpdlib/util/content_type.cpp \
           src/httpdlib/util/platform/win32_content_type.cpp
