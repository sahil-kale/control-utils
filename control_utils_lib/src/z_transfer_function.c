#include "z_transfer_function.h"

#include <stddef.h>

#include "control_utils_util.h"

control_utils_z_tf_init_E control_utils_z_tf_init(control_utils_z_tf_t *tf) {
    control_utils_z_tf_init_E ret = Z_TF_INIT_OK;

    if (tf == NULL) {
        ret = Z_TF_INIT_ERROR_INVALID_POINTER;
    }

    if ((ret == Z_TF_INIT_OK) && (IS_APPROX_ZERO(tf->denominator[0]))) {
        ret = Z_TF_INIT_ERROR_TF_NOT_PROPER;
    }

    if (ret == Z_TF_INIT_OK) {
        tf->initialized = true;
        for (size_t i = 0; (i < tf->order) && (ret == Z_TF_INIT_OK); i++) {
            tf->numerator_buffer[i] = 0;
            tf->denominator_buffer[i] = 0;
        }
    }

    return ret;
}

control_utils_z_tf_step_E control_utils_z_tf_step(control_utils_z_tf_t *tf, float input, float *output) {
    control_utils_z_tf_step_E ret = Z_TF_STEP_OK;

    if ((tf == NULL) || (output == NULL)) {
        ret = Z_TF_STEP_ERROR_INVALID_POINTER;
    }

    if ((ret == Z_TF_STEP_OK) && (tf->initialized == false)) {
        ret = Z_TF_STEP_ERROR_NOT_INITIALIZED;
    }

    if (ret == Z_TF_STEP_OK) {
        const size_t num_elements = tf->order + 1;
        // Move all the values in the buffer down one
        // TODO: This could be optimized by using a circular buffer. But it is more readable this way for now until that is there.

        for (size_t i = num_elements - 1; i > 0; i--) {
            tf->numerator_buffer[i] = tf->numerator_buffer[i - 1];
            tf->denominator_buffer[i] = tf->denominator_buffer[i - 1];
        }

        tf->numerator_buffer[0] = input;
        tf->denominator_buffer[0] = 0.0F;

        float convolution_sum = 0.0F;

        for (size_t i = 0; i < num_elements; i++) {
            convolution_sum += tf->numerator[i] * tf->numerator_buffer[i];
            if (i > 0) {
                convolution_sum -= tf->denominator[i] * tf->denominator_buffer[i];
            }
        }

        tf->denominator_buffer[0] = convolution_sum / tf->denominator[0];

        *output = tf->denominator_buffer[0];
    }

    return ret;
}