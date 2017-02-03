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
