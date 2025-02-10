#include "refactor/color_sort.hpp"

/** @todo Finish this file. Not in a working state */

Color_Sort::Color_Sort (pros::Optical *optical, pros::Distance *distance) {
    this->optical = optical;
    this->distance = distance;
}

pros::Color Color_Sort::get_color () {
    pros::Color color = optical->get_rgb().red > optical->get_rgb().blue? pros::Color::red : pros::Color::blue;
    Color_Sort::last_color = color;
    return color;
}

bool Color_Sort::object_detected () {
    return distance->get_distance() < 4;
}

pros::Color Color_Sort::object_color () {
    if (Color_Sort::object_detected()) {
        return Color_Sort::get_color();
    }
    return pros::Color::gray; // a default for if there isn't an object
}