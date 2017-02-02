#include "httpdlib/string_util/string_util.h"

#include "test_macros.h"

#include <algorithm>
#include <iostream>

bool tst_url_encode() {
    using httpdlib::string_util::url_encode;
    START_TEST("tst_url_encode()");

    MAKE_TESTVAR(must_not_be_modified_upper =
                     url_encode("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    TEST_TRUE(must_not_be_modified_upper == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    MAKE_TESTVAR(must_not_be_modified_lower =
                     url_encode("abcdefghijklmnopqrstuvwxyz"));
    TEST_TRUE(must_not_be_modified_lower == "abcdefghijklmnopqrstuvwxyz");

    MAKE_TESTVAR(must_not_be_modified_numbers = url_encode("0123456789"));
    TEST_TRUE(must_not_be_modified_numbers == "0123456789");

    MAKE_TESTVAR(must_not_be_modified_special = url_encode("-_.~"));
    TEST_TRUE(must_not_be_modified_special == "-_.~");

    MAKE_TESTVAR(must_be_encoded_reserved_chars =
                     url_encode("!#$&'()*+,/:;=?@[]"));
    TEST_TRUE(must_be_encoded_reserved_chars ==
              "%21%23%24%26%27%28%29%2a%2b%2c%2f%3a%3b%3d%3f%40%5b%5d");

    MAKE_TESTVAR(must_be_encoded_arbitrary = url_encode(R"|("%<>\^`{|})|"));
    TEST_TRUE(must_be_encoded_arbitrary == "%22%25%3c%3e%5c%5e%60%7b%7c%7d");

    MAKE_TESTVAR(must_be_encoded_spaces = url_encode(" \t\r\n"));
    TEST_TRUE(must_be_encoded_spaces == "%20%09%0d%0a");

    MAKE_TESTVAR(must_be_encoded_mixed = url_encode("\tHello World!"));
    TEST_TRUE(must_be_encoded_mixed == "%09Hello%20World%21");

    END_TEST();
}

bool tst_url_decode() {

    using httpdlib::string_util::url_decode;

    START_TEST("tst_url_decode()");

    bool decode_ok;

    MAKE_TESTVAR(must_not_be_modified_upper =
                     url_decode("ABCDEFGHIJKLMNOPQRSTUVWXYZ", decode_ok));
    TEST_TRUE(decode_ok &&
              must_not_be_modified_upper == "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

    MAKE_TESTVAR(must_not_be_modified_lower =
                     url_decode("abcdefghijklmnopqrstuvwxyz", decode_ok));
    TEST_TRUE(decode_ok &&
              must_not_be_modified_lower == "abcdefghijklmnopqrstuvwxyz");

    MAKE_TESTVAR(must_not_be_modified_numbers =
                     url_decode("0123456789", decode_ok));
    TEST_TRUE(decode_ok && must_not_be_modified_numbers == "0123456789");

    MAKE_TESTVAR(must_not_be_modified_special = url_decode("-_.~", decode_ok));
    TEST_TRUE(decode_ok && must_not_be_modified_special == "-_.~");

    MAKE_TESTVAR(must_not_be_modified_reserved_chars =
                     url_decode("!#$&'()*+,/:;=?@[]", decode_ok));
    TEST_TRUE(decode_ok &&
              must_not_be_modified_reserved_chars == "!#$&'()*+,/:;=?@[]");

    MAKE_TESTVAR(must_fail = url_decode("%hello", decode_ok));
    TEST_TRUE(!decode_ok);

    MAKE_TESTVAR(bad_encoding = url_decode("hello%a", decode_ok));
    TEST_TRUE(!decode_ok);

    MAKE_TESTVAR(must_be_decoded_reserved_chars = url_decode(
                     "%21%23%24%26%27%28%29%2a%2b%2c%2f%3a%3b%3d%3f%40%5b%5d",
                     decode_ok));
    TEST_TRUE(decode_ok &&
              must_be_decoded_reserved_chars == "!#$&'()*+,/:;=?@[]");

    MAKE_TESTVAR(must_be_decoded_arbitrary =
                     url_decode("%22%25%3c%3e%5c%5e%60%7b%7c%7d", decode_ok));
    TEST_TRUE(decode_ok && must_be_decoded_arbitrary == R"|("%<>\^`{|})|");

    MAKE_TESTVAR(must_be_decoded_spaces =
                     url_decode("%20%09%0d%0a", decode_ok));
    TEST_TRUE(decode_ok && must_be_decoded_spaces == " \t\r\n");

    MAKE_TESTVAR(must_be_decoded_mixed =
                     url_decode("%09Hello%20World%21", decode_ok));
    TEST_TRUE(decode_ok && must_be_decoded_mixed == "\tHello World!");

    END_TEST();
}
