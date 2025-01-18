#include "interpolation_table.h"

#include <stddef.h>

bool control_utils_interpolation_table_init(control_utils_interpolation_table_t *const table) {
    const bool ret = table != NULL;
    if (ret) {
        for (size_t i = 0; i < table->slopeBufferSize; i++) {
            const float x_diff = table->x[i + 1] - table->x[i];
            const float y_diff = table->y[i + 1] - table->y[i];
            table->slopeBuffer[i] = y_diff / x_diff;
        }

        table->initialized = true;
    }

    return ret;
}

bool control_utils_interpolation_table_interpolate(control_utils_interpolation_table_t const *const table, float x,
                                                   float *const y) {
    bool ret = (table != NULL) && (y != NULL);
    if (ret) {
        ret &= table->initialized;
    }

    if (ret) {
        size_t idx = 0;
        float slope = 0;

        bool found = false;

        // Find the interval where x is located. Note the size - 1, as we are looking at the interval [x[i], x[i+1])
        for (size_t i = 0; i < table->size - 1; i++) {
            if ((x >= table->x[i]) && (x < table->x[i + 1])) {
                idx = i;
                // Only assign slope here, as we will use the fact that if slope is not calculated here it will be 0, thereby only
                // returning the y value

                if (table->slopeBufferSize > 0U) {
                    slope = table->slopeBuffer[i];
                } else {
                    const float x_diff = table->x[i + 1] - table->x[i];
                    const float y_diff = table->y[i + 1] - table->y[i];
                    slope = y_diff / x_diff;
                }
                found = true;
                break;
            }
        }

        if (found == false) {
            // only possible if x is outside the range of the table.
            if (x <= table->x[0]) {
                idx = 0;
                slope = 0.0F;
            } else {
                idx = table->size - 1;
                slope = 0.0F;
            }
        }

        const float delta_x = x - table->x[idx];

        *y = table->y[idx] + slope * delta_x;
    }

    return ret;
}