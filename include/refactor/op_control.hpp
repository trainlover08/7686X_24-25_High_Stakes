#pragma once

#include "connor.h"
#include "constructors.hpp"
#include "refactor/intake.hpp"
#include <cmath>
//#include "op_control.hpp"

bool mut = 1;

void intake_task () {
    while (1) {
        if (mut) {
            if (intake_button_combo.is_pressing()) {
                intake.move();
            } else if (extake_button_combo.is_pressing()) {
                intake.move(true);
            } else {
                intake.stop();
            }
        }
        pros::delay(10);
    }
}

void ui_task () {
    while (1) {
        std::string x_str = "X: " + std::to_string(chassis.getPose().x);
        std::string y_str = "Y: " + std::to_string(chassis.getPose().y);
        std::string t_str = "Theta: " + std::to_string(chassis.getPose().theta);
        console.clear();
        console.println(x_str);
        console.println(y_str);
        console.println(t_str);
        pros::delay(50);
    }
}

void drive_task () {
    while (1) {
        int x_axis = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int y_axis = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        if (abs(x_axis) < 5 || abs(y_axis) < 5) {
            chassis.arcade(x_axis, y_axis);
        } else {
            chassis.curvature(x_axis, y_axis);
        }
        pros::delay(10);
    }
}

void misc_task () {
    while (1) {
        if (mogo_button_combo.is_pressing()) {
            mogo_mech_piston.toggle();
            if (!mogo_mech_piston.is_extended()) {
                mut=0;
                intake.move_upper(100, false);
                pros::delay(100);
                mut=1;
            }
            while (mogo_button_combo.is_pressing()) {
                pros::delay(10);
            }
        }
        if (mogo_button_combo.is_pressing()) {
            doinker_piston.extend();
        } else {
            doinker_piston.retract();
        }
        pros::delay(10);
    }
}

void lady_brown_intake_task () {
    Lady_Brown::position* pose = &hold;
    lady_brown_rotation.calibrate();
    while (1) {
        if (lb_down_button.is_pressing()) {
            pose = pose->angle == hold.angle ? pose = &load : pose->angle == load.angle ? pose = &score :  pose = &hold;
            if (pose->angle == load.angle) {lady_brown.use_bang_bang = true;} else {lady_brown.use_bang_bang = false;}
            while (lb_down_button.is_pressing()) pros::delay(10);
        }
        lady_brown.move_to_position(pose);
        pros::delay(10);
    }
}