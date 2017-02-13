#include "test_macros.h"
#include <iostream>

DECLARE_TEST(tst_header_collection_parse);
DECLARE_TEST(tst_header_collection_add_remove);

int main() {
    BEGIN_TEST_SEQUENCE(){tst_header_collection_parse,
                          tst_header_collection_add_remove};

    RUN_TESTS();
}
