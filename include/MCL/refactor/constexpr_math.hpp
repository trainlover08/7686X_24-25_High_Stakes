#pragma once

/** Note: This is not templated */

namespace cxprmath {
    constexpr double cos (double x);
    constexpr double sin (double x);
    constexpr double clean_angle_degrees (double t, bool abs);
    constexpr double deg_to_rad(double t);
    constexpr double PI = 3.14159265358979323846;
    constexpr double abs (double x);
    constexpr double EPSILON = 1e-10;
    constexpr double newton_sqrt(double x, double curr, double prev);
    constexpr double sqrt(double x);
    constexpr double hypot(double x, double y);
    constexpr double ln(double x);
    constexpr double exp(double x);
    constexpr double pow(double x, double y);
    constexpr double atan2(double x, double y);
    constexpr double atan(double z);
    constexpr double atan_series(double z);
    constexpr double rad_to_degrees(double r);
};