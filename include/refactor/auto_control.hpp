#pragma once

#include "auto_paths.hpp"
#include "robodash/apix.h"

rd::Selector selector({});

void cancel_auto_motion () {
    while (1) {
        if (master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_X) > 50 || master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y) > 50 || master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X) || master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y)) chassis.cancelAllMotions();
        pros::delay(50);
    }
}