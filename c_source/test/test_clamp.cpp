#include "CppUTest/TestHarness.h"

extern "C" {
#include "clamp.h"
}

TEST_GROUP(clamp_test){void setup(){} void teardown(){}};

TEST(clamp_test, clamp_test) {
    float value = 10.0;
    float min = 0.0;
    float max = 20.0;

    DOUBLES_EQUAL(10.0, control_util_clamp(value, min, max), 0.01);

    value = -10.0;
    DOUBLES_EQUAL(0.0, control_util_clamp(value, min, max), 0.01);

    value = 30.0;
    DOUBLES_EQUAL(20.0, control_util_clamp(value, min, max), 0.01);
}
