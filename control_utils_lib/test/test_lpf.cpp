#include <math.h>

#include "CppUTest/TestHarness.h"

extern "C" {
#include "low_pass_filter.h"
}

TEST_GROUP(low_pass_filter_test){void setup(){} void teardown(){}};

TEST(low_pass_filter_test, test_init) {
    control_utils_lpf_t lpf;
    CHECK_EQUAL(true, control_utils_lpf_init(&lpf, 10, 0.01));
    const float w0_T = 2 * M_PI * 10 * 0.01;

    const float expected_alpha = w0_T / (w0_T + 1);
    DOUBLES_EQUAL(expected_alpha, lpf.alpha, 0.001);
    DOUBLES_EQUAL(0, lpf.output, 0.001);

    CHECK_EQUAL(false, control_utils_lpf_init(NULL, 10, 0.01));
}

TEST(low_pass_filter_test, test_run) {
    control_utils_lpf_t lpf;

    control_utils_lpf_init(&lpf, 10, 0.01);

    CHECK_EQUAL(true, control_utils_lpf_step(&lpf, 1));
    CHECK_EQUAL(false, control_utils_lpf_step(NULL, 1));
}