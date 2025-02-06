#include "include/MCL/refactor/particle_filter.hpp"


Filter::Filter () {

}

constexpr Filter::Particle::Particle (double x, double y, double theta) {
    this->x_1 = x;
    this->y_1 = y;
    this->theta_1 = theta;
    this->x_2 = x;
    this->y_2 = y;
    this->theta_2 = theta;
}

constexpr void Filter::Particle::update (double x, double y, double theta) {
    this->x_2 = this->x_1;
    this->y_2 = this->y_1;
    this->theta_2 = this->theta_1;
    this->x_1 = x;
    this->y_1 = y;
    this->theta_1 = theta;
}

constexpr std::array<double, 3> Filter::Particle::grad () {
    return {this->x_1 - this->x_2, this->y_1 - this->y_2, this->theta_1 - this->theta_2};
}

Filter::Sensor::Sensor (pros::Distance* d, double x, double y, double theta) {
    this->d = d;
    this->x = x;
    this->y = y;
    this->theta = theta;
}

constexpr double Filter::Sensor::get_raw () {
    return d->get();
}

constexpr double Filter::Sensor::distance (double raw) {
    // add extra logic here
    return raw;
}

constexpr Filter::Robot::Robot (lemlib::Pose* odom_pose, std::vector<Sensor*> sensor_vector) {
    this->odom_pose = odom_pose;
    this->sensor_vector = sensor_vector;
}

constexpr std::array<double, 2> Filter::find_intercept (double global_theta, double x, double y) {
    // x and y are in 0:144 format
    if (global_theta > 0 && global_theta <= 90) {
        double solution_1 = x + (cxprmath::sin(global_theta) * (cxprmath::sin(180.0 - (90.0 + global_theta)) / 144.0 - y));
        double solution_2 = y + (cxprmath::sin(90.0 - global_theta) * (cxprmath::sin(180.0 - global_theta) / 144.0 - x));
        return (solution_1 > 144.0 || solution_1 < 0.0) ? {144.0, solution_2} : {solution_1, 144.0};
    } else if (global_theta > 90 && global_theta <= 180) {
        // @todo fill the rest of this out
    } else if (global_theta > 180 && global_theta <= 270) {

    } else {

    }
}