#include "refactor/intake.hpp"
#include <cmath>

Intake::Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, Color_Sort* lower_sort, Color_Sort* upper_sort) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->lower_sort = lower_sort;
     this->upper_sort = upper_sort;
}

double Intake::convert_angle_to_length (double angle) {
     // centigrees to degrees
     angle /= 100;
     // multiple my circumference
     double distance;
     distance = (angle/360) * (diameter * M_PI);
     // make sure length is always sanitized
     distance = distance > chain_length ? distance - chain_length : distance;
     distance = 0.0f > distance ? distance : distance + chain_length;
     return distance;
}