#include "op_control.hpp"
//#include "main.h"
#include "dimentions.hpp"

void initialize() {
    chassis.calibrate();
}

void disabled () {

}

void competition_initialize () {

}

void autonomous() {

}

void opcontrol () {
    pros::Task intake_task_opcontrol (intake_task);
    pros::Task drivetrain_task_opcontrol (drive_task);
    pros::Task ui_task_opcontrol (ui_task);
    pros::Task misc_task_opcontrol (misc_task);
}