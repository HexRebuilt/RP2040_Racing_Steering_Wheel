// Minimal Unity.h mock for native testing
#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Global test result
static int _unity_test_failures = 0;

// Unity test macros
#define TEST_ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d: %s\n", __FILE__, __LINE__, #condition); \
            _unity_test_failures = 1; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d: Expected %d, got %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual)); \
            _unity_test_failures = 1; \
        } \
    } while(0)

#define TEST_ASSERT_FALSE(condition) TEST_ASSERT_TRUE(!(condition))

// Unity functions
void UNITY_BEGIN(void);
int UNITY_END(void);

// Test registration
void RUN_TEST(void (*test_func)(void));

#endif // UNITY_H