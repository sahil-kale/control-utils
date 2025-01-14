#include "hysteresis_controller.h"

#include <stddef.h>

bool control_utils_hysteresis_controller_init(
    control_utils_hysteresis_controller_data_t *hysteresis_controller_data,
    control_utils_hysteresis_controller_config_t const *const hysteresis_controller_config) {
    const bool ret = (hysteresis_controller_data != NULL) && (hysteresis_controller_config != NULL);

    if (ret) {
        hysteresis_controller_data->config = hysteresis_controller_config;
        hysteresis_controller_data->state = false;
    }

    return ret;
}

bool control_utils_hysteresis_controller_run(control_utils_hysteresis_controller_data_t *const hysteresis_controller_data,
                                             float input) {
    const bool ret = hysteresis_controller_data != NULL;

    if (ret) {
        if ((hysteresis_controller_data->state == true) && (input <= hysteresis_controller_data->config->lower_threshold)) {
            hysteresis_controller_data->state = false;
        } else if ((hysteresis_controller_data->state == false) &&
                   (input >= hysteresis_controller_data->config->upper_threshold)) {
            hysteresis_controller_data->state = true;
        } else {
            // Not met any hysteresis condition, just chill
        }
    }

    return ret;
}
