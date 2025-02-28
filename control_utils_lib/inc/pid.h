#ifndef PID_H
#define PID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

typedef struct {
    float max_windup;  /// Maximum windup value. Leave as 0.0 if not used
} control_utils_pid_limits_t;

typedef struct {
    float kp;  /// Proportional gain
    float ki;  /// Integral gain
    float kd;  /// Derivative gain

    float dt;  /// Time step (seconds)
} control_utils_pid_config_t;

typedef struct {
    float accumulator;
    float last_error;
    float output;
    bool initialized;

    float one_over_dt;

    control_utils_pid_config_t const *config;
    control_utils_pid_limits_t const *limits;
} control_utils_pid_data_t;

/**
 * @brief Initialize PID controller
 *
 * @param pid_data Pointer to PID data structure
 * @param pid_config Pointer to PID gains structure
 * @param pid_limits Pointer to PID limits structure
 *
 * @return true if initialization was successful
 */
bool control_utils_pid_init(control_utils_pid_data_t *pid_data, control_utils_pid_config_t const *const pid_config,
                            control_utils_pid_limits_t const *const pid_limits);

/**
 * @brief Run PID controller
 *
 * @param error Error value (error = setpoint - output)
 * @param pid_data Pointer to PID data structure
 *
 * @return true if PID controller was run successfully, false otherwise
 */
bool control_utils_pid_run(float error, control_utils_pid_data_t *const pid_data);

#ifdef __cplusplus
}
#endif

#endif  // PID_H