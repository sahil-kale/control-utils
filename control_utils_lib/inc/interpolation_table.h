#ifndef INTERPOLATION_TABLE_H
#define INTERPOLATION_TABLE_H

#include <stdbool.h>
#include <stddef.h>

#include "control_utils_util.h"

typedef struct {
    float *x;     /// X values - MUST BE ASCENDING (lookup value key)
    float *y;     /// Y values (lookup value pair)
    size_t size;  /// Size of the table

    float *slopeBuffer;  /// Buffer to store the slope values. slopeBuffer[i] is the slope on the interval between [x[i], x[i+1])
    size_t slopeBufferSize;  /// Size of the slope buffer, 0U if not used

    bool initialized;  /// Flag to indicate if the table has been initialized
} control_utils_interpolation_table_t;

// Macro to declare an interpolation table with a slope buffer - this avoids the need to calculate the slope when interpolating
#define CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE_WITH_SLOPE_BUFFER(name, ascending_x_values, y_values) \
    CONTROL_UTILS_STATIC_ASSERT(sizeof(ascending_x_values) == sizeof(y_values),                         \
                                "X and Y value arrays should have the same size");                      \
    static float slopeBuffer_##name[(sizeof(ascending_x_values) / sizeof(float) - 1)] = {0};            \
    static control_utils_interpolation_table_t name = {                                                 \
        .x = ascending_x_values,                                                                        \
        .y = y_values,                                                                                  \
        .size = sizeof(ascending_x_values) / sizeof(float),                                             \
        .slopeBuffer = slopeBuffer_##name,                                                              \
        .slopeBufferSize = sizeof(slopeBuffer_##name) / sizeof(float),                                  \
        .initialized = false,                                                                           \
    };

#define CONTROL_UTILS_DECLARE_INTERPOLATION_TABLE(name, ascending_x_values, y_values) \
    CONTROL_UTILS_STATIC_ASSERT(sizeof(ascending_x_values) == sizeof(y_values),       \
                                "X and Y value arrays should have the same size");    \
    static control_utils_interpolation_table_t name = {                               \
        .x = ascending_x_values,                                                      \
        .y = y_values,                                                                \
        .size = sizeof(ascending_x_values) / sizeof(float),                           \
        .slopeBuffer = NULL,                                                          \
        .slopeBufferSize = 0U,                                                        \
        .initialized = false,                                                         \
    };

/**
 * @brief Initialize the interpolation table
 *
 * @param table Pointer to the interpolation table
 *
 * @return true if the interpolation table was initialized successfully, false otherwise
 */
bool control_utils_interpolation_table_init(control_utils_interpolation_table_t *const table);

/**
 * @brief Interpolate a value from the interpolation table
 *
 * @param table Pointer to the interpolation table
 * @param x The x value to interpolate
 * @param y Pointer to the interpolated y value - will be set if the interpolation is successful
 */
bool control_utils_interpolation_table_interpolate(control_utils_interpolation_table_t const *const table, float x,
                                                   float *const y);

#endif  // INTERPOLATION_TABLE_H