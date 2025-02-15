#pragma once

#include "pros/distance.hpp"
#include "lemlib/pose.hpp"
#include <array>
#include <vector>

class Filter {
public:

    struct My_Pose {
    public:
        double x, y, theta;
    };

    class Particle {
    public:
        constexpr Particle(double x, double y, double theta);
        double x_1, y_1, theta_1;
        double x_2, y_2, theta_2;
        constexpr void update(double x, double y, double theta);
        constexpr void update_for_odom (lemlib::Pose* pose);
        constexpr std::array<double, 3> grad ();
        double fitness;
        lemlib::Pose* last_pose;
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
        constexpr Robot(lemlib::Pose* odom_pose, std::vector<Sensor*> sensor_vector);
        lemlib::Pose* odom_pose;
        std::vector<Sensor*> sensor_vector;
    };

    class Monte_Carlo {
    public:
        Monte_Carlo(Robot* robot);
        constexpr std::array<Particle*, 20> select (std::array<Particle*, 100> p_arr, Robot* r);
        constexpr std::array<Particle*, 100> breed (std::array<Particle*, 20> p_arr);
        std::array<Particle*, 100> mutate (std::array<Particle*, 100> p_arr);
        lemlib::Pose* cycle ();
        std::array<Particle*, 100> population;
        Robot* robot;
        constexpr lemlib::Pose* weighted_average (std::array<Particle*, 20> p_arr);
        lemlib::Pose* pose;
    };
};

class MCL {
    /** @todo needs more testing */
public:
    struct sensor {
        pros::Distance* d;
        double x, y, theta;
    };
    MCL(lemlib::Pose* pose, std::vector<sensor> sensors);
    lemlib::Pose* pose;
    Filter filter;
    std::vector<Filter::Sensor*> sensor_vec;
    Filter::Robot* robot;
    Filter::Monte_Carlo* mc;
    lemlib::Pose* update();
};

namespace helper {
    constexpr std::array<double, 2> find_intercept (double global_theta, double x, double y);
    constexpr double calculate_distance (std::array<std::array<double, 2>, 2>);
    constexpr double calculate_loss (double o, double e, int raw);
    template <typename T, std::size_t N>
    constexpr std::array<int, N> bubble_sort(std::array<T, N> arr);
    constexpr double rayBoxIntersection (double x0, double y0, double dx, double dy, double x_min, double x_max, double y_min, double y_max);
    constexpr double findSquareInterceptDistance(double x0, double y0, double theta);
};