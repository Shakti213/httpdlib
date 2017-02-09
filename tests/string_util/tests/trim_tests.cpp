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
#include <cctype>
#include <iostream>

CREATE_TEST(tst_string_util_trim_start) {

    using httpdlib::string_util::trim_start;

    std::string expect_unmodified[] = {"",
                                       "abc",
                                       "abc ",
                                       "abc\r",
                                       "abc\n",
                                       "abc \r\n",
                                       "ab cd",
                                       "ab\rcd",
                                       "ab\ncd",
                                       "ab \r\ncd",
                                       "ab cd ",
                                       "ab\rcd \r",
                                       "ab\ncd \r\n",
                                       "ab \r\ncd  \r\n\r\t"};

    std::string beginning_expected_result = "abc";
    std::string sp_beginning = " " + beginning_expected_result;
    std::string tab_beginning = "\t" + beginning_expected_result;
    std::string cr_beginning = "\r" + beginning_expected_result;
    std::string lf_beginning = "\n" + beginning_expected_result;
    std::string sp_cr_lf_beginning = " \r\n" + beginning_expected_result;

    int i = 0;
    for (auto s : expect_unmodified) {
        if (trim_start(s) != s) {
            std::cerr << "expected_unmodified[" << i << "] "
                      << " FAILURE" << std::endl;
            TEST_FAIL();
        }
        else {
            TEST_SUCCESS();
        }
    }

    MAKE_TESTVAR(sp_beginning_ok = trim_start(sp_beginning));
    TEST_TRUE(sp_beginning_ok == beginning_expected_result);

    MAKE_TESTVAR(cr_beginning_ok = trim_start(cr_beginning));
    TEST_TRUE(cr_beginning_ok == beginning_expected_result);

    MAKE_TESTVAR(lf_beginning_ok = trim_start(lf_beginning));
    TEST_TRUE(lf_beginning_ok == beginning_expected_result);
    MAKE_TESTVAR(sp_cr_lf_beginning_ok = trim_start(sp_cr_lf_beginning));
    TEST_TRUE(sp_cr_lf_beginning_ok == beginning_expected_result);
    MAKE_TESTVAR(tab_beginning_ok = trim_start(tab_beginning));
    TEST_TRUE(tab_beginning_ok == beginning_expected_result);

    std::string to_shuffle = " abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/"
                             "()=?;,:._-<>|'*^~¤   \r\n\t";
    bool punctuation_ok = trim_start(to_shuffle) ==
                          "abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/"
                          "()=?;,:._-<>|'*^~¤   \r\n\t";
    TEST_TRUE(punctuation_ok);

    bool fuzzy_testing_ok = true;
    // Do some fuzzy testing
    for (int i = 0; i < 200000; i++) {
        std::random_shuffle(to_shuffle.begin(), to_shuffle.end());
        auto trimmed = trim_start(to_shuffle);
        if (std::isspace((static_cast<int>(to_shuffle[0])) & 0x00ff) == false) {
            if (trimmed != to_shuffle) {
                std::cerr << "Fuzzy testing failed with string \"" << to_shuffle
                          << "\"" << std::endl;
                fuzzy_testing_ok = false;
            }
        }
    }
    TEST_TRUE(fuzzy_testing_ok);

    END_TEST();
}

CREATE_TEST(tst_string_util_trim_end) {

    using httpdlib::string_util::trim_end;

    std::string expect_unmodified[] = {"",
                                       "abc",
                                       " abc",
                                       "\rabc",
                                       "\nabc",
                                       " \r\nabc",
                                       "ab cd",
                                       "ab\rcd",
                                       "ab\ncd",
                                       "ab \r\ncd",
                                       " ab cd",
                                       "\r ab\rcd",
                                       " \r\nab\ncd",
                                       "  \t\r\n  ab \r\ncd"};

    std::string end_expected_result = "abc";
    std::string sp_end = end_expected_result + " ";
    std::string tab_end = end_expected_result + "\t";
    std::string cr_end = end_expected_result + "\r";
    std::string lf_end = end_expected_result + "\n";
    std::string sp_cr_lf_end = end_expected_result + " \r\n";

    int i = 0;
    for (auto s : expect_unmodified) {
        if (trim_end(s) != s) {
            std::cerr << "expected_unmodified[" << i << "] "
                      << " FAILURE" << std::endl;

            TEST_FAIL();
        }
        else {
            TEST_SUCCESS();
        }

        i++;
    }

    MAKE_TESTVAR(sp_end_ok = trim_end(sp_end));
    TEST_TRUE(sp_end_ok == end_expected_result);
    MAKE_TESTVAR(cr_end_ok = trim_end(cr_end));
    TEST_TRUE(cr_end_ok == end_expected_result);
    MAKE_TESTVAR(lf_end_ok = trim_end(lf_end));
    TEST_TRUE(lf_end_ok == end_expected_result);
    MAKE_TESTVAR(sp_cr_lf_end_ok = trim_end(sp_cr_lf_end));
    TEST_TRUE(sp_cr_lf_end_ok == end_expected_result);
    MAKE_TESTVAR(tab_end_ok = trim_end(tab_end));
    TEST_TRUE(tab_end_ok == end_expected_result);

    std::string to_shuffle = " abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/"
                             "()=?;,:._-<>|'*^~   \r\n\t";
    bool punctuation_ok =
        trim_end(to_shuffle) ==
        " abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?;,:._-<>|'*^~";
    TEST_TRUE(punctuation_ok);

    bool fuzzy_testing_ok = true;
    // Do some fuzzy testing
    for (int i = 0; i < 200000; i++) {
        std::random_shuffle(to_shuffle.begin(), to_shuffle.end());
        auto trimmed = trim_end(to_shuffle);
        if (std::isspace(
                (static_cast<int>(to_shuffle[to_shuffle.length() - 1])) &
                0x00ff) == false) {
            if (trimmed != to_shuffle) {
                std::cout << "Fuzzy testing failed with string \"" << to_shuffle
                          << "\"" << std::endl;
                fuzzy_testing_ok = false;
            }
        }
    }
    TEST_TRUE(fuzzy_testing_ok);

    END_TEST();
}

CREATE_TEST(tst_string_util_trim_both) {

    using httpdlib::string_util::trim_both;

    std::string expect_unmodified[] = {"",        "abc",      "a bc", "a\rbc",
                                       "a\nb}tc", "a \r\nbc", "abcd", "ab\rcd",
                                       "ab\ncd",  "ab \r\ncd"};

    std::string end_expected_result = "abc";
    std::string sp = " " + end_expected_result + " ";
    std::string tab = "\t" + end_expected_result + "\t";
    std::string cr = "\r" + end_expected_result + "\r";
    std::string lf = "\n" + end_expected_result + "\n";
    std::string sp_cr_lf = "\r\n" + end_expected_result + " \r\n";

    int i = 0;
    for (auto s : expect_unmodified) {
        if (trim_both(s) != s) {
            std::cerr << "expected_unmodified[" << i << "] "
                      << " FAILURE" << std::endl;
            TEST_FAIL();
        }
        else {
            TEST_SUCCESS();
        }

        i++;
    }

    MAKE_TESTVAR(sp_ok = trim_both(sp));
    TEST_TRUE(sp_ok == end_expected_result);
    MAKE_TESTVAR(cr_ok = trim_both(cr));
    TEST_TRUE(cr_ok == end_expected_result);
    MAKE_TESTVAR(lf_ok = trim_both(lf));
    TEST_TRUE(lf_ok == end_expected_result);
    MAKE_TESTVAR(sp_cr_lf_ok = trim_both(sp_cr_lf));
    TEST_TRUE(sp_cr_lf_ok == end_expected_result);
    MAKE_TESTVAR(tab_ok = trim_both(tab));
    TEST_TRUE(tab_ok == end_expected_result);

    std::string to_shuffle = " abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/"
                             "()=?;,:._-<>|'*^~   \r\n\t";
    bool punctuation_ok =
        trim_both(to_shuffle) ==
        "abcdefghijklmnopqrstuvwxyz1234567890+!\"#%&/()=?;,:._-<>|'*^~";
    TEST_TRUE(punctuation_ok);

    bool fuzzy_testing_ok = true;
    // Do some fuzzy testing
    for (int i = 0; i < 200000; i++) {
        std::random_shuffle(to_shuffle.begin(), to_shuffle.end());

        auto trimmed = trim_both(to_shuffle);

        int end_as_int = to_shuffle[to_shuffle.length() - 1];
        int start_as_int = to_shuffle[0];
        bool end_is_space = std::isspace(end_as_int & 0x00ff) != 0;
        bool beg_is_space = std::isspace(start_as_int & 0x00ff) != 0;

        if (end_is_space == false && beg_is_space == false) {
            if (trimmed != to_shuffle) {
                std::cerr << "Fuzzy testing failed with string \"" << to_shuffle
                          << "\"" << std::endl;
                fuzzy_testing_ok = false;
            }
        }
    }
    TEST_TRUE(fuzzy_testing_ok);

    END_TEST();
}
