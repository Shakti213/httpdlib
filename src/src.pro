INCLUDEPATH += ../include

HEADERS += \
           ../include/httpdlib/filesystem_response_generator.h \
           ../include/httpdlib/header_collection.h \
           ../include/httpdlib/memory_response.h \
           ../include/httpdlib/pointer_response.h \
           ../include/httpdlib/request.h \
           ../include/httpdlib/response_generator_collection.h \
           ../include/httpdlib/stream_response.h \
           ../include/httpdlib/codes/codes.h \
           ../include/httpdlib/interface/response.h \
           ../include/httpdlib/interface/response_generator.h \
           ../include/httpdlib/string_util/string_util.h \
           ../include/httpdlib/util/content_type.h \
           httpdlib/util/platform/content_type.h
SOURCES +=  \
           httpdlib/filesystem_response_generator.cpp \
           httpdlib/header_collection.cpp \
           httpdlib/memory_response.cpp \
           httpdlib/pointer_response.cpp \
           httpdlib/request.cpp \
           httpdlib/response_generator_collection.cpp \
           httpdlib/stream_response.cpp \
           httpdlib/interface/response.cpp \
           httpdlib/interface/response_generator.cpp \
           httpdlib/string_util/string_util.cpp \
           httpdlib/util/content_type.cpp \
           httpdlib/util/platform/win32_content_type.cpp
