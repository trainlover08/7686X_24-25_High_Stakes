#include "refactor/lady_brown.hpp"

Lady_Brown::Lady_Brown(pros::Motor* motor, pros::adi::Potentiometer* rotation, lemlib::PID* pid) {
    this->motor = motor;
    this->rotation = rotation;
    this->pid = pid;
}

void Lady_Brown::move_to_position (const Lady_Brown::position& pos) {
    double error = pos.angle - this->rotation->get_value();
    double output = this->pid->update(error);
    this->motor->move(output);
}

void Lady_Brown::move_to_position (const Lady_Brown::position* pos) {
    double error = pos->angle - this->rotation->get_value();
    if (this->use_bang_bang) {
        double output = this->pid->update(error);
        this->motor->move(output - 10);
    } else {
        if (error < -10) {
            this->motor->move_velocity(-200);
        } else if (error > 10) {
            this->motor->move_velocity(200);
        } else {
            this->motor->move_velocity(0);
        }
    }
}

bool Lady_Brown::is_at_position (const Lady_Brown::position& pos) {
    if (rotation->get_angle() + 500 >= pos.angle && rotation->get_angle() - 500 <= pos.angle) { // small error range
        return true;
    }
    return false;
}