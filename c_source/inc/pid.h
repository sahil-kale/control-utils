#ifndef PID_H
#define PID_H

#include <stdbool.h>

typedef struct {
    float max_windup;  /// Maximum windup value. Leave as 0.0 if not used
} pid_limits_t;

typedef struct {
    float kp;  /// Proportional gain
    float ki;  /// Integral gain
    float kd;  /// Derivative gain

    float dt;  /// Time step (seconds)
} pid_config_t;

typedef struct {
    float accumulator;
    float last_error;
    float output;
    bool initialized;
} pid_data_t;

/**
 * @brief Initialize PID controller
 *
 * @param pid_data Pointer to PID data structure
 *
 * @return true if initialization was successful
 */
bool control_utils_pid_init(pid_data_t *pid_data);

/**
 * @brief Run PID controller
 *
 * @param error Error value (error = setpoint - output)
 * @param pid_data Pointer to PID data structure
 * @param pid_config Pointer to PID gains structure
 * @param pid_limits Pointer to PID limits structure
 *
 * @return true if PID controller was run successfully, false otherwise
 */
bool control_utils_pid_run(float error, pid_data_t *const pid_data, pid_config_t const *const pid_config,
                           pid_limits_t const *const pid_limits);

#endif  // PID_H