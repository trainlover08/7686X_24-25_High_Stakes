#pragma once

#include "pros/rotation.hpp"
#include "pros/distance.hpp"
#include "pros/colors.hpp"
#include "pros/motors.hpp"
#include "color_sort.hpp"
#include "pros/rtos.hpp"
#include <vector>

class Intake {
public:
    Intake(pros::Motor* lower_intake, pros::Motor* upper_intake, Color_Sort* lower_sort, Color_Sort* upper_sort);
    Intake(pros::Motor* lower_intake, pros::Motor* upper_intake);

    void move();
    void move(bool reverse);
    void move(bool reverse, double percentage);
    void move_lower(double percentage); 
    void move_lower(double percentage, bool reverse);
    void move_upper(double percentage);
    void move_upper(double percentage, bool reverse);
    void stop();
    void stop_lower();
    void stop_upper();
    void lb_relief();

    unsigned int lb_relief_timeout = 100;

    /*
    bool is_scheduled_motion();
    std::vector<double> hook_poses;
    void move_hook_to_ready();
    void pickup_and_hold();
    void load_lb();
    void score_ring();
    void sort_ring();*/
    
    private:
    pros::Rotation *rotation;
    pros::Distance *upper_distance;
    pros::Distance *lower_distance;
    pros::Color *upper_color;
    pros::Color *lower_color;
    pros::Motor* lower_intake;
    pros::Motor* upper_intake;
    Color_Sort* lower_sort;
    Color_Sort* upper_sort;

    //pros::Color get_color();
    //bool object_detected();
    //pros::Color object_color();
    //pros::Color last_color;
    double convert_angle_to_length (double angle);
    //void nearest_hook_to_pose(double pose);

    double diameter = 2.0f;
    double chain_length = 18.0f;
};