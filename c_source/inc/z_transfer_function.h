#ifndef Z_TRANSFER_FUNCTION_H
#define Z_TRANSFER_FUNCTION_H
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "util.h"

typedef enum { Z_TF_INIT_OK, Z_TF_INIT_ERROR_TF_NOT_PROPER, Z_TF_INIT_ERROR_INVALID_POINTER } Z_tf_init_E;
typedef enum { Z_TF_STEP_OK, Z_TF_STEP_ERROR_NOT_INITIALIZED, Z_TF_STEP_ERROR_INVALID_POINTER } Z_tf_step_E;

typedef struct {
    float *numerator;
    float *denominator;
    float *numerator_buffer;
    float *denominator_buffer;
    size_t order;
    bool initialized;
} Z_TF_T;

/**
 * @brief Declare a transfer function with numerator and denominator values.
 *
 * @param name The name of the transfer function.
 * @param tf_numerator The numerator coefficient values, in descending order.
 * @param tf_denominator The denominator values, in descending order.
 *
 * @warning The numerator and denominator arrays must have the same size. The transfer function must be proper
 * @note Ex: the transfer function (z^2+2z+4)/(z^3+3z+5) would be declared as: DECLARE_Z_TRANSFER_FUNCTION(tf, {0, 1, 2, 4}, {1,
 * 0, 3, 5}).
 */
#define DECLARE_Z_TRANSFER_FUNCTION(name, tf_numerator, tf_denominator)                           \
    STATIC_ASSERT(sizeof(tf_numerator) == sizeof(tf_denominator),                                 \
                  "Numerator and Denominator should have same order (proper transfer function)"); \
    static float numerator_buffer_##name[sizeof(tf_numerator) / sizeof(float)] = {0};             \
    static float denominator_buffer_##name[sizeof(tf_denominator) / sizeof(float)] = {0};         \
    static Z_TF_T name = {                                                                        \
        .numerator = tf_numerator,                                                                \
        .denominator = tf_denominator,                                                            \
        .numerator_buffer = numerator_buffer_##name,                                              \
        .denominator_buffer = denominator_buffer_##name,                                          \
        .order = (int)((sizeof(tf_numerator) / sizeof(float)) - 1),                               \
        .initialized = false,                                                                     \
    };

/**
 * @brief Initialize the transfer function.
 *
 * @param tf The transfer function to initialize.
 * @return Z_tf_init_E Z_TF_INIT_OK if the transfer function was initialized successfully
 */
Z_tf_init_E Z_tf_init(Z_TF_T *tf);

/**
 * @brief Step the transfer function with a new input. Should be called at consistent time step.
 *
 * @param tf The transfer function to update.
 * @param input The new input value.
 * @param output The output value after finishing the convolution.
 *
 * @return Z_tf_step_E Z_TF_STEP_OK if the transfer function was updated successfully
 */
Z_tf_step_E Z_tf_step(Z_TF_T *tf, float input, float *output);

#endif  // Z_TRANSFER_FUNCTION_H