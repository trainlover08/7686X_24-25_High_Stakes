#pragma once // this is a preprocessor directive. do not touch

// change the values after #define. should be self explanitory
#define front_left_drive_motor 1
#define middle_left_drive_motor 2
#define back_left_drive_motor 3
#define front_right_drive_motor -4
#define middle_right_drive_motor -5
#define back_right_drive_motor -6
#define front_stage_intake_motor 7
#define hook_stage_intake_motor 8
#define lady_brown_motor_port 9

#define mogo_mech_port 'A' // make sure digital ports have a set of '' or are of type char
#define doinker_port 'B'

#define vertical_odom_pod_rotation_sensor 10
#define lady_brown_rotation_sensor 11

#define imu_port 12

#define lady_brown_intake_backup_timeout 500 // milliseconds, must be whole number

#define drivetrain_lateral_p_gain 5.0 //P, I, D. Dont play with these right before match
#define drivetrain_lateral_i_gain 0.0
#define drivetrain_lateral_d_gain 3.0
#define drivetrain_angular_p_gain 2.0
#define drivetrain_angular_i_gain 0.0
#define drivetrain_angular_d_gain 12.0
#define lb_p_gain 1 //P, I, D for the lady brown controller
#define lb_i_gain 2
#define lb_d_gain 3

#define drive_width 12 // 1. find the middle of the wheel (not the length, the thickness). 2. measure the the distance from the middle of the wheels on both sides.
#define wheel_diameter 3.25
#define max_wheel_rpm 450

#define vertical_tracking_offset 5.125 // find the tracking center of the robot. this is the the distance in the vertical direction to the tracking center. negitive is aft of the tracking center
// let me know if you change the size of tracking wheels

#define throttle_curve_intensity 26
#define steer_curve_intensity 22

// @todo Find a good way to do button mapping