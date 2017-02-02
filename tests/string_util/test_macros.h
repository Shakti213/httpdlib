#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#include <iostream>
#include <string>

#define TEST_TRUE(x)                                                           \
    if (!(x)) {                                                                \
        std::cerr << "\t" << tst_tag_name << #x << " FAILURE" << std::endl;    \
        return false;                                                          \
    }                                                                          \
    else {                                                                     \
    }

#define MAKE_TESTVAR(x)                                                        \
    std::cout << "\tTESTING WITH " << #x << std::endl;                         \
    auto x

#define START_TEST(x)                                                          \
    auto tst_tag_name = x;                                                     \
    std::cout << "Running " << x << "..." << std::endl;
#define END_TEST()                                                             \
    std::cout << "Ending " << tst_tag_name << "\r\n" << std::endl;             \
    return true;

#endif // TEST_MACROS_H