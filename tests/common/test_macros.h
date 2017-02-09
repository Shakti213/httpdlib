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
#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <functional>
#include <iostream>
#include <string>

struct result_t
{
    result_t() : success_count(0), fail_count(0) {
    }
    int success_count;
    int fail_count;
};

#define BEGIN_TEST_SEQUENCE() std::function<result_t()> tst_sequence[] =
#define END_TEST_SEQUENCE()

#define RUN_TESTS()                                                            \
    result_t tst_results;                                                      \
    for (auto f : tst_sequence) {                                              \
        auto r = f();                                                          \
        tst_results.success_count += r.success_count;                          \
        tst_results.fail_count += r.fail_count;                                \
    }                                                                          \
    auto total_test_count =                                                    \
        (tst_results.success_count + tst_results.fail_count);                  \
    std::cout << "COMPLETE RESULT: " << tst_results.success_count << " of "    \
              << total_test_count << " passed, coverage = "                    \
              << 100.0 * static_cast<double>(tst_results.success_count) /      \
                     total_test_count                                          \
              << "%" << std::endl;

#define DECLARE_TEST(x) result_t x()

#define CREATE_TEST(x)                                                         \
    result_t x() {                                                             \
        result_t tst_result;                                                   \
        auto tst_tag_name = std::string(#x) + "()";                            \
        std::cout << "Running " << tst_tag_name << "..." << std::endl;

#define TEST_SUCCESS() tst_result.success_count++;
#define TEST_FAIL() tst_result.fail_count++;

#define TEST_TRUE(x)                                                           \
    if (!(x)) {                                                                \
        std::cerr << "\t" << tst_tag_name << #x << " FAILURE" << std::endl;    \
        TEST_FAIL();                                                           \
    }                                                                          \
    else {                                                                     \
        TEST_SUCCESS();                                                        \
    }

#define MAKE_TESTVAR(x)                                                        \
    std::cout << "\tTESTING WITH " << #x << std::endl;                         \
    auto x

#define END_TEST()                                                             \
    std::cout << "Ending " << tst_tag_name << std::endl;                       \
    std::cout << "Successes: " << tst_result.success_count                     \
              << " Failures: " << tst_result.fail_count << "\r\n"              \
              << std::endl;                                                    \
    return tst_result;                                                         \
    }

#endif // TEST_MACROS_H
