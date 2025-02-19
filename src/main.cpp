#include "refactor/auto_paths.hpp"
#include "refactor/op_control.hpp"
#include "refactor/auto_control.hpp"
#include "main.h"

bool auto_ran = 0;

void initialize() {
    chassis.setPose(0, 0, 0);
    selector.focus();
    chassis.calibrate();
    lady_brown_rotation.calibrate();
}

void disabled () {

}

void competition_initialize () {

}

void autonomous() {
    auto_ran = 1;
    selector.run_auton();
}

void opcontrol () {
    chassis.setPose(0, 0, 0);
    while (1) {
        //chassis.moveRelative(60, 5000);
        //chassis.moveRelative(-60, 5000, true);
        chassis.moveToPoint(0, 48, 5000);
        chassis.moveToPoint(0, 0, 5000, {.forwards=false});
    }
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
    pros::Task lady_brown_intake_task_opcontrol (lady_brown_intake_task);
}