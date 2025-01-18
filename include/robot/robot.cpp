#include "pros/misc.h"
#include "robot_constructors.cpp"

Arm arm(&upper_intake, &arm_rotation_sensor);
Intake intake(&lower_intake_second_stage, &upper_intake, &color_sort_lower, &color_sort_upper);

bool is_in_current_macro = 0;
pros::Color color_sort_color;
bool color_sort_on_off = 1;
bool arm_load_lockout = 0;
bool mogo_load_lockout = 0;
bool intake_move_lockout = 0;
bool arm_down = 1;

void hold_until_unlock () {
    while (arm_load_lockout) {
        intake.hold_ring();
        pros::delay(50);
    }
    intake.unhold_ring();
}

void arm_control_opcontrol () {
    while(1) {
        double set_arm_angle = 15;

        if (change_arm_macro.is_pressing()) {
            if (set_arm_angle == 15) {
                set_arm_angle = 90;
            } else {
                set_arm_angle = 15;
            }
            while (change_arm_macro.is_pressing()) {
                pros::delay(50);
            }
        }

        if (arm.not_in_motion()) {
            arm_down = 1;
        } else if (!intake_move_lockout) {
            arm_down = 0;
            arm.move_to_position(set_arm_angle);
        }

        pros::delay(10);
    }
}

void intake_control_opcontrol () {
    while(!0) {
        int intake_lower_first_stage_voltage = 0;

        if (move_intake.is_pressing()) {
            intake_lower_first_stage_voltage += 12000;
        }

        if (move_intake_reverse.is_pressing()) {
            intake_lower_first_stage_voltage -= 12000;
        }

        lower_intake_first_stage.move(intake_lower_first_stage_voltage);

        if (color_sort_select_blue.is_pressing()) {
            color_sort_color = pros::Color::blue;
        }
        
        if (color_sort_select_red.is_pressing()) {
            color_sort_color = pros::Color::red;
        }

        if (color_sort_on.is_pressing()) {
            color_sort_on_off = 1;
        }

        if (color_sort_off_button.is_pressing()) {
            color_sort_on_off = 0;
        }

        if (cue_button.is_pressing()) {
            intake.cue_ring();
        }

        intake.color = color_sort_color;

        if (only_load_wall_stakes_toggle.is_pressing()) {
            if (arm_down) {
                if (color_sort_on_off == 0) {
                    if (intake.arm_cue_number == 0) {
                        intake.arm_cue_number = 1;
                    }
                    intake_move_lockout = 1;
                    intake.load_arm_constant();
                    intake_move_lockout = 0;
                } else {
                    if (intake.arm_cue_number == 0){
                        intake.arm_cue_number = 1;
                    }
                    intake_move_lockout = 1;
                    intake.load_arm_sorted();
                    intake_move_lockout = 0;
                }
            }
        } else if (simple_intake.is_pressing()) {
            redirect_piston.retract();
            intake_move_lockout = 1;
            lower_intake_first_stage.move(12000);
            lower_intake_second_stage.move(12000);
            pros::delay(50);
            intake_move_lockout = 0;
        } else if (simple_extake.is_pressing()) {
            redirect_piston.retract();
            intake_move_lockout = 1;
            lower_intake_first_stage.move(-12000);
            lower_intake_second_stage.move(-12000);
            pros::delay(50);
            intake_move_lockout = 0;
        } else if (intake.arm_cue_number != 0) {
            if (arm_down) {
                if (color_sort_on_off == 0) {
                    hold_until_unlock();
                    intake_move_lockout = 1;
                    intake.load_arm_constant();
                    intake_move_lockout = 0;
                } else {
                    hold_until_unlock();
                    intake_move_lockout = 1;
                    intake.load_arm_sorted();
                    intake_move_lockout = 0;
                }
            }
        } else {
            if (color_sort_on_off == 0) {
                hold_until_unlock();
                intake_move_lockout = 1;
                intake.load_mogo_constant();
                intake_move_lockout = 0;
            } else {
                hold_until_unlock();
                intake_move_lockout = 1;
                intake.load_mogo_sorted();
                intake_move_lockout = 0;
            }
        }

        pros::delay(10);
    }
}

void drivetrain_control_opcontrol () {
    while (!0) {
        if (!is_in_current_macro) {
            int controller_y = main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int controller_x = main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

            if ((controller_x < 5) || (controller_y < 5)) {
                chassis.arcade(controller_y, controller_x);
            } else {
                chassis.curvature(controller_y, controller_x);
            }
        }

        if ((main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) > 50) || (main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y) > 50) || (main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 50) || (main_controller.controller->get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) > 50)) {
            is_in_current_macro = 0;
            chassis.cancelAllMotions();
        }
        
        pros::delay(10);
    }
}

void misc_control_opcontrol () {
    bool auto_clamp_enable = 1;
    while (!0) {
        doinker_piston.set_value(doinker_button.is_pressing());

        if ((clamp_down_button.is_pressing())) {
            mogo_mech_piston.extend();
        }

        if (clamp_up_button.is_pressing()) {
            mogo_mech_piston.retract();
        }

        pros::delay(10);
    }
}

void user_interface_opcontrol () {
    unsigned int time_start = 105000;
    while (1) {
        unsigned int time_left = time_start - pros::millis();
        time_left = time_left <= 0 || time_left > 105000 ? 0 : time_left;

        main_controller.print_time(time_left);

        lemlib::Pose pose = chassis.getPose();

        if (view_position_alternate.is_pressing()) {
            alternate.print_field_position(pose.x, pose.y, pose.theta);
        } else if (view_auton_alternate.is_pressing()) {
            alternate.print_current_auton();
        } else {
            alternate.print_time(time_left);
        }

        if (time_left > 12000 && time_left < 15000) {
            main_controller.controller->rumble("-");
        }

        pros::delay(50);
    }
}

void macros_opcontrol () {

    while (1) {
        if (corner_macro_button_red_positive.is_pressing() || red_positive_corner_macro_alternate.is_pressing()) {
            is_in_current_macro = 1;
            chassis.moveToPoint(-36, -36, 5000, {.earlyExitRange = 4}, false);
        } else if (corner_macro_button_blue_positive.is_pressing() || blue_positive_corner_macro_alternate.is_pressing()) {
            is_in_current_macro = 1;
            chassis.moveToPoint(36, -36, 5000, {.earlyExitRange = 4}, false);
        } else if (corner_macro_button_red_negative.is_pressing() || red_negative_corner_macro_alternate.is_pressing()) {
            is_in_current_macro = 1;
            chassis.moveToPoint(-36, 36, 5000, {.earlyExitRange = 4}, false);
        } else if (corner_macro_button_blue_negative.is_pressing() || blue_negative_corner_macro_alternate.is_pressing(), false) {
            is_in_current_macro = 1;
            chassis.moveToPoint(36, 36, 5000, {.earlyExitRange = 4}, false);
        } else if (wall_stake_macro_button_positive.is_pressing()) {
            is_in_current_macro = 1;
            chassis.moveToPose(0, -70+8, 180, 5000, {.earlyExitRange = 0.25}, false);
        } else if (wall_stake_macro_button_negative.is_pressing()) {
            is_in_current_macro = 1;
            chassis.moveToPose(0, 70-8, 0, 5000, {.earlyExitRange = 0.25}, false);
        } else {
            is_in_current_macro = 0;
            chassis.cancelAllMotions();
        }

        pros::delay(10);
    }
}

void test_simple_intake () {
    while (1) {
        int intake_voltage = 0;
        if (simple_intake.is_pressing()) {
            intake_voltage += 12000;
        }

        if (simple_extake.is_pressing()) {
            intake_voltage -= 12000;
        }

        lower_intake_first_stage.move(intake_voltage);
        lower_intake_second_stage.move(intake_voltage);
        upper_intake.move(intake_voltage);

        pros::delay(50);
    }
}