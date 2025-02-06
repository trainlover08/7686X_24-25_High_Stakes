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
    double theta_rad = cxprmath::deg_to_rad(global_theta);
    
    double cos_theta = cxprmath::cos(theta_rad);
    double sin_theta = cxprmath::sin(theta_rad);
    
    double t_x, t_y;
    if (cos_theta > 0) {
        t_x = (144.0 - x) / cos_theta;
    } else if (cos_theta < 0) {
        t_x = (0.0 - x) / cos_theta;
    } else {
        t_x = 1e9;
    }
    
    if (sin_theta > 0) {
        t_y = (144.0 - y) / sin_theta;
    } else if (sin_theta < 0) {
        t_y = (0.0 - y) / sin_theta;
    } else {
        t_y = 1e9;
    }
    
    // find first intercept
    double t = (t_x < t_y) ? t_x : t_y;
    
    return { x + t * cos_theta, y + t * sin_theta };
}

constexpr double Filter::calculate_distance (std::array<std::array<double, 2>, 2> arr) {
    return cxprmath::hypot();
}