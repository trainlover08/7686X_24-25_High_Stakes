#pragma once

#include "auto_paths.hpp"
#include "pros/misc.h"

rd::Selector selector({{"Red Right", red_right_goal_rush, "/usd/images/red_right_elim.bin"}, {"Blue AWP", awp_blue, "/usd/images/blue_left_awp.bin"}, {"Red AWP", awp_red, "/usd/images/red_left_awp.bin"}});

void cancel_auto_motion () {
    while (1) {
        if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 50) chassis.cancelAllMotions();
        pros::delay(50);
    }
}