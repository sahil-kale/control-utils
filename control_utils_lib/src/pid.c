#include "pid.h"

#include <stddef.h>

#include "clamp.h"

bool control_utils_pid_init(control_utils_pid_data_t *pid_data, control_utils_pid_config_t const *const pid_config,
                            control_utils_pid_limits_t const *const pid_limits) {
    bool ret = false;
    if ((pid_data != NULL) && (pid_config != NULL)) {
        ret = true;

        pid_data->accumulator = 0.0F;
        pid_data->last_error = 0.0F;
        pid_data->output = 0.0F;

        pid_data->config = pid_config;
        pid_data->limits = pid_limits;

        if (pid_config->dt > 0.0F) {
            pid_data->one_over_dt = 1.0F / pid_config->dt;
        } else {
            ret = false;
        }

        pid_data->initialized = ret;
    }
    return ret;
}

bool control_utils_pid_run(float error, control_utils_pid_data_t *const pid_data) {
    bool ret = false;

    const bool pid_data_initialized = (pid_data != NULL) && (pid_data->initialized);
    const bool pid_config_initialized = (pid_data_initialized) && (pid_data->config != NULL);
    const bool pid_limits_initialized =
        (pid_data_initialized) && (pid_data->limits != NULL) && (pid_data->limits->max_windup > 0.0);

    if (pid_data_initialized && pid_config_initialized) {
        // We prescale the integral gain by ki to make this play nicer with gain scheduled controllers
        pid_data->accumulator += error * pid_data->config->dt * pid_data->config->ki;
        if (pid_limits_initialized) {
            pid_data->accumulator =
                control_utils_clamp(pid_data->accumulator, -pid_data->limits->max_windup, pid_data->limits->max_windup);
        }

        const float derivative = (error - pid_data->last_error) * pid_data->one_over_dt;

        pid_data->output = pid_data->config->kp * error + pid_data->accumulator + pid_data->config->kd * derivative;
        pid_data->last_error = error;
        ret = true;
    }
    return ret;
}