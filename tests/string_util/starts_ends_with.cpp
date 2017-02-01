#include "../../include/httpdlib/string_util.h"

#include "test_macros.h"

#include <iostream>

bool tst_starts_with() {
    using httpdlib::string_util::starts_with;
    START_TEST("tst_starts_with()");

    MAKE_TESTVAR(should_succeed = starts_with("abc defgh", "abc de"));
    TEST_TRUE(should_succeed == true);

    MAKE_TESTVAR(same_twice = starts_with("abc defgh", "abc defgh"));
    TEST_TRUE(same_twice == true);

    MAKE_TESTVAR(should_fail_too_short = starts_with("abc de", "abc def"));
    TEST_TRUE(should_fail_too_short == false);

    MAKE_TESTVAR(should_fail_no_match = starts_with("abc def", "bc def"));
    TEST_TRUE(should_fail_no_match == false);

    MAKE_TESTVAR(should_fail_no_match2 = starts_with("abc def", "abc df"));
    TEST_TRUE(should_fail_no_match2 == false);

    END_TEST();
}

bool tst_ends_with() {
    using httpdlib::string_util::ends_with;
    START_TEST("tst_ends_with()");

    MAKE_TESTVAR(should_succeed = ends_with("abc defgh", " defgh"));
    TEST_TRUE(should_succeed == true);

    MAKE_TESTVAR(same_twice = ends_with("abc def", "abc def"));
    TEST_TRUE(same_twice == true);

    MAKE_TESTVAR(should_fail_too_short = ends_with("abc de", "abc def"));
    TEST_TRUE(should_fail_too_short == false);

    MAKE_TESTVAR(should_fail_no_match = ends_with("abc def", "abc de"));
    TEST_TRUE(should_fail_no_match == false);

    MAKE_TESTVAR(should_fail_no_match2 = ends_with("abc def", "abc df"));
    TEST_TRUE(should_fail_no_match2 == false);

    END_TEST();
}
