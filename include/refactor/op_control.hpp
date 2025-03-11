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
            //if (intake_button_combo.is_pressing()) {
            if (1) {
                intake.move();
                intake.color_sort(pros::Color::red);
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
        std::string dist = "Distance: " + std::to_string(opt.get_proximity());
        std::string rbg = "R: " + std::to_string(opt.get_rgb().red) + "B: " + std::to_string(opt.get_rgb().blue);
        std::string ring_str = opt.get_proximity() > 140 ? opt.get_rgb().red > opt.get_rgb().blue ? "Red" : "Blue": "Null";
        ring_str = "Ring: " + ring_str;
        console.clear();
        //console.println(x_str);
        //console.println(y_str);
        //console.println(t_str);
        console.println(dist);
        console.println(rbg);
        console.println(ring_str);
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
        if (doinker_button_combo.is_pressing()) {
            doinker_piston.extend();
        } else {
            doinker_piston.retract();
        }
        pros::delay(10);
    }
}

void lady_brown_intake_task () {
    Lady_Brown::position* pose = &hold;
    lady_brown_motor.set_zero_position(0);
    lady_brown_rotation.calibrate();
    while (1) {
        if (master.get_digital(x_button)) {
            lady_brown_motor.move_velocity(200);
        } else if (master.get_digital(r2)) {
            lady_brown_motor.move_velocity(-200);
        } else if (master.get_digital(a_button) || master.get_digital(y_button)) {
            double target = 3500.0;
            double kP = 0.1;
            double kD = 0.0325;
            double error = 0;
            while (1) {
                double error2 = error;
                double error = target - lady_brown_rotation.get_value();
                if (error < 0.5 && error > -0.5) break;
                double output = kP * error + kD * error - error2;
                lady_brown_motor.move_velocity(output);
                pros::delay(10);
            }
        } else {
            lady_brown_motor.move_velocity(0);
        }
        pros::delay(10);
    }
}