#include "low_pass_filter.h"

#include <stddef.h>

static float control_utils_lpf_compute_alpha(float fc, float dt) {
    /**
     * Backward Euler Discretization of an lpf with the form H(s) = 1 / (s/w0 + 1)
     * plug s <- 1/T (1-z^-1)
     *
     * Rearrange into form y[k] = alpha * u[k] + (1 - alpha) * y[k-1]
     *
     * where:
     * alpha = w0 * dt / (w0 * dt + 1)
     */

    const float w0_T = 2.0F * 3.14159F * fc * dt;
    return w0_T / (w0_T + 1.0F);
}

bool control_utils_lpf_init(control_utils_lpf_t *lpf, float fc, float dt) {
    bool ret = lpf != NULL;

    if (ret) {
        lpf->alpha = control_utils_lpf_compute_alpha(fc, dt);
        lpf->output = 0.0F;
    }

    return ret;
}

bool control_utils_lpf_step(control_utils_lpf_t *lpf, float input) {
    const bool ret = lpf != NULL;

    if (ret) {
        lpf->output = lpf->alpha * input + (1.0F - lpf->alpha) * lpf->output;
    }

    return ret;
}