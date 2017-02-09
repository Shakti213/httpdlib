#include "test_macros.h"
#include <iostream>

DECLARE_TEST(tst_header_collection_parse);

int main() {
    BEGIN_TEST_SEQUENCE(){tst_header_collection_parse};

    RUN_TESTS();
}
