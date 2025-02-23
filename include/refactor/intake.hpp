#pragma once

#include "pros/rotation.hpp"
#include "pros/distance.hpp"
#include "pros/colors.hpp"
#include "pros/motors.hpp"
#include "pros/adi.hpp"
#include "color_sort.hpp"
#include "lemlib/pid.hpp"
#include <vector>

class Intake {
public:
    Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, pros::adi::Pneumatics* raiser, Color_Sort* lower_sort, pros::Rotation* rotation);
    Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, pros::adi::Pneumatics* raiser);

    void move();
    void move(bool reverse);
    void move(bool reverse, double percentage);
    void move_lower();
    void move_lower(double percentage); 
    void move_lower(double percentage, bool reverse);
    void move_upper();
    void move_upper(double percentage);
    void move_upper(double percentage, bool reverse);
    void stop();
    void stop_lower();
    void stop_upper();
    void lb_relief();

    void raise_intake (bool extend);

    std::vector<double> hook_poses {0, 0, 0};
    void move_hook_to_ready();
    void pickup_and_hold();
    void load_lb();
    void score_ring();
    void sort_ring();
    double ready_pose;
    double load_lb_pose;
    double sort_stop_pose;
    double lb_backup_pose;

    double kP, kI, kD = 0.0;
    
    private:
    pros::Rotation *rotation;
    pros::Distance *upper_distance;
    pros::Distance *lower_distance;
    pros::Color *upper_color;
    pros::Color *lower_color;
    pros::Motor* lower_intake;
    pros::Motor* upper_intake;
    pros::adi::Pneumatics* raiser;
    Color_Sort* lower_sort;
    Color_Sort* upper_sort;
    pros::Mutex mutex;

    pros::Color get_color();
    bool object_detected();
    pros::Color object_color();
    pros::Color last_color;
    double convert_angle_to_length (double angle);
    void update_poses ();
    void nearest_hook_to_pose(double pose, bool reverse=0);

    double diameter = 2.0f;
    double chain_length = 18.0f;

    lemlib::PID* pid;
};