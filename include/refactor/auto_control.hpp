#pragma once

#include "auto_paths.hpp"
#include "pros/misc.h"

rd::Selector selector({{"Red Negative Elims", red_neg_sweep, "/usd/images/red_left_elim.bin"}, {"Red Negative AWP", red_neg_awp, "/usd/images/red_left_awp.bin"}, {"Blue Negative Elims", blue_neg_sweep, "/usd/images/blue_left_elim.bin"}, {"Blue Negative AWP", blue_neg_awp, "/usd/images/blue_right_awp.bin"}, {"Blue Positive Sweep", blue_pos_sweep, "/usd/images/blue_left_elim.bin"}, {"Blue Positive AWP", blue_pos_awp, "/usd/images/blue_left_awp.bin"}, {"Blue Positive Wallstake", blue_pos_cover, "/usd/images/blue_left_elim.bin"}});

void cancel_auto_motion () {
    while (1) {
        if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 50) chassis.cancelAllMotions();
        pros::delay(50);
    }
}