#pragma once

#include "pros/rotation.hpp"
#include "pros/distance.hpp"
#include "pros/colors.hpp"
#include "color_sort.hpp"
#include <vector>

class Intake {
    bool is_scheduled_motion();
    std::vector<double> hook_poses;
    void move_hook_to_ready();
    void pickup_and_hold();
    void load_lb();
    void score_ring();
    void sort_ring();
    
    private:
    pros::Rotation *rotation;
    pros::Distance *upper_distance;
    pros::Distance *lower_distance;
    pros::Color *upper_color;
    pros::Color *lower_color;

    pros::Color get_color();
    bool object_detected();
    pros::Color object_color();
    pros::Color last_color;
    double convert_rotation_to_length (double angle);
    void nearest_hook_to_pose(double pose);
};