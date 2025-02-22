#pragma once // this is a preprocessor directive. do not touch
#include "api.h"
using namespace pros; 

// change the values after #define. should be self explanitory
#define front_left_drive_motor -3
#define middle_left_drive_motor -14
#define back_left_drive_motor -17
#define front_right_drive_motor 11
#define middle_right_drive_motor 18
#define back_right_drive_motor 15
#define front_stage_intake_motor 20
#define hook_stage_intake_motor -5
#define lady_brown_motor_port 4

#define mogo_mech_port 'G' // make sure digital ports have a set of '' or are of type char
#define doinker_port 'D'
#define intake_piston_port 'E'

#define vertical_odom_pod_rotation_sensor 9
#define lady_brown_pot 'H'

#define imu_port 2

#define lady_brown_ready_macro 3750
#define lady_brown_load_macro 3580
#define lady_brown_score_macro 2100

#define drivetrain_lateral_p_gain 10.0 //P, I, D. Dont play with these right before match
#define drivetrain_lateral_i_gain 0.0
#define drivetrain_lateral_d_gain 60.0
#define drivetrain_angular_p_gain 2.0
#define drivetrain_angular_i_gain 0.1
#define drivetrain_angular_d_gain 26.0
#define lb_ready_p_gain 0.00 //P, I, D for the lady brown controller
#define lb_ready_i_gain 0.0
#define lb_ready_d_gain 0.100
#define lb_score_p_gain 2.5
#define lb_score_i_gain .2
#define lb_score_d_gain 15
#define lb_return_p_gain .075
#define lb_return_i_gain .01
#define lb_return_d_gain 0.75

#define drive_width 12.25 // 1. find the middle of the wheel (not the length, the thickness). 2. measure the the distance from the middle of the wheels on both sides.
#define wheel_diameter 3.25
#define max_wheel_rpm 450

#define vertical_tracking_offset -0.875 // find the tracking center of the robot. this is the the distance in the vertical direction to the tracking center. negitive is aft of the tracking center
// let me know if you change the size of tracking wheels

#define throttle_curve_intensity 26
#define steer_curve_intensity 22

// quick-guide does not support button combos or multiple controllers by default
#define doinker_button l2
#define mogo_button x_button
#define intake_button r1
#define extake_button l1
#define lady_brown_down_button r2 // change macro for now
#define lady_brown_que_button y_button

// generally don't touch things below this line
#define l2 E_CONTROLLER_DIGITAL_L2
#define l1 E_CONTROLLER_DIGITAL_L1
#define r2 E_CONTROLLER_DIGITAL_R2
#define r1 E_CONTROLLER_DIGITAL_R1
#define x_button E_CONTROLLER_DIGITAL_X
#define a_button E_CONTROLLER_DIGITAL_A
#define b_button E_CONTROLLER_DIGITAL_B
#define y_button E_CONTROLLER_DIGITAL_Y
#define up E_CONTROLLER_DIGITAL_UP
#define down E_CONTROLLER_DIGITAL_DOWN
#define left E_CONTROLLER_DIGITAL_LEFT
#define right E_CONTROLLER_DIGITAL_RIGHT