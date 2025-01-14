#include "pid.h"

#include <stddef.h>

#include "clamp.h"

bool control_utils_pid_init(pid_data_t *pid_data) {
    bool ret = false;
    if (pid_data != NULL) {
        ret = true;

        pid_data->accumulator = 0.0F;
        pid_data->last_error = 0.0F;
        pid_data->output = 0.0F;
        pid_data->initialized = true;
    }
    return ret;
}

bool control_utils_pid_run(float error, pid_data_t *const pid_data, pid_config_t const *const pid_config,
                           pid_limits_t const *const pid_limits) {
    bool ret = false;

    const bool pid_data_initialized = (pid_data != NULL) && (pid_data->initialized);
    const bool pid_config_initialized = (pid_config != NULL) && (pid_config->dt > 0.0);
    const bool pid_limits_initialized = (pid_limits != NULL) && (pid_limits->max_windup > 0.0);

    if (pid_data_initialized && pid_config_initialized) {
        // We prescale the integral gain by ki to make this play nicer with gain scheduled controllers
        pid_data->accumulator += error * pid_config->dt * pid_config->ki;
        if (pid_limits_initialized) {
            pid_data->accumulator = control_util_clamp(pid_data->accumulator, -pid_limits->max_windup, pid_limits->max_windup);
        }

        const float derivative = (error - pid_data->last_error) / pid_config->dt;

        pid_data->output = pid_config->kp * error + pid_data->accumulator + pid_config->kd * derivative;
        pid_data->last_error = error;
        ret = true;
    }
    return ret;
}