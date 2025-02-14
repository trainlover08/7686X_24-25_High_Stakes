#include "refactor/auto_paths.hpp"
#include "refactor/op_control.hpp"
#include "refactor/auto_control.hpp"
#include "main.h"
#include "dimentions.hpp"

void initialize() {
    chassis.setPose(0, 0, 0);
    chassis.calibrate();
}

void disabled () {

}

void competition_initialize () {

}

void autonomous() {
    red_right_goal_rush();
}

void opcontrol () {
    pre_match_red_right_elims();
    pros::Task intake_task_opcontrol (intake_task);
    pros::Task drivetrain_task_opcontrol (drive_task);
    pros::Task ui_task_opcontrol (ui_task);
    pros::Task misc_task_opcontrol (misc_task);
}