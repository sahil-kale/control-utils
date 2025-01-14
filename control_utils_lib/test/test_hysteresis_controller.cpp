#include "CppUTest/TestHarness.h"

extern "C" {
#include "hysteresis_controller.h"
}

TEST_GROUP(hysteresis_controller_test){void setup(){} void teardown(){}};

TEST(hysteresis_controller_test, test_init) {
    control_utils_hysteresis_controller_data_t hysteresis_controller_data;
    control_utils_hysteresis_controller_config_t hysteresis_controller_config = {10, 5};

    CHECK_EQUAL(true, control_utils_hysteresis_controller_init(&hysteresis_controller_data, &hysteresis_controller_config));
    CHECK_EQUAL(false, hysteresis_controller_data.state);

    CHECK_EQUAL(false, control_utils_hysteresis_controller_init(NULL, &hysteresis_controller_config));
    CHECK_EQUAL(false, control_utils_hysteresis_controller_init(&hysteresis_controller_data, NULL));
}

TEST(hysteresis_controller_test, false_to_true) {
    control_utils_hysteresis_controller_data_t hysteresis_controller_data;
    control_utils_hysteresis_controller_config_t hysteresis_controller_config = {10, 5};

    control_utils_hysteresis_controller_init(&hysteresis_controller_data, &hysteresis_controller_config);

    CHECK_EQUAL(true, control_utils_hysteresis_controller_run(&hysteresis_controller_data, 9.9F));
    CHECK_EQUAL(false, hysteresis_controller_data.state);

    CHECK_EQUAL(true, control_utils_hysteresis_controller_run(&hysteresis_controller_data, 10.0F));
    CHECK_EQUAL(true, hysteresis_controller_data.state);
}

TEST(hysteresis_controller_test, true_to_false) {
    control_utils_hysteresis_controller_data_t hysteresis_controller_data;
    control_utils_hysteresis_controller_config_t hysteresis_controller_config = {10, 5};

    control_utils_hysteresis_controller_init(&hysteresis_controller_data, &hysteresis_controller_config);

    hysteresis_controller_data.state = true;

    CHECK_EQUAL(true, control_utils_hysteresis_controller_run(&hysteresis_controller_data, 5.1F));
    CHECK_EQUAL(true, hysteresis_controller_data.state);

    CHECK_EQUAL(true, control_utils_hysteresis_controller_run(&hysteresis_controller_data, 5.0F));
    CHECK_EQUAL(false, hysteresis_controller_data.state);
}