#pragma once

#include "pros/optical.hpp"
#include "pros/colors.hpp"

class Color_Sort {
public:
    Color_Sort(pros::Optical *optical);

    pros::Color get_color();

    bool object_detected();

    pros::Color object_color();

    pros::Color last_color;
private:
    pros::Optical *optical;
};