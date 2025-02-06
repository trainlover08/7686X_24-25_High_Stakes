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

constexpr double cxprmath::deg_to_rad (double t) {
    return (t / 180.0) * cxprmath::M_PI;
}

constexpr double cxprmath::abs (double x) {
    return x > 0.0 ? x : -x;
}

constexpr double cxprmath::newton_sqrt (double x, double curr, double prev) {
    return (cxprmath::abs(curr - prev) < cxprmath::EPSILON) ? curr : cxprmath::newton_sqrt(x, 0.5 * (curr + x / curr), curr));
}

constexpr double cxprmath::sqrt (double x) {
    return x < 0.0 ? -1.0 : x == 0 ? 0.0 : cxprmath::newton_sqrt(x, x, 0);
}

constexpr double cxprmath::hypot (double x, double y) {
    return cxprmath::sqrt(x * x + y * y);
}