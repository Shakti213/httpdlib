#include "httpdlib/header_collection.h"
#include "test_macros.h"

CREATE_TEST(tst_header_collection_parse) {
    using httpdlib::header_collection;

    MAKE_TESTVAR(empty_before_parsing = header_collection());
    TEST_TRUE(empty_before_parsing.size() == 0);

    MAKE_TESTVAR(parse_single = header_collection());
    parse_single.parse("hello: test");
    TEST_TRUE(parse_single.size() == 1);
    TEST_TRUE(parse_single.contains("hello"));
    TEST_TRUE(parse_single.value("hello") == "test");

    MAKE_TESTVAR(parse_consecutive = header_collection());
    parse_consecutive.parse("hello: test");
    TEST_TRUE(parse_consecutive.size() == 1);
    TEST_TRUE(parse_consecutive.contains("hello"));
    TEST_TRUE(parse_consecutive.value("hello") == "test");
    parse_consecutive.parse("hello2: test2");
    TEST_TRUE(parse_consecutive.size() == 2);
    TEST_TRUE(parse_consecutive.contains("hello"));
    TEST_TRUE(parse_consecutive.value("hello") == "test");
    TEST_TRUE(parse_consecutive.contains("hello2"));
    TEST_TRUE(parse_consecutive.value("hello2") == "test2");

    MAKE_TESTVAR(parse_empty_value = header_collection());
    parse_empty_value.parse("hello:");
    parse_empty_value.parse("should_ignore_value_spaces:   ");
    parse_empty_value.parse("  should_ignore_key_spaces  :");
    parse_empty_value.parse("  should_ignore_key_value_spaces   :   ");
    parse_empty_value.parse("\tshould_ignore_key_tabs\t:");
    parse_empty_value.parse("should_ignore_value_tabs:\t");
    parse_empty_value.parse("\tshould_ignore_key_value_tabs\t:\t");
    TEST_TRUE(parse_empty_value.size() == 7);
    TEST_TRUE(parse_empty_value.contains("hello") == true);
    TEST_TRUE(parse_empty_value.value("hello") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_value_spaces") == true);
    TEST_TRUE(parse_empty_value.value("should_ignore_value_spaces") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_key_spaces") == true);
    TEST_TRUE(parse_empty_value.value("should_ignore_key_spaces") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_key_value_spaces") ==
              true);
    TEST_TRUE(parse_empty_value.value("should_ignore_key_value_spaces") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_key_tabs") == true);
    TEST_TRUE(parse_empty_value.value("should_ignore_key_tabs") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_value_tabs") == true);
    TEST_TRUE(parse_empty_value.value("should_ignore_value_tabs") == "");
    TEST_TRUE(parse_empty_value.contains("should_ignore_key_value_tabs") ==
              true);
    TEST_TRUE(parse_empty_value.value("should_ignore_key_value_tabs") == "");

    MAKE_TESTVAR(parse_multiline = header_collection());
    parse_multiline.parse("hello: test\r\nhello2: test2");
    TEST_TRUE(parse_multiline.size() == 2);
    TEST_TRUE(parse_multiline.contains("hello"));
    TEST_TRUE(parse_multiline.value("hello") == "test");
    TEST_TRUE(parse_multiline.contains("hello2"));
    TEST_TRUE(parse_multiline.value("hello2") == "test2");

    MAKE_TESTVAR(parse_value_with_space = header_collection());
    parse_value_with_space.parse("hello: hello world");
    TEST_TRUE(parse_value_with_space.size() == 1);
    TEST_TRUE(parse_value_with_space.contains("hello"));
    TEST_TRUE(parse_value_with_space.value("hello") == "hello world");

    END_TEST();
}
