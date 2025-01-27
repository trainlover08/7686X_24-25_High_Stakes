#include "refactor/color_sort.hpp"

Color_Sort::Color_Sort (pros::Optical *optical, pros::Distance *distance) {
    this->optical = optical;
    this->distance = distance;
}

Color_Sort::get_color () {
    return optical->get_rgb().red > optical->get_rgb().blue? pros::Color::red : pros::Color::blue;
}