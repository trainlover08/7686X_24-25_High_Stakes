#include "robot/autos.cpp"
#include "main.h"

void initialize() {
    selector.focus();
    main_controller.selector = &selector;
    alternate.selector = &selector;
    chassis.calibrate();
}

void disabled () {

}

void competition_initialize () {

}

void autonomous() {
    lower_intake_second_stage.set_voltage_limit(7000);
    //upper_intake.set_voltage_limit(8000);
    selector.run_auton();
}

void opcontrol () {
    lower_intake_second_stage.set_voltage_limit(7000);
    //upper_intake.set_voltage_limit(8000);
    //red_right_awp();

    //pros::Task arm_task(arm_control_opcontrol);
    //pros::Task intake_task(intake_control_opcontrol);
    pros::Task drive_task(drivetrain_control_opcontrol);
    pros::Task misc_task(misc_control_opcontrol);
    pros::Task ui_task(user_interface_opcontrol);
    pros::Task macros_task(macros_opcontrol);

    pros::Task simple_intake_task(test_simple_intake);
}