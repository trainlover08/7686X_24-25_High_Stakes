#include "refactor/lady_brown.hpp"
#include "connor.h"

Lady_Brown::Lady_Brown(pros::Motor* motor, pros::adi::Potentiometer* rotation, lemlib::PID* pid) {
    this->motor = motor;
    this->rotation = rotation;
    this->pid = pid;
    this->motor->set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    this->motor->set_zero_position(((((this->rotation->get_value() - lady_brown_ready_macro)  / 9999.0) * 3.0) * 360.0) + this->motor->get_position()); // look at this is a problem occurs
}

void Lady_Brown::move_to_position (const Lady_Brown::position& pos) {
    double error = pos.angle - this->rotation->get_value();
    double output = this->pid->update(error);
    this->motor->move(output);
}

void Lady_Brown::move_to_position (const Lady_Brown::position* pos) {
    this->motor->move_absolute(pos->angle, 200);
}

bool Lady_Brown::is_at_position (const Lady_Brown::position& pos) {
    if (rotation->get_angle() + 500 >= pos.angle && rotation->get_angle() - 500 <= pos.angle) { // small error range
        return true;
    }
    return false;
}