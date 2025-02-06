#include "constexpr_math.hpp"

constexpr double cxprmath::cos (double x) {
    double result = 1.0;
    double term = 1.0;
    double x_squared = x * x;
    for (int n = 1; n <= 10; ++n) {
        term *= -x_squared / (2 * n * (2 * n - 1));
        result += term;
    }
    return result;
}

constexpr double cxprmath::sin (double x) {
    double result = x;
    double term = x;
    double x_squared = x * x;
    for (int n = 1; n < 10; ++n) {
        term *= -x_squared / ((2 * n + 1) * (2 * n));
        result += term;
    }
    return result;
}

constexpr double cxprmath::clean_angle_degrees (double t) {
    // do not input too big of a value
    bool sign = 0;
    if (t < 0.0) {
        sign = 1;
        t *= -1.0;
    }
    for (int i = 0; i < 100; ++i) {
        if (t > 360.0) break;
        t -= 360.0;
    }
    return sign ? t *= -1.0 : t;
}