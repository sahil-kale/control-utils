#ifndef LOW_PASS_FILTER_H
#define LOW_PASS_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    float alpha;
    float output;
} control_utils_lpf_t;

/**
 * @brief Initialize a low pass filter
 *
 * @param lpf Pointer to the low pass filter
 * @param fc Cut-off frequency of the low pass filter in Hz
 * @param dt Sampling time of the filter in seconds
 *
 * @return true if the low pass filter was initialized successfully, false otherwise
 */
bool control_utils_lpf_init(control_utils_lpf_t *lpf, float fc, float dt);

/**
 * @brief Run the low pass filter
 *
 * @param lpf Pointer to the low pass filter
 * @param input Input to the low pass filter
 *
 * @return true if the low pass filter was run successfully, false otherwise
 * @note The output of the low pass filter is stored in the output field of the lpf struct
 */
bool control_utils_lpf_step(control_utils_lpf_t *lpf, float input);

#ifdef __cplusplus
}
#endif

#endif  // LOW_PASS_FILTER_H