#pragma once

/** Note: This is not templated */

namespace cxprmath {
    constexpr double cos (double x);
    constexpr double sin (double x);
    constexpr double clean_angle_degrees (double t);
    constexpr double deg_to_rad(double t);
    constexpr double M_PI = 3.14159265358979323846;
    constexpr double abs (double x);
    constexpr double epsilon 1e-10;
    constexpr double newton_sqrt(double x, double curr, double prev);
    constexpr double sqrt(double x);
    constexpr double hypot(double x, double y);
};