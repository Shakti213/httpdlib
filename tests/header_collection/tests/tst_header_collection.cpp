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
    TEST_TRUE(parse_single["hello"] == "test");

    MAKE_TESTVAR(parse_consecutive = header_collection());
    parse_consecutive.parse("hello: test");
    TEST_TRUE(parse_consecutive.size() == 1);
    TEST_TRUE(parse_consecutive.contains("hello"));
    TEST_TRUE(parse_consecutive.value("hello") == "test");
    TEST_TRUE(parse_consecutive["hello"] == "test");
    parse_consecutive.parse("hello2: test2");
    TEST_TRUE(parse_consecutive.size() == 2);
    TEST_TRUE(parse_consecutive.contains("hello"));
    TEST_TRUE(parse_consecutive.value("hello") == "test");
    TEST_TRUE(parse_consecutive["hello"] == "test");
    TEST_TRUE(parse_consecutive.contains("hello2"));
    TEST_TRUE(parse_consecutive.value("hello2") == "test2");
    TEST_TRUE(parse_consecutive["hello2"] == "test2");

    MAKE_TESTVAR(parse_empty_value = header_collection());
    parse_empty_value.parse("hello:");
    parse_empty_value.parse("hello_without_colon");
    parse_empty_value.parse("should_ignore_value_spaces:   ");
    parse_empty_value.parse("  should_ignore_key_spaces  :");
    parse_empty_value.parse("  should_ignore_key_value_spaces   :   ");
    parse_empty_value.parse("\tshould_ignore_key_tabs\t:");
    parse_empty_value.parse("should_ignore_value_tabs:\t");
    parse_empty_value.parse("\tshould_ignore_key_value_tabs\t:\t");
    TEST_TRUE(parse_empty_value.size() == 8);
    TEST_TRUE(parse_empty_value.contains("hello") == true);
    TEST_TRUE(parse_empty_value.value("hello") == "");
    TEST_TRUE(parse_empty_value.contains("hello_without_colon") == true);
    TEST_TRUE(parse_empty_value.value("hello_without_colon") == "");
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

    MAKE_TESTVAR(case_insensitive = header_collection());
    case_insensitive.parse("HELLO: WORLD");
    TEST_TRUE(case_insensitive.value("HELLO") == "WORLD");
    TEST_TRUE(case_insensitive.value("hello") ==
              case_insensitive.value("HELLO"));
    TEST_TRUE(case_insensitive["HELLO"] == "WORLD");
    TEST_TRUE(case_insensitive["HELLO"] == case_insensitive["hello"]);

    END_TEST();
}

CREATE_TEST(tst_header_collection_add_remove) {
    using httpdlib::header_collection;

    MAKE_TESTVAR(single_add = header_collection());
    single_add.add("hello", "world");
    TEST_TRUE(single_add.size() == 1);
    TEST_TRUE(single_add.value("hello") == "world");

    MAKE_TESTVAR(add_twice = header_collection());
    add_twice.add("hello", "world");
    TEST_TRUE(add_twice.size() == 1);
    TEST_TRUE(add_twice.value("hello") == "world");
    add_twice.add("hello", "universe");
    TEST_TRUE(add_twice.size() == 1);
    TEST_TRUE(add_twice.value("hello") == "universe");

    MAKE_TESTVAR(add_multiple = header_collection());
    add_multiple.add("hello", "world");
    add_multiple.add("second", "third");
    TEST_TRUE(add_multiple.size() == 2);
    TEST_TRUE(add_multiple.value("hello") == "world");
    TEST_TRUE(add_multiple.value("second") == "third");

    MAKE_TESTVAR(case_insensitive = header_collection());
    case_insensitive.add("HELLO", "WORLD");
    TEST_TRUE(case_insensitive.value("HELLO") == "WORLD");
    TEST_TRUE(case_insensitive.value("hello") ==
              case_insensitive.value("HELLO"));
    TEST_TRUE(case_insensitive["HELLO"] == "WORLD");
    TEST_TRUE(case_insensitive["HELLO"] == case_insensitive["hello"]);

    MAKE_TESTVAR(remove_empty = header_collection());
    remove_empty.remove("");
    TEST_TRUE(remove_empty.size() == 0);
    remove_empty.add("hello", "world");
    TEST_TRUE(remove_empty.size() == 1);
    remove_empty.remove("");
    TEST_TRUE(remove_empty.size() == 1);

    MAKE_TESTVAR(remove_single = header_collection());
    remove_single.add("hello", "world");
    TEST_TRUE(remove_single.size() == 1);
    remove_single.remove("hello");
    TEST_TRUE(remove_single.size() == 0);

    MAKE_TESTVAR(remove_case_insensitive = header_collection());
    remove_case_insensitive.add("hello", "world");
    TEST_TRUE(remove_case_insensitive.size() == 1);
    remove_single.remove("hello");
    TEST_TRUE(remove_single.size() == 0);
    remove_case_insensitive.add("HELLO", "world");
    TEST_TRUE(remove_case_insensitive.size() == 1);
    remove_single.remove("hello");
    TEST_TRUE(remove_single.size() == 0);
    remove_case_insensitive.add("hello", "world");
    TEST_TRUE(remove_case_insensitive.size() == 1);
    remove_single.remove("HELLO");
    TEST_TRUE(remove_single.size() == 0);

    END_TEST();
}
