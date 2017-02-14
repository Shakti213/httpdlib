#include "httpdlib/interface/data_parser.h"

namespace httpdlib
{

namespace interface
{

data_parser::~data_parser() {
}

int data_parser::error() const {
    return m_error_code;
}
}
}
