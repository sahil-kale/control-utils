#ifndef HYSTERESIS_CONTROLLER_H
#define HYSTERESIS_CONTROLLER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    float upper_threshold;  /// Threshold input at which the controller will switch from false to true
    float lower_threshold;  /// Threshold input at which the controller will switch from true to false
} control_utils_hysteresis_controller_config_t;

typedef struct {
    bool state;
    control_utils_hysteresis_controller_config_t const *config;
} control_utils_hysteresis_controller_data_t;

/**
 * @brief Initialize hysteresis controller
 *
 * @param hysteresis_controller_data Pointer to hysteresis controller data structure
 * @param hysteresis_controller_config Pointer to hysteresis controller configuration structure
 *
 * @return true if initialization was successful
 */
bool control_utils_hysteresis_controller_init(
    control_utils_hysteresis_controller_data_t *hysteresis_controller_data,
    control_utils_hysteresis_controller_config_t const *const hysteresis_controller_config);

/**
 * @brief Run hysteresis controller
 *
 * @param hysteresis_controller_data Pointer to hysteresis controller data structure
 * @param input Input value
 *
 * @return true if hysteresis controller was run successfully, false otherwise
 * @note The state of the hysteresis controller is stored in the state field of the hysteresis_controller_data struct
 */
bool control_utils_hysteresis_controller_run(control_utils_hysteresis_controller_data_t *const hysteresis_controller_data,
                                             float input);

#ifdef __cplusplus
}
#endif

#endif  // HYSTERESIS_CONTROLLER_H