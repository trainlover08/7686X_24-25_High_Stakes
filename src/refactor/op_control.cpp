#pragma once

#include "constructors.cpp"
#include "auto_control.cpp"

void intake_task () {
    while (1) {
        if (intake_button.is_pressing()) {
            intake.move();
        } else if (extake_button.is_pressing()) {
            intake.move(true);
        }
        pros::delay(10);
    }
}

void ui_task () {
    selector.focus();
}

void drive_task () {
    while (1) {
        chassis.curvature(master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y), master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X));
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