/*
 * MIT License
 *
 * Copyright (c) 2017 Andreas Wass
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "httpdlib/string_util/string_util.h"

#include "test_macros.h"

#include <algorithm>
#include <iostream>

using httpdlib::string_util::split_once;
using httpdlib::string_util::split_all;

CREATE_TEST(tst_string_util_split_once) {
    MAKE_TESTVAR(empty_all = split_once("", '.'));
    TEST_TRUE(empty_all.first == "");
    TEST_TRUE(empty_all.second == "");

    MAKE_TESTVAR(empty_first = split_once(".split", '.'));
    TEST_TRUE(empty_first.first == "");
    TEST_TRUE(empty_first.second == "split");

    MAKE_TESTVAR(empty_last = split_once("abc.", '.'));
    TEST_TRUE(empty_last.first == "abc");
    TEST_TRUE(empty_last.second == "");

    MAKE_TESTVAR(trivial_split = split_once("abc.efg", '.'));
    TEST_TRUE(trivial_split.first == "abc");
    TEST_TRUE(trivial_split.second == "efg");

    MAKE_TESTVAR(double_initial_seperator = split_once("..split", '.'));
    TEST_TRUE(double_initial_seperator.first == "");
    TEST_TRUE(double_initial_seperator.second == ".split");

    MAKE_TESTVAR(semicolon_seperator = split_once("abc;defgh", ';'));
    TEST_TRUE(semicolon_seperator.first == "abc");
    TEST_TRUE(semicolon_seperator.second == "defgh");

    MAKE_TESTVAR(double_separator_middle = split_once("abc;;def", ';'));
    TEST_TRUE(double_separator_middle.first == "abc");
    TEST_TRUE(double_separator_middle.second == ";def");

    END_TEST();
}

CREATE_TEST(tst_string_util_split_all) {

    MAKE_TESTVAR(empty_all = split_all("", '.'));
    TEST_TRUE(empty_all.size() == 0);

    MAKE_TESTVAR(multiple_seperators_only = split_all("...", '.'));
    TEST_TRUE(multiple_seperators_only.size() == 4);
    TEST_TRUE(multiple_seperators_only[0] == "");
    TEST_TRUE(multiple_seperators_only[1] == "");
    TEST_TRUE(multiple_seperators_only[2] == "");
    TEST_TRUE(multiple_seperators_only[3] == "");

    MAKE_TESTVAR(multiple_seperators_only_no_empty =
                     split_all("...", '.', false));
    TEST_TRUE(multiple_seperators_only_no_empty.size() == 0);

    MAKE_TESTVAR(empty_first = split_all(".split", '.'));
    TEST_TRUE(empty_first.size() == 2);
    TEST_TRUE(empty_first[0] == "");
    TEST_TRUE(empty_first[1] == "split");

    MAKE_TESTVAR(empty_first_no_empty_result = split_all(".split", '.', false));
    TEST_TRUE(empty_first_no_empty_result.size() == 1);
    TEST_TRUE(empty_first_no_empty_result[0] == "split");

    MAKE_TESTVAR(empty_last = split_all("abc.", '.'));
    TEST_TRUE(empty_last.size() == 2);
    TEST_TRUE(empty_last[0] == "abc");
    TEST_TRUE(empty_last[1] == "");

    MAKE_TESTVAR(empty_last_no_empty = split_all("abc.", '.', false));
    TEST_TRUE(empty_last_no_empty.size() == 1);
    TEST_TRUE(empty_last_no_empty[0] == "abc");

    MAKE_TESTVAR(trivial_split = split_all("abc.efg", '.'));
    TEST_TRUE(trivial_split.size() == 2);
    TEST_TRUE(trivial_split[0] == "abc");
    TEST_TRUE(trivial_split[1] == "efg");

    MAKE_TESTVAR(semicolon_seperator = split_all("abc;defgh", ';'));
    TEST_TRUE(semicolon_seperator.size() == 2);
    TEST_TRUE(semicolon_seperator[0] == "abc");
    TEST_TRUE(semicolon_seperator[1] == "defgh");

    MAKE_TESTVAR(double_separator_middle = split_all("abc;;def", ';'));
    TEST_TRUE(double_separator_middle.size() == 3);
    TEST_TRUE(double_separator_middle[0] == "abc");
    TEST_TRUE(double_separator_middle[1] == "");
    TEST_TRUE(double_separator_middle[2] == "def");

    MAKE_TESTVAR(double_separator_middle_no_empty =
                     split_all("abc;;def", ';', false));
    TEST_TRUE(double_separator_middle_no_empty.size() == 2);
    TEST_TRUE(double_separator_middle_no_empty[0] == "abc");
    TEST_TRUE(double_separator_middle_no_empty[1] == "def");

    END_TEST();
}
