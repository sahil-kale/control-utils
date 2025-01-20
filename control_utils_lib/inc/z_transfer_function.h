#ifndef Z_TRANSFER_FUNCTION_H
#define Z_TRANSFER_FUNCTION_H
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "control_utils_util.h"

typedef enum { Z_TF_INIT_OK, Z_TF_INIT_ERROR_TF_NOT_PROPER, Z_TF_INIT_ERROR_INVALID_POINTER } control_utils_z_tf_init_E;
typedef enum { Z_TF_STEP_OK, Z_TF_STEP_ERROR_NOT_INITIALIZED, Z_TF_STEP_ERROR_INVALID_POINTER } control_utils_z_tf_step_E;

typedef struct {
    float *numerator;
    float *denominator;
    float *numerator_buffer;
    float *denominator_buffer;
    size_t order;
    bool initialized;
} control_utils_z_tf_t;

/**
 * @brief Declare a transfer function with numerator and denominator values.
 *
 * @param name The name of the transfer function.
 * @param tf_numerator The numerator coefficient values, in descending order.
 * @param tf_denominator The denominator values, in descending order.
 *
 * @warning The numerator and denominator arrays must have the same size. The transfer function must be proper
 * @note Ex: the transfer function (z^2+2z+4)/(z^3+3z+5) would be declared as: CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(tf, {0,
 * 1, 2, 4}, {1, 0, 3, 5}).
 */
#define CONTROL_UTILS_DECLARE_Z_TRANSFER_FUNCTION(name, tf_numerator, tf_denominator)                           \
    CONTROL_UTILS_STATIC_ASSERT(sizeof(tf_numerator) == sizeof(tf_denominator),                                 \
                                "Numerator and Denominator should have same order (proper transfer function)"); \
    static float numerator_buffer_##name[sizeof(tf_numerator) / sizeof(float)] = {0};                           \
    static float denominator_buffer_##name[sizeof(tf_denominator) / sizeof(float)] = {0};                       \
    static control_utils_z_tf_t name = {                                                                        \
        .numerator = tf_numerator,                                                                              \
        .denominator = tf_denominator,                                                                          \
        .numerator_buffer = numerator_buffer_##name,                                                            \
        .denominator_buffer = denominator_buffer_##name,                                                        \
        .order = (int)((sizeof(tf_numerator) / sizeof(float)) - 1),                                             \
        .initialized = false,                                                                                   \
    };

/**
 * @brief Initialize the transfer function.
 *
 * @param tf The transfer function to initialize.
 * @return control_utils_z_tf_init_E Z_TF_INIT_OK if the transfer function was initialized successfully
 */
control_utils_z_tf_init_E control_utils_z_tf_init(control_utils_z_tf_t *tf);

/**
 * @brief Step the transfer function with a new input. Should be called at consistent time step.
 *
 * @param tf The transfer function to update.
 * @param input The new input value.
 * @param output The output value after finishing the convolution.
 *
 * @return control_utils_z_tf_step_E Z_TF_STEP_OK if the transfer function was updated successfully
 */
control_utils_z_tf_step_E control_utils_z_tf_step(control_utils_z_tf_t *tf, float input, float *output);

#endif  // Z_TRANSFER_FUNCTION_H