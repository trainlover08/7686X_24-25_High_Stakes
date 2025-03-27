#pragma once

#include "connor.h"
#include "constructors.hpp"
#include "refactor/intake.hpp"
#include <cmath>
//#include "op_control.hpp"

bool mut = 1;

void intake_task () {
    double v1, v2 = 0.0;
     pros::Mutex m;
     while (1) {
        v1 = v2;
        v2 = upper_intake_motor.get_actual_velocity();
        if (v2 - v1 < -1.0 && v2 < 5.0) {
            //intake.move_upper(-100);
            pros::delay(90);
        }
        if (mut) {
            if (intake_button_combo.is_pressing()) {
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
        console.clear();
        main_controller.controller->clear();
        pros::delay(50);
        std::string x_str = "X: " + std::to_string(chassis.getPose().x);
        std::string y_str = "Y: " + std::to_string(chassis.getPose().y);
        std::string t_str = "Theta: " + std::to_string(chassis.getPose().theta);
        std::string d_str = "Wheel Size: " + std::to_string((vertical_tracking_wheel.get_position() / 60000.0) / M_PI);
        console.println(d_str);
        main_controller.controller->print(0, 0, "X: %lf", chassis.getPose().x);
        pros::delay(50);
        main_controller.controller->print(1, 0, "Y: %lf", chassis.getPose().y);
        pros::delay(50);
        main_controller.controller->print(2, 0, "T: %lf", chassis.getPose().theta);
        pros::delay(150);
    }
}

void drive_task () {
    while (1) {
        int x_axis = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int y_axis = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        chassis.arcade(x_axis, y_axis);
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
    constexpr char rumble_pattern = '.'; 
    while (1) {
        if (master.get_digital(x_button)) {
            lady_brown_motor.move_velocity(200);
        } else if (master.get_digital(r2)) {
            lady_brown_motor.move_velocity(-200);
        } else if (master.get_digital(a_button) || master.get_digital(y_button)) {
            double target = 3705.0;
            double kP = 0.10;
            double kI = 0.000;
            double kD = 0.0325;
            double error = 0;
            double error_sum = 0;
            while (1) {
                double error2 = error;
                error_sum += error;
                double error = target - lady_brown_rotation.get_value();
                if (error < 1.0 && error > -1.0) {main_controller.controller->rumble(&rumble_pattern); pros::delay(50); break;}
                double output = kP * error + kD * error - error2 + kI * error_sum;
                lady_brown_motor.move_velocity(output);
                pros::delay(10);
            }
        } else {
            lady_brown_motor.move_velocity(0);
        }
        pros::delay(10);
    }
}