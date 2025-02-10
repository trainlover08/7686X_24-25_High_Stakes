#include "refactor/intake.hpp"
#include <cmath>

Intake::Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, Color_Sort* lower_sort, Color_Sort* upper_sort) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->lower_sort = lower_sort;
     this->upper_sort = upper_sort;
}

Intake::Intake (pros::Motor* lower_intake, pros::Motor* upper_intake) {
     this->lower_intake = lower_intake;
     this->upper_intake = upper_intake;
     this->lower_sort = nullptr;
     this->upper_sort = nullptr;
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

void Intake::move () {
     this->lower_intake->move(12000);
     this->upper_intake->move(12000);
}

void Intake::move (bool reverse) {
     if (reverse) {
          this->lower_intake->move(-12000);
          this->upper_intake->move(-12000);
     } else {Intake::move();}
}

void Intake::move (bool reverse, double percentage) {
     reverse = 2 * reverse - 1;
     percentage *= 12000.0 / 100.0;
     percentage *= reverse;
     this->lower_intake->move(percentage);
     this->upper_intake->move(percentage);
}

void Intake::move_lower (double percentage) {
     this->lower_intake->move((percentage * 12000.0) / 100.0);
}

void Intake::move_lower (double percentage, bool reverse) {
     percentage *= reverse * 2 - 1;
     this->move_lower(percentage);
}

void Intake::move_upper (double percentage) {
     this->upper_intake->move((percentage * 12000.0) / 100.0);
}

void Intake::move_upper (double percentage, bool reverse) {
     percentage *= reverse * 2 - 1;
     this->move_upper(percentage);
}

void Intake::stop () {
     this->upper_intake->move(0);
     this->lower_intake->move(0);
}

void Intake::stop_lower () {
     this->lower_intake->move(0);
}

void Intake::stop_upper () {
     this->upper_intake->move(0);
}

// call as a task
void Intake::lb_relief() {
     this->move_upper(100, true);
     pros::delay(this->lb_relief_timeout);
     this->stop_upper();
}