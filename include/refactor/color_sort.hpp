#pragma once

#include "pros/optical.hpp"
#include "pros/colors.hpp"
#include "pros/distance.hpp"

class Color_Sort {
    public:
    Color_Sort(pros::Optical *optical, pros::Distance *distance);

    pros::Color get_color();

    bool object_detected();

    pros::Color object_color();

    pros::Color last_color;
    private:
    pros::Optical *optical;
    pros::Distance *distance;
};