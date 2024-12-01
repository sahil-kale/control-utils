#include "CppUTest/TestHarness.h"

extern "C" {
#include "pid.h"
}

TEST_GROUP(pid_test){void setup(){} void teardown(){}};

TEST(pid_test, check_init) {
    pid_data_t pid_data;

    pid_data.accumulator = 132132.0;
    pid_data.last_error = -160.0;
    pid_data.output = 120.0;
    CHECK_EQUAL(true, control_utils_pid_init(&pid_data));
    CHECK_EQUAL(true, pid_data.initialized);

    DOUBLES_EQUAL(0.0, pid_data.accumulator, 0.01);
    DOUBLES_EQUAL(0.0, pid_data.last_error, 0.01);
    DOUBLES_EQUAL(0.0, pid_data.output, 0.01);
}

TEST(pid_test, check_null) { CHECK_EQUAL(false, control_utils_pid_init(NULL)); }

TEST(pid_test, check_run_kp) {
    pid_gains_t pid_gains = {20.0, 0.0, 0.0, 0.1};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL(200.0, pid_data.output, 0.01);
}

TEST(pid_test, check_run_ki) {
    pid_gains_t pid_gains = {0.0, 20.0, 0.0, 0.1};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, NULL);
        integral += 10.0 * 20.0 * 0.1;
        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(integral, pid_data.output, 0.01);
}

TEST(pid_test, check_run_ki_max_windup) {
    pid_gains_t pid_gains = {0.0, 20.0, 0.0, 0.1};
    pid_limits_t pid_limits = {100.0};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, &pid_limits);
        integral += 10.0 * 20.0 * 0.1;
        if (integral > 10.0) {
            integral = 10.0;
        }
        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(integral * 20, pid_data.output, 0.01);
}

TEST(pid_test, windup_disabled) {
    pid_gains_t pid_gains = {0.0, 20.0, 0.0, 0.1};
    pid_limits_t pid_limits = {0.0};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, &pid_limits);
        integral += 10.0 * 20.0 * 0.1;
        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(integral, pid_data.output, 0.01);
}

TEST(pid_test, check_run_kd) {
    pid_gains_t pid_gains = {0.0, 0.0, 20.0, 0.1};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL((10 / 0.1 * 20), pid_data.output, 0.01);

    ret = control_utils_pid_run(20.0, &pid_data, &pid_gains, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL((10 / 0.1 * 20), pid_data.output, 0.01);
}

TEST(pid_test, invalid_dt) {
    pid_gains_t pid_gains = {20.0, 0.0, 0.0, 0.0};

    pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_gains, NULL);
    CHECK_EQUAL(false, ret);
    DOUBLES_EQUAL(0.0, pid_data.output, 0.01);
}

TEST(pid_test, invalid_params) {
    // Test with NULL pid_data
    pid_gains_t pid_gains = {20.0, 0.0, 0.0, 0.1};
    CHECK_EQUAL(false, control_utils_pid_run(10.0, NULL, &pid_gains, NULL));

    // Test with uninitialized pid_data
    pid_data_t pid_data;
    pid_data.initialized = false;
    CHECK_EQUAL(false, control_utils_pid_run(10.0, &pid_data, &pid_gains, NULL));
}