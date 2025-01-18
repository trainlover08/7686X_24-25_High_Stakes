#include "pros/misc.h"
#include "robodash/views/image.hpp"
#include "robot/color_sort.hpp"
#include "robot/controllers.hpp"

pros::Motor left_front_motor(-14, pros::MotorCart::blue);
pros::Motor left_middle_motor(-12, pros::MotorCart::blue);
pros::Motor left_back_motor(-21, pros::MotorCart::blue);

pros::Motor right_front_motor(9, pros::MotorCart::blue);
pros::Motor right_middle_motor(13, pros::MotorCart::blue);
pros::Motor right_back_motor(11, pros::MotorCart::blue);

pros::Motor lower_intake_first_stage(20, pros::MotorCart::green);
pros::Motor lower_intake_second_stage(-18, pros::MotorCart::green);
pros::Motor upper_intake(-15, pros::MotorCart::green);



pros::adi::Pneumatics intake_piston('E', false);
pros::adi::Pneumatics color_sort_piston('A', false);
pros::adi::Pneumatics redirect_piston('F', false);
pros::adi::Pneumatics mogo_mech_piston('C', false);
pros::adi::Pneumatics doinker_piston('D', false);

pros::adi::DigitalIn bumper_left('G');
pros::adi::DigitalIn bumper_right('H');

pros::Rotation vertical_tracking_wheel(19);
pros::Rotation horizontal_tracking_wheel(3);
pros::Rotation arm_rotation_sensor(1);

pros::Optical optical_sort_upper(1);
pros::Optical optical_sort_lower(1);

pros::Distance distance_sort_upper(1);
pros::Distance distance_sort_lower(1);

pros::IMU imu(17);

pros::MotorGroup left_drive_motors({left_front_motor.get_port(), left_middle_motor.get_port(), left_back_motor.get_port()}, pros::MotorCart::blue);
pros::MotorGroup right_drive_motors({right_front_motor.get_port(), right_middle_motor.get_port(), right_back_motor.get_port()}, pros::MotorCart::blue);

lemlib::Drivetrain drivetrain(&left_drive_motors, &right_drive_motors, 12.25, 3.25, 450, 8);

lemlib::TrackingWheel vertical_wheel(&vertical_tracking_wheel, lemlib::Omniwheel::NEW_2, -0.285);
lemlib::TrackingWheel horizontal_wheel(&horizontal_tracking_wheel, lemlib::Omniwheel::NEW_2, -1.694);

lemlib::OdomSensors sensors(&vertical_wheel, nullptr, &horizontal_wheel, nullptr, &imu);

lemlib::ControllerSettings lateral_controller(
    5,                                           //kP
    0,                                           //kI
    8,                                           //kD
    0,                                           //anti windup
    0,                                           //small error range (inches)
    0,                                           //small error timeout (ms)
    0,                                           //large error range (inches)
    0,                                           //large error timeout (ms)
    0                                            //slew (m/s^2)
);

lemlib::ControllerSettings angular_controller(
    2,                                           //kP
    0,                                           //kI
    12,                                           //kD
    0,                                           //anti windup
    0,                                           //small error range (inches)
    0,                                           //small error timeout (ms)
    0,                                           //large error range (inches)
    0,                                           //large error timeout (ms)
    0                                            //slew (m/s^2)
);

lemlib::ExpoDriveCurve throttle_curve(
    1,                                  //joystick deadband
    0,                                  //minimum output for drivetrain
    1.026                               //expo curve
);

lemlib::ExpoDriveCurve steer_curve(
    1,                                  //joystick deadband
    0,                                  //minimum output for drivetrain
    1.022                               //expo curve
);

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &throttle_curve, &steer_curve);

lemlib::PID arm_controller(
    0,                      //kP
    0,                      //kI
    0                       //kD
);

rd::Console console;

/*rd::Image rrwp("/usd/images/red_right_awp.bin", "Red Right AWP");
rd::Image rre("/usd/images/red_right_elim.bin", "Red Right Elims");
rd::Image rlwp("/usd/images/red_left_awp.bin", "Red Left AWP");
rd::Image rle("/usd/images/red_left_elim.bin", "Red Left Elims");
rd::Image brwp("/usd/images/blue_right_awp/bin", "Blue Right AWP");
rd::Image bre("/usd/images/blue_right_elim.bin", "Blue Right Elims");
rd::Image blwp("/usd/images/blue_left_awp.bin", "Blue Left AWP");
rd::Image ble("/usd/images/blue_left_elim.bin", "Blue Left Elims");*/

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

Mapable_Controller main_controller(&master);
Mapable_Controller alternate(&partner);

Mapable_Controller::Button_Combo move_intake(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_L1});
Mapable_Controller::Button_Combo move_intake_reverse(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R1});
Mapable_Controller::Button_Combo change_arm_macro(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_UP});
Mapable_Controller::Button_Combo doinker_button(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_L2});
Mapable_Controller::Button_Combo auto_clamp_enable_disable(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_LEFT});
Mapable_Controller::Button_Combo clamp_up_button(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_RIGHT});
Mapable_Controller::Button_Combo clamp_down_button(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_DOWN});
Mapable_Controller::Button_Combo color_sort_on(main_controller.controller,{pros::E_CONTROLLER_DIGITAL_B});
Mapable_Controller::Button_Combo color_sort_select_blue(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_Y});
Mapable_Controller::Button_Combo color_sort_select_red(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_X});
Mapable_Controller::Button_Combo cue_button(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_A});
Mapable_Controller::Button_Combo simple_extake(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R1});
Mapable_Controller::Button_Combo simple_intake(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_L1});
Mapable_Controller::Button_Combo wall_stake_macro_button_positive(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_X});
Mapable_Controller::Button_Combo wall_stake_macro_button_negative(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_Y});
Mapable_Controller::Button_Combo corner_macro_button_red_positive(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_DOWN});
Mapable_Controller::Button_Combo corner_macro_button_red_negative(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_RIGHT});
Mapable_Controller::Button_Combo corner_macro_button_blue_positive(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_UP});
Mapable_Controller::Button_Combo corner_macro_button_blue_negative(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_LEFT});
Mapable_Controller::Button_Combo color_sort_off_button(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_B});
Mapable_Controller::Button_Combo only_load_wall_stakes_toggle(main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R2, pros::E_CONTROLLER_DIGITAL_A});

Mapable_Controller::Button_Combo blue_positive_corner_macro_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_UP});
Mapable_Controller::Button_Combo blue_negative_corner_macro_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_DOWN});
Mapable_Controller::Button_Combo red_positive_corner_macro_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_RIGHT});
Mapable_Controller::Button_Combo red_negative_corner_macro_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_LEFT});
Mapable_Controller::Button_Combo view_auton_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_A});
Mapable_Controller::Button_Combo view_position_alternate(alternate.controller, {pros::E_CONTROLLER_DIGITAL_B});

Color_Sort color_sort_upper(&optical_sort_upper, &distance_sort_upper, &color_sort_piston, 0, &upper_intake);
Color_Sort color_sort_lower(&optical_sort_lower, &distance_sort_lower, nullptr, 0, &lower_intake_second_stage);

class Intake {
public:
    Intake(pros::Motor* lower, pros::Motor* upper, Color_Sort* lower_sort, Color_Sort* upper_sort) : lower(lower), upper(upper), lower_sort(lower_sort), upper_sort(upper_sort) {}

    unsigned int arm_cue_number = 0;
    unsigned int current_in_arm = 0;
    bool lock_upper_stage = 0;

    pros::Color color;

    void load_arm_sorted () {
        // check to see if the arm needs loading
        if (arm_cue_number > 0) {
            // sees if there is an object to move
            if (lower_sort->object_detected()) {
                // check the color and sort approprately
                if (color == lower_sort->get_color()) {
                    redirect_piston.extend();
                    lower->move(12000);
                    try_upper_lock();
                    upper->move(0);
                    pros::delay(1000);
                    arm_cue_number--;
                } else {
                    redirect_piston.retract();
                    lower->move(12000);
                    try_upper_lock();
                    upper->move(12000);
                }
            } else {
                try_upper_lock();
                upper->move(0);
                lower->move(0);
            }
        }
    }

    void load_arm_constant () {
        if (arm_cue_number > 0) {
            if (lower_sort->object_detected()) {
                redirect_piston.extend();
                lower->move(12000);
                try_upper_lock();
                upper->move(0);
                pros::delay(1000);
                arm_cue_number--;
            } else {
                lower->move(0);
            }
        }
    }

    void cue_ring () {
        arm_cue_number++;
    }

    void stop () {
        lower->move(0);
        try_upper_lock();
        upper->move(0);
    }

    void load_mogo_sorted () {
        if (lower_sort->object_detected()) {
            // start the ring on the path to the mogo
            load_mogo_lower();
            // spin until the ring gets picked up by the upper sensors
            while (!upper_sort->object_detected()) {
                try_upper_lock();
                upper->move(12000);
                pros::delay(100);
                // ensures proper management of multiple rings
                if (lower_sort->object_detected()) {
                    return;
                }
            }
            // color sort the ring before placing on mogo
            upper_sort->sort(color);
            // make sure to check for multiple rings
            if (lower_sort->object_detected()) {
                return;
            }
            pros::delay(500);
            try_upper_lock();
            upper->move(0);
        }
        return;
    }

    void load_mogo_constant () {
        if (lower_sort->object_detected()) {
            load_mogo_lower();
            while (!upper_sort->object_detected()) {
                try_upper_lock();
                upper->move(12000);
                pros::delay(100);
                if (lower_sort->object_detected()) {
                    return;
                }
            }
            pros::delay(500);
            try_upper_lock();
            upper->move(0);
        }
    }

    void hold_ring () {
        redirect_piston.extend();
        while (lower_sort->object_detected()) {
            lower_intake_second_stage.move(12000);
            pros::delay(50);
        }
        lower_intake_second_stage.move(0);
    }

    void unhold_ring () {
        while (!lower_sort->object_detected()) {
            lower_intake_second_stage.move(-12000);
            pros::delay(50);
        }

        while (lower_sort->object_detected()) {
            lower_intake_second_stage.move(-12000);
            pros::delay(50);
        }

        lower_intake_second_stage.move(0);
    }

private:
    pros::Mutex intake_mutex;

    pros::Motor *upper;
    pros::Motor *lower;

    Color_Sort *lower_sort;
    Color_Sort *upper_sort;

    void load_mogo_lower () {
        // set path to intake
        redirect_piston.retract();
        // make sure that the intake can be used
        try_upper_lock();
        // spins the upper intake
        upper->move(12000);
        // moves the intake until the object is no longer detected by the lower intake
        while (lower_sort->object_detected()) {
            lower->move(12000);
            pros::delay(100);
        }
        pros::delay(1000);
        lower->move(0);
    }

    void try_upper_lock () {
        while (lock_upper_stage) {
            // lock up the thread
            pros::delay(50);
        }
    }
};

class Arm {
public:
    Arm(pros::Motor *motor, pros::Rotation *rotation) : motor(motor), rotation(rotation) {}


    void move_to_position(double angle) {
        target = angle;
        error = angle - rotation->get_position();
        motor->move_velocity(arm_controller.update(error));
        pros::delay(50);
    }

    bool not_in_motion () {
        if ((target < 15) && ((error < -5) || (error < 5))) {
            return 1;
        }
        return 0;
    }
    
private:
    pros::Motor *motor;
    pros::Rotation *rotation;
    double error = 0;
    double target = 0;
};