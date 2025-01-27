#pragma once

#include "pros/rotation.hpp"
#include "pros/distance.hpp"
#include "pros/colors.hpp"

class intake {
    bool is_spinning();
    pros::Rotation get_direction();
    pros::Color get_color();
    bool object_detected();
    pros::Color object_color();
    pros::Color last_color;
    
    private:
    pros::Rotation *rotation;
    pros::Distance *upper_distance;
    pros::Distance *lower_distance;
    pros::Color *upper_color;
    pros::Color *lower_color;
};