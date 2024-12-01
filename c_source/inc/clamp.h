#ifndef CLAMP_H
#define CLAMP_H

/**
 * @brief Clamp value between min and max
 *
 * @param value Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 *
 * @return Clamped value
 */
inline float control_util_clamp(float value, float min, float max) {
    float ret = value;
    if (value < min) {
        ret = min;
    } else if (value > max) {
        ret = max;
    } else {
        // Do nothing
    }

    return ret;
}

#endif