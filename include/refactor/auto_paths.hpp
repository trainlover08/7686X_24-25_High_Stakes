#pragma once

#include "constructors.hpp"
#include "lemlib/api.hpp"
#include "pros/apix.h"
#include "dimentions.hpp"

using namespace dimensions::all;

void red_right_goal_rush () {
    chassis.setPose(-TILE * 2.0 - DRIVE_LENGTH / 2.0, -TILE - DRIVE_WIDTH / 2.0, RED_STATION);
    chassis.moveToPoint(-TILE * 1.5, -TILE - (1 / 3) * TILE, 2000, {.forwards=0});
    chassis.turnToPoint(-TILE, -TILE, 500, {.forwards=0});
    chassis.moveToPoint(-TILE, -TILE, 2000, {.forwards=0}, 0);
    pros::delay(250);
    mogo_mech_piston.extend();
    intake.move_upper();
    pros::delay(500);
    chassis.moveToPoint(-TILE, -TILE * 2, 2000, {});
    intake.move_lower();
    intake.stop_upper();
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE, -TILE * 2, 2000, {});
    chassis.turnToPoint(-MOGO_CORNER, -TILE * 2, 500, {.forwards=0});
    chassis.moveToPoint(-MOGO_CORNER - DRIVE_LENGTH / 2, -TILE * 2, 2000, {.forwards=0}, 0);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPoint(-TILE - MOGO_CORNER - DRIVE_LENGTH / 2, -TILE * 2, 2000, {}, 0);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE, -TILE * 2, 1000, {.forwards=0}, 0);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPoint(TILE * -2.5, TILE * -2, 2000, {});
    intake.move();
    pros::delay(500);
    intake.stop_lower();
    chassis.turnToPoint(-TILE * 3 + RING / 2.0 + DRIVE_WIDTH / 2.0, -TILE * 3 + RING / 2 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, 500);
    chassis.moveToPose(-TILE * 3 + RING / 2.0 + DRIVE_WIDTH / 2.0, -TILE * 3 + RING / 2 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, AUDIENCE, 2000, {}, false);
    doinker_piston.extend();
    pros::delay(500);
    chassis.moveToPoint(-TILE * 2, -TILE * 2, 2000, {.forward=false}, false);
    doinker_piston.retract();
    pros::delay(250);
    chassis.moveToPose(-DRIVE_LENGTH / 2 - 2, -TILE * 3 + 2 + DRIVE_WIDTH / 2, BLUE_STATION, 5000, {});
}

void pre_match_red_right_elims () {
    chassis.moveToPose(TILE * 2, DRIVE_WIDTH / 2 + 2 - TILE * 3, BLUE_STATION, 5000, {.minSpeed=72, .earlyExitRange=6}, 0);
};