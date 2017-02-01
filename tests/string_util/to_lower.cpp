#include "../../include/httpdlib/string_util.h"

#include "test_macros.h"

#include <algorithm>
#include <iostream>

bool tst_to_lower() {
    using httpdlib::string_util::to_lower;
    START_TEST("tst_to_lower()");

    MAKE_TESTVAR(lowers_only = to_lower("abcdefghijklmnopqrstuvwxyz"));
    TEST_TRUE(lowers_only == "abcdefghijklmnopqrstuvwxyz");

    MAKE_TESTVAR(uppers_only = to_lower("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    TEST_TRUE(uppers_only == "abcdefghijklmnopqrstuvwxyz");

    MAKE_TESTVAR(numbers_only = to_lower("0123456789"));
    TEST_TRUE(numbers_only == "0123456789");

    MAKE_TESTVAR(spaces_only = to_lower(" \t\r\n"));
    TEST_TRUE(spaces_only == " \t\r\n");

    MAKE_TESTVAR(specials_only =
                     to_lower("!\"#¤%&/()=?@£${[]}\\+`´^¨~'*-_.:,;<>|"));
    TEST_TRUE(specials_only == "!\"#¤%&/()=?@£${[]}\\+`´^¨~'*-_.:,;<>|");

    MAKE_TESTVAR(mixed = to_lower("Hello World!\r\n"));
    TEST_TRUE(mixed == "hello world!\r\n");

    END_TEST();
}
