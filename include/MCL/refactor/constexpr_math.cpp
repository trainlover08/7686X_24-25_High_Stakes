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

constexpr double cxprmath::clean_angle_degrees (double t, bool abs) {
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
    t = sign ? t *= -1.0 : t;
    if (!abs) return t;
    return t > 0.0 ? t : t + 360.0;
}

constexpr double cxprmath::deg_to_rad (double t) {
    return (t / 180.0) * cxprmath::PI;
}

constexpr double cxprmath::abs (double x) {
    return x > 0.0 ? x : -x;
}

constexpr double cxprmath::newton_sqrt (double x, double curr, double prev) {
    return (cxprmath::abs(curr - prev) < cxprmath::EPSILON) ? curr : cxprmath::newton_sqrt(x, 0.5 * (curr + x / curr), curr);
}

constexpr double cxprmath::sqrt (double x) {
    return x < 0.0 ? -1.0 : x == 0 ? 0.0 : cxprmath::newton_sqrt(x, x, 0);
}

constexpr double cxprmath::hypot (double x, double y) {
    return cxprmath::sqrt(x * x + y * y);
}

constexpr double cxprmath::ln (double x) {
    if (x <= 0.0) return 0.0;
    int k = 0;
    while (x > 2.0) {
        x /= 2.0;
        ++k;
    }
    while (x < 1.0) {
        x *= 2.0;
        --k;
    }
    double z = x - 1.0;
    double term = z;
    double result = 0.0;
    int n = 1;
    while (cxprmath::abs(term) > cxprmath::EPSILON) {
        result += ((n % 2) ? term : -term) / n;
        ++n;
        term *= z;
    }
    return result + k * 0.6931471805599453;
}

constexpr double cxprmath::exp (double x) {
    double sum = 1.0;
    double term = 1.0;
    int n = 1;
    while (cxprmath::abs(term) > cxprmath::EPSILON) {
        term *= x / n;
        sum += term;
        ++n;
    }
    return sum;
}

constexpr double cxprmath::pow (double x, double y) {
    if (y == 0.0) return 1.0;
    if (y < 0.0) return 1.0 / cxprmath::pow(x, -y);
    if (y == static_cast<int>(y)) {
        int expInt = static_cast<int>(y);
        double result = 1.0;
        double b = x;
        while (expInt > 0) {
            if (expInt & 1) result *= b;
            b *= b;
            expInt /=2;
        }
        return result;
    }
    return cxprmath::exp(y * cxprmath::ln(x));
}

constexpr double cxprmath::atan_series (double z) {
    double term = z;
    double result = z;
    double z2 = z * z;
    int i = 1;
    for (; i < 1000; ++i) {
        term *= -z2;
        double next_term = term / (2 * i + 1);
        if (cxprmath::abs(next_term) < cxprmath::EPSILON) break;
        result += next_term;
    }
    return result;
}

constexpr double cxprmath::atan (double z) {
    if (z < 0) return -cxprmath::atan(-z);
    if (z > 1) return cxprmath::PI / 2 - cxprmath::atan(1 / z);
    return cxprmath::atan_series(z);
}
constexpr double cxprmath::atan2 (double x, double y) {
    if (x > 0) {
        return cxprmath::atan(y / x);
    } else if (x < 0 && y >= 0) {
        return cxprmath::atan(y / x) + cxprmath::PI;
    } else if (x < 0 && y < 0) {
        return cxprmath::atan(y / x) - cxprmath::PI;
    } else if (x == 0 && y > 0) {
        return cxprmath::PI / 2;
    } else if (x == 0 && y < 0) {
        return -cxprmath::PI / 2;
    } else {
        return 0.0;
    }
}

constexpr double cxprmath::rad_to_degrees (double r) {
    return r * 180.0 / cxprmath::PI;
}