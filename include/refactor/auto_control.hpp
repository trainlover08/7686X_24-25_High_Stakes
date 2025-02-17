#pragma once

#include "auto_paths.hpp"
#include "pros/misc.h"

rd::Selector selector({{"Red Right", red_right_goal_rush}});

void cancel_auto_motion () {
    while (1) {
        if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 50) chassis.cancelAllMotions();
        pros::delay(50);
    }
}