#include "CppUTest/TestHarness.h"

extern "C" {
#include "pid.h"
}

TEST_GROUP(pid_test){void setup(){} void teardown(){}};

TEST(pid_test, check_init) {
    control_utils_pid_data_t pid_data;

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
    control_utils_pid_config_t pid_config = {20.0, 0.0, 0.0, 0.1};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_config, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL(200.0, pid_data.output, 0.01);
}

TEST(pid_test, check_run_ki) {
    control_utils_pid_config_t pid_config = {0.0, 20.0, 0.0, 0.1};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        ret = control_utils_pid_run(10.0, &pid_data, &pid_config, NULL);
        integral += 10.0 * 20.0 * 0.1;
        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(integral, pid_data.output, 0.01);
}

TEST(pid_test, check_run_ki_max_windup) {
    const float dt = 0.1;
    control_utils_pid_config_t pid_config = {0.0, 20.0, 0.0, dt};
    control_utils_pid_limits_t pid_limits = {100.0};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        const float err = 10.0;
        ret = control_utils_pid_run(err, &pid_data, &pid_config, &pid_limits);
        integral += err * dt * pid_config.ki;

        if (integral >= pid_limits.max_windup) {
            integral = pid_limits.max_windup;
        }

        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(pid_limits.max_windup, pid_data.output, 0.01);
}

TEST(pid_test, windup_disabled) {
    control_utils_pid_config_t pid_config = {0.0, 20.0, 0.0, 0.1};
    control_utils_pid_limits_t pid_limits = {0.0};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    float integral = 0.0;
    for (uint8_t i = 0; i < 10; i++) {
        ret = control_utils_pid_run(10.0, &pid_data, &pid_config, &pid_limits);
        integral += 10.0 * 20.0 * 0.1;
        CHECK_EQUAL(true, ret);
    }

    DOUBLES_EQUAL(integral, pid_data.output, 0.01);
}

TEST(pid_test, check_run_kd) {
    control_utils_pid_config_t pid_config = {0.0, 0.0, 20.0, 0.1};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_config, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL((10 / 0.1 * 20), pid_data.output, 0.01);

    ret = control_utils_pid_run(20.0, &pid_data, &pid_config, NULL);
    CHECK_EQUAL(true, ret);
    DOUBLES_EQUAL((10 / 0.1 * 20), pid_data.output, 0.01);
}

TEST(pid_test, invalid_dt) {
    control_utils_pid_config_t pid_config = {20.0, 0.0, 0.0, 0.0};

    control_utils_pid_data_t pid_data;
    bool ret = control_utils_pid_init(&pid_data);
    CHECK_EQUAL(true, ret);

    ret = control_utils_pid_run(10.0, &pid_data, &pid_config, NULL);
    CHECK_EQUAL(false, ret);
    DOUBLES_EQUAL(0.0, pid_data.output, 0.01);
}

TEST(pid_test, invalid_params) {
    // Test with NULL pid_data
    control_utils_pid_config_t pid_config = {20.0, 0.0, 0.0, 0.1};
    CHECK_EQUAL(false, control_utils_pid_run(10.0, NULL, &pid_config, NULL));

    // Test with uninitialized pid_data
    control_utils_pid_data_t pid_data;
    pid_data.initialized = false;
    CHECK_EQUAL(false, control_utils_pid_run(10.0, &pid_data, &pid_config, NULL));
}