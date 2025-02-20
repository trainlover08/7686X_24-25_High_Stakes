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
    std::string x_str = "X: " + std::to_string(chassis.getPose().x);
    std::string y_str = "Y: " + std::to_string(chassis.getPose().y);
    std::string t_str = "Theta: " + std::to_string(chassis.getPose().theta);
    console.clear();
    console.println(x_str);
    console.println(y_str);
    console.println(t_str);
    pros::delay(50);
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
    double v2 = 0.0;
    unsigned int que = 0;
    while (1) {
        mut=0;
        double v1 = v2;
        v2 = upper_intake_motor.get_velocity();
        if (upper_intake_motor.get_torque() > 0.5 * upper_intake_motor.get_max_torque()) {
            if (upper_intake_motor.get_max_velocity() * 0.1 > upper_intake_motor.get_velocity()) {
                if (((v2 - v1 <= 0.0) && (pose.angle == hold.angle)) && (lady_brown.is_stopped())) {
                    while (!lb_down_button.is_pressing()) pros::delay(10);
                    pose = &score;
                    while (lb_down_button.is_pressing()) pros::delay(10);
                }
            }
        }
        mut=1;
        que += lb_que_button.pressing();
        while (lb_que_button.pressing()) pros::delay(10);
        if (que > 0) {
            if (pose.angle == hold.angle) pose = &load;
        }
        if (lb_down_button.is_pressing()) {
            pose = &hold;
            while (lb_down_button.is_pressing()) pros::delay(10);
        }
        lady_brown.move_to_position(pose);
        pros::delay(10);
    }
}