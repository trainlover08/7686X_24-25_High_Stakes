#pragma once

#include "pros/distance.hpp"
#include "lemlib/pose.hpp"
#include <array>
#include <vector>
#include "constexpr_math.hpp"

class Filter {
public:
    Filter();

    class Particle {
    public:
        Particle(double x, double y, double theta);
        double x_1, y_1, theta_1;
        double x_2, y_2, theta_2;
        constexpr void update(double x, double y, double theta);
        constexpr std::array<double, 3> grad ();
        double fitness;
    };

    class Sensor {
    public:
        // relative to the tracking center of the robot
        Sensor(pros::Distance* d, double x, double y, double theta);
        double x, y, theta;
        constexpr double distance(double raw);
        constexpr double get_raw();
        pros::Distance* d;
    };

    class Robot {
    public:
        Robot(lemlib::Pose* odom_pose, std::vector<Sensor*> sensor_vector);
        lemlib::Pose* odom_pose;
        std::vector<Sensor*> sensor_vector;
    };

    constexpr std::array<double, 2> find_intercept (double global_theta, double x, double y);

    constexpr double calculate_distance (std::array<std::array<double, 2>, 2>);
};