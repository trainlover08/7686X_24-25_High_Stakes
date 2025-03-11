#pragma once // this is a preprocessor directive. do not touch
#include "api.h"
using namespace pros; 

// change the values after #define. should be self explanitory
#define front_left_drive_motor -13
#define middle_left_drive_motor -14
#define back_left_drive_motor -15
#define front_right_drive_motor 12
#define middle_right_drive_motor 17
#define back_right_drive_motor 16
#define front_stage_intake_motor 20
#define hook_stage_intake_motor -8
#define lady_brown_motor_port 7

#define mogo_mech_port 'D' // make sure digital ports have a set of '' or are of type char
#define doinker_port 'F'
#define intake_piston_port 'E'

#define vertical_odom_pod_rotation_sensor 10
#define lady_brown_pot 'G'

#define imu_port 11

#define lady_brown_ready_macro 3850
#define lady_brown_load_macro 15
#define lady_brown_score_macro 180

#define drivetrain_lateral_p_gain 10.5 //P, I, D. Dont play with these right before match
#define drivetrain_lateral_i_gain 0.0
#define drivetrain_lateral_d_gain 70.0
#define drivetrain_angular_p_gain 2.0
#define drivetrain_angular_i_gain 0.1
#define drivetrain_angular_d_gain 28.0
#define lb_ready_p_gain 0.00 //P, I, D for the lady brown controller
#define lb_ready_i_gain 0.0
#define lb_ready_d_gain 0.100
#define lb_score_p_gain 2.5
#define lb_score_i_gain .2
#define lb_score_d_gain 15
#define lb_return_p_gain .075
#define lb_return_i_gain .01
#define lb_return_d_gain 0.75

#define lady_brown_p 1.4 // this is the value you change to control the firmness of the motor
#define lady_brown_load_position 30 // this is what you change to change the load position of the lb. its in percents of hte rotation sensor

#define drive_width 12.25 // 1. find the middle of the wheel (not the length, the thickness). 2. measure the the distance from the middle of the wheels on both sides.
#define wheel_diameter 3.25
#define max_wheel_rpm 450

#define vertical_tracking_offset -0.875 // find the tracking center of the robot. this is the the distance in the vertical direction to the tracking center. negitive is aft of the tracking center
// let me know if you change the size of tracking wheels

#define throttle_curve_intensity 26
#define steer_curve_intensity 22

#define SORT_MACRO 330
 
// quick-guide does not support button combos or multiple controllers by default
#define doinker_button l2
#define mogo_button b_button
#define intake_button r1
#define extake_button l1

#define lady_brown_down_button a_button // change macro for now
#define lady_brown_que_button y_button
#define lady_brown_up_button r2 // change macro for now

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