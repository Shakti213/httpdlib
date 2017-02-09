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
#include "test_macros.h"
#include <iostream>

DECLARE_TEST(tst_string_util_trim_start);
DECLARE_TEST(tst_string_util_trim_end);
DECLARE_TEST(tst_string_util_trim_both);
DECLARE_TEST(tst_string_util_split_once);
DECLARE_TEST(tst_string_util_split_all);
DECLARE_TEST(tst_url_encode);
DECLARE_TEST(tst_url_decode);

DECLARE_TEST(tst_to_lower);
DECLARE_TEST(tst_starts_with);
DECLARE_TEST(tst_ends_with);

int main() {
    BEGIN_TEST_SEQUENCE(){tst_string_util_trim_start,
                          tst_string_util_trim_end,
                          tst_string_util_trim_both,
                          tst_string_util_split_once,
                          tst_string_util_split_all,
                          tst_url_encode,
                          tst_url_decode,
                          tst_to_lower,
                          tst_starts_with,
                          tst_ends_with};
    RUN_TESTS();
}
