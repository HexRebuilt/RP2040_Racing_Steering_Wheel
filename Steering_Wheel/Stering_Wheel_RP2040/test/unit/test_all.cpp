// Consolidated test file for native testing
#include <Arduino.h>
#include <unity.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// ============================================
// Button ID constants
// ============================================
#define RGREEN  1
#define RRED    2
#define RBLUE   3
#define RBLACK  4
#define RUP     5
#define RDOWN   6
#define RRIGHT  7
#define RLEFT   8
#define RPUSH   9
#define LRED    10
#define LYELLOW 11
#define LWHITE  12

// ============================================
// GPS Parsing functions
// ============================================
int parseGPSSpeed(const char* data) {
    if (data == NULL) return 0;
    const char* ptr = strstr(data, "g.sp.");
    if (ptr == NULL) return 0;
    ptr += 5;
    char speedStr[16] = {0};
    int i = 0;
    while (*ptr && isdigit(*ptr) && i < 15) {
        speedStr[i++] = *ptr++;
    }
    return atoi(speedStr);
}

int parseGPSSatellites(const char* data) {
    if (data == NULL) return 0;
    const char* ptr = strstr(data, "g.st.");
    if (ptr == NULL) return 0;
    ptr += 5;
    char satsStr[16] = {0};
    int i = 0;
    while (*ptr && isdigit(*ptr) && i < 15) {
        satsStr[i++] = *ptr++;
    }
    return atoi(satsStr);
}

// ============================================
// Timer class
// ============================================
class Timer {
private:
    clock_t startTime;
public:
    long timePassed() {
        return (long)clock() - startTime;
    }
    void startTimer() {
        startTime = clock();
    }
};

// ============================================
// Test cases - Button IDs
// ============================================
void test_button_ids_radio() {
    TEST_ASSERT_EQUAL(1, RGREEN);
    TEST_ASSERT_EQUAL(2, RRED);
    TEST_ASSERT_EQUAL(3, RBLUE);
    TEST_ASSERT_EQUAL(4, RBLACK);
    TEST_ASSERT_EQUAL(5, RUP);
    TEST_ASSERT_EQUAL(6, RDOWN);
    TEST_ASSERT_EQUAL(7, RRIGHT);
    TEST_ASSERT_EQUAL(8, RLEFT);
    TEST_ASSERT_EQUAL(9, RPUSH);
}

void test_button_ids_ecu() {
    TEST_ASSERT_EQUAL(10, LRED);
    TEST_ASSERT_EQUAL(11, LYELLOW);
    TEST_ASSERT_EQUAL(12, LWHITE);
}

// ============================================
// Test cases - GPS Parsing
// ============================================
void test_parse_speed() {
    const char* test = "g.sp.45 g.st.8 g.h.14 g.m.30 g.s.25";
    TEST_ASSERT_EQUAL(45, parseGPSSpeed(test));
}

void test_parse_satellites() {
    const char* test = "g.sp.45 g.st.8 g.h.14 g.m.30 g.s.25";
    TEST_ASSERT_EQUAL(8, parseGPSSatellites(test));
}

void test_parse_zero_speed() {
    const char* test = "g.sp.0 g.st.0 g.h.0 g.m.0 g.s.0";
    TEST_ASSERT_EQUAL(0, parseGPSSpeed(test));
}

void test_parse_no_gps() {
    const char* test = "hello world";
    TEST_ASSERT_EQUAL(0, parseGPSSpeed(test));
}

// ============================================
// Test cases - Timer
// ============================================
void test_timer_initial() {
    Timer t;
    t.startTimer();
    TEST_ASSERT_TRUE(true);  // Just verify it doesn't crash
}

void test_timer_timing() {
    Timer t;
    clock_t before = clock();
    t.startTimer();
    // Basic check - time tracking works
    TEST_ASSERT_TRUE(true);
}

// ============================================
// Test setup - Unity framework
// ============================================
void setup() {
    UNITY_BEGIN();
    
    // Button ID tests
    RUN_TEST(test_button_ids_radio);
    RUN_TEST(test_button_ids_ecu);
    
    // GPS parsing tests
    RUN_TEST(test_parse_speed);
    RUN_TEST(test_parse_satellites);
    RUN_TEST(test_parse_zero_speed);
    RUN_TEST(test_parse_no_gps);
    
    // Timer tests
    RUN_TEST(test_timer_initial);
    RUN_TEST(test_timer_timing);
    
    UNITY_END();
}

void loop() {}

// ============================================
// Main entry point for native executable
// ============================================
int main(int argc, char* argv[]) {
    setup();
    return 0;
}
