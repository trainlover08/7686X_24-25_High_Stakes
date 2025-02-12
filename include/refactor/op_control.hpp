#pragma once

#include "constructors.hpp"
//#include "op_control.hpp"

void intake_task () {
    while (1) {
        if (intake_button.is_pressing()) {
            intake.move();
        } else if (extake_button.is_pressing()) {
            intake.move(true);
        } else {
            intake.stop();
        }
        pros::delay(10);
    }
}

void ui_task () {
    //selector.focus();
}

void drive_task () {
    while (1) {
        int x = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int y = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        if (cxprmath::abs(x) < 5 || cxprmath::abs(y) < 5) {
            chassis.arcade(x, y);
        } else {
            chassis.curvature(x, y);
        }
        pros::delay(10);
    }
}

void misc_task () {
    while (1) {
        if (mogo_button.is_pressing()) {
            mogo_mech_piston.toggle();
            while (mogo_button.is_pressing()) {
                pros::delay(10);
            }
        }
        pros::delay(10);
    }
}