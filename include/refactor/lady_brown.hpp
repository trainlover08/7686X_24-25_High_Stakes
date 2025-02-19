#pragma once

#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "pros/colors.hpp"
#include "lemlib/pid.hpp"

class Lady_Brown {
    public:
        Lady_Brown(pros::Motor* motor, pros::adi::Potentiometer* rotation, lemlib::PID* pid);

        struct position {
            double angle;
            lemlib::PID* pid = pid;
        };

        void move_to_position(const position& pos);
        void move_to_position(const position* pos);

        bool is_at_position(const position& pos);

        pros::Color slot_1_color, slot_2_color;

    private:
    pros::Motor* motor;
    lemlib::PID* pid;
    pros::adi::Potentiometer* rotation;
};