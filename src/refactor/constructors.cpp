/*#include "constructors.hpp"

pros::MotorGroup left_drive({front_left_drive_motor, middle_left_drive_motor, back_left_drive_motor}, pros::MotorCart::blue);
pros::MotorGroup right_drive({front_right_drive_motor, middle_right_drive_motor, back_right_drive_motor}, pros::MotorCart::blue);

pros::Motor lower_intake_motor(front_stage_intake_motor, pros::MotorCart::blue);
pros::Motor upper_intake_motor(hook_stage_intake_motor, pros::MotorCart::blue);
pros::Motor lady_brown_motor(lady_brown_motor_port, pros::MotorCart::blue);

pros::adi::Pneumatics mogo_mech_piston(mogo_mech_port, false);
pros::adi::Pneumatics doinker_piston(doinker_port, false);

pros::Rotation vertical_tracking_wheel(vertical_odom_pod_rotation_sensor);
pros::Rotation lady_brown_rotation(lady_brown_rotation_sensor);

pros::IMU imu(imu_port);

lemlib::Drivetrain drivetrain(&left_drive, &right_drive, drive_width, wheel_diameter, max_wheel_rpm, 8);

lemlib::TrackingWheel vertical_wheel(&vertical_tracking_wheel, lemlib::Omniwheel::NEW_2, vertical_tracking_offset);

lemlib::OdomSensors sensors(&vertical_wheel, nullptr, nullptr, nullptr, &imu);

lemlib::ControllerSettings lateral_controller(
    drivetrain_lateral_p_gain,                                           //kP
    drivetrain_lateral_i_gain,                                           //kI
    drivetrain_lateral_d_gain,                                           //kD
    0,                                           //anti windup
    0,                                           //small error range (inches)
    0,                                           //small error timeout (ms)
    0,                                           //large error range (inches)
    0,                                           //large error timeout (ms)
    0                                            //slew (m/s^2)
);

lemlib::ControllerSettings angular_controller(
    drivetrain_angular_p_gain,                                           //kP
    drivetrain_angular_i_gain,                                           //kI
    drivetrain_angular_d_gain,                                           //kD
    0,                                           //anti windup
    0,                                           //small error range (inches)
    0,                                           //small error timeout (ms)
    0,                                           //large error range (inches)
    0,                                           //large error timeout (ms)
    0                                            //slew (m/s^2)
);

lemlib::ExpoDriveCurve throttle_curve(
    1,                                        //joystick deadband
    0,                                        //minimum output for drivetrain
    1.0 + 0.001 * throttle_curve_intensity    //expo curve
);

lemlib::ExpoDriveCurve steer_curve(
    1,                                        //joystick deadband
    0,                                        //minimum output for drivetrain
    1.0 + 0.001 * steer_curve_intensity       //expo curve
);

lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &throttle_curve, &steer_curve);

lemlib::PID lady_brown_controller (
    lb_p_gain,                      //kP
    lb_i_gain,                      //kI
    lb_d_gain                       //kD
);

rd::Console console;

pros::Controller master(pros::E_CONTROLLER_MASTER);

Mapable_Controller main_controller(&master);

Mapable_Controller::Button_Combo intake_button (main_controller.controller, {pros::E_CONTROLLER_DIGITAL_L1});
Mapable_Controller::Button_Combo extake_button (main_controller.controller, {pros::E_CONTROLLER_DIGITAL_R1});
Mapable_Controller::Button_Combo mogo_button (main_controller.controller, {pros::E_CONTROLLER_DIGITAL_DOWN});

Intake intake(&lower_intake_motor, &upper_intake_motor);
Lady_Brown lady_brown(&lady_brown_motor, &lady_brown_rotation, &lady_brown_controller);*/