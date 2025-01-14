#include "CppUTest/TestHarness.h"

extern "C" {
#include "z_transfer_function.h"
}

TEST_GROUP(z_transfer_function_test){void setup(){} void teardown(){}};

TEST(z_transfer_function_test, test_init) {
    static float numerator[] = {0, 1, 2, 4};
    static float denominator[] = {1, 0, 3, 5};

    CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(tf, numerator, denominator);
    CHECK_EQUAL(3, tf.order);

    // make the numerator and denominator buffers all 243.0
    for (size_t i = 0; i < tf.order; i++) {
        tf.numerator_buffer[i] = 243.0;
        tf.denominator_buffer[i] = 243.0;
    }

    control_utils_z_tf_init_E result = control_utils_z_tf_init(&tf);

    CHECK_EQUAL(Z_TF_INIT_OK, result);
    // check that the numerator and denominator buffers are initialized to 0
    for (size_t i = 0; i < tf.order; i++) {
        CHECK_EQUAL(0, tf.numerator_buffer[i]);
        CHECK_EQUAL(0, tf.denominator_buffer[i]);
    }

    CHECK_EQUAL(tf.initialized, true);
}

TEST(z_transfer_function_test, test_invalid_denominator) {
    static float numerator[] = {0, 1, 2, 4};
    static float denominator[] = {0, 0, 3, 5};

    CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(tf, numerator, denominator);

    CHECK_EQUAL(control_utils_z_tf_init(&tf), Z_TF_INIT_ERROR_TF_NOT_PROPER);
}

TEST(z_transfer_function_test, test_null_pointer) {
    CHECK_EQUAL(Z_TF_INIT_ERROR_INVALID_POINTER, control_utils_z_tf_init(NULL));
    CHECK_EQUAL(Z_TF_STEP_ERROR_INVALID_POINTER, control_utils_z_tf_step(NULL, 0, NULL));
}

TEST(z_transfer_function_test, test_convolution) {
    static float numerator[] = {0, 2, 3, 4};
    static float denominator[] = {1, 2, 3, 4};

    CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(tf, numerator, denominator);

    control_utils_z_tf_init(&tf);

    float output = 0;

    float check_input_buffer[5] = {1, 1, 1, 1, 1};
    float check_output_buffer[5] = {0, 2, 1, 1, -4};

    for (size_t i = 0; i < 5; i++) {
        CHECK_EQUAL(control_utils_z_tf_step(&tf, check_input_buffer[i], &output), Z_TF_STEP_OK);
        CHECK_EQUAL(check_output_buffer[i], output);
    }
}

TEST(z_transfer_function_test, test_not_initialized_step) {
    static float numerator[] = {0, 2, 3, 4};
    static float denominator[] = {1, 2, 3, 4};

    CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(tf, numerator, denominator);

    float output = 0;

    CHECK_EQUAL(Z_TF_STEP_ERROR_NOT_INITIALIZED, control_utils_z_tf_step(&tf, 0, &output));
}