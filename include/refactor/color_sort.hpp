#pragma once

#include "pros/optical.hpp"
#include "pros/colors.hpp"
#include "pros/distance.hpp"

class Color_Sort {
    pros::Color get_color();

    bool object_detected();

    pros::Color last_color;
};