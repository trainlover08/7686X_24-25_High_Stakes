#include "refactor/auto_paths.hpp"
#include "refactor/op_control.hpp"
#include "refactor/auto_control.hpp"
#include "main.h"

bool auto_ran = 0;

void initialize() {
    chassis.setPose(0, 0, 0);
    selector.focus();
    chassis.calibrate();
}

void disabled () {

}

void competition_initialize () {

}

void autonomous() {
    selector.run_auton();
    auto_ran = 1;
}

void opcontrol () {
    pros::Task auto_cancel(cancel_auto_motion);
    if (auto_ran) {
        if (selector.get_auton()->name == "Red Right") {
            pre_match_red_right_elims();
        } else if (selector.get_auton()->name == "Blue Left") {
            pre_match_blue_left_elims();
        }
    }
    pros::Task intake_task_opcontrol (intake_task);
    pros::Task drivetrain_task_opcontrol (drive_task);
    pros::Task ui_task_opcontrol (ui_task);
    pros::Task misc_task_opcontrol (misc_task);
}