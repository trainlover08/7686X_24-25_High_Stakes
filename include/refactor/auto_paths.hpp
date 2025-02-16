#pragma once

#include "constructors.hpp"
#include "lemlib/api.hpp"
#include "pros/apix.h"
#include "dimentions.hpp"

using namespace dimensions::all;

constexpr double DOINKER_LENGTH = 5.5;

void check_time () {
    unsigned int time0 = pros::millis();
    while (1) {
        unsigned int time1 = pros::millis();
        if ((time1 - time0) > 14500) {
            chassis.cancelAllMotions();
            break;
        }
        pros::delay(10);
    }
    chassis.moveToPose(chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta, 1000, {}, 0);
}

void red_right () {
    chassis.setPose(-TILE * 2.0 - DRIVE_LENGTH / 2.0, -TILE - DRIVE_WIDTH / 2.0, RED_STATION);
    mogo_mech_piston.retract();
    chassis.moveToPoint(statics::match::MOGO_RIGHT_RED.first - MOGO_CORNER, statics::match::MOGO_RIGHT_RED.second - MOGO_FLATEDGE, 2000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5});
    chassis.turnToPoint(statics::match::MOGO_RIGHT_RED.first, statics::match::MOGO_RIGHT_RED.second, 500, {.forwards=0});
    chassis.moveToPoint(statics::match::MOGO_RIGHT_RED.first, statics::match::MOGO_RIGHT_RED.second, 2000, {.forwards=0}, 0);
    //pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(500);
    intake.move_upper();
    pros::delay(500);
    chassis.moveToPoint(-TILE, -TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    pros::delay(500);
    intake.move_lower();
    intake.stop_upper();
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE, -TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint((statics::match::MOGO_GOAL_RUSH.first - MOGO_CORNER) + 1, statics::match::MOGO_GOAL_RUSH.second, 500, {.forwards=0});
    chassis.moveToPose((statics::match::MOGO_GOAL_RUSH.first - MOGO_CORNER) + 1, statics::match::MOGO_GOAL_RUSH.second, RED_STATION, 4000, {.forwards=0}, 0);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(750);
    intake.move_upper();
    chassis.moveToPoint(TILE * -2.5, TILE * -2, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move_lower(100, false);
    pros::delay(500);
    chassis.turnToPoint(-TILE * 3 + RING / 2.0 + DRIVE_WIDTH / 2.0, -TILE * 3 + RING / 2 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, 500, {}, 0);
    doinker_piston.extend();
    intake.stop_upper();
    pros::delay(500);
    intake.stop_lower();
    chassis.moveToPose(-TILE * 3 + DRIVE_WIDTH / 2.0 + 0.5, -TILE * 3 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, AUDIENCE, 2000, {}, false);
    intake.move_lower(100, false);
    pros::delay(500);
    chassis.moveToPoint(-TILE * 2, -TILE * 2, 2000, {.forwards=false}, false);
    doinker_piston.retract();
}

void blue_left () {
    chassis.setPose(TILE * 2.0 + DRIVE_LENGTH / 2.0, -TILE - DRIVE_WIDTH / 2.0, BLUE_STATION);
    mogo_mech_piston.retract();
    chassis.moveToPoint(statics::match::MOGO_LEFT_BLUE.first + MOGO_CORNER, statics::match::MOGO_RIGHT_RED.second - MOGO_FLATEDGE, 2000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5});
    chassis.turnToPoint(statics::match::MOGO_LEFT_BLUE.first, statics::match::MOGO_RIGHT_RED.second, 500, {.forwards=0});
    chassis.moveToPoint(statics::match::MOGO_LEFT_BLUE.first, statics::match::MOGO_RIGHT_RED.second, 2000, {.forwards=0}, 0);
    //pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(500);
    intake.move_upper();
    pros::delay(500);
    chassis.moveToPoint(TILE, -TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    pros::delay(500);
    intake.move_lower();
    intake.stop_upper();
    mogo_mech_piston.retract();
    chassis.moveToPoint(TILE, -TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint((statics::match::MOGO_GOAL_RUSH.first + MOGO_CORNER) - 1, statics::match::MOGO_GOAL_RUSH.second, 500, {.forwards=0});
    chassis.moveToPose((statics::match::MOGO_GOAL_RUSH.first + MOGO_CORNER) - 1, statics::match::MOGO_GOAL_RUSH.second, BLUE_STATION, 4000, {.forwards=0}, 0);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(750);
    intake.move_upper();
    chassis.moveToPoint(TILE * 2.5, TILE * -2, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.stop_lower();
    pros::delay(1000);
    //chassis.turnToPoint(TILE * 3 - RING / 2.0 + DRIVE_WIDTH / 2.0, -TILE * 3 + RING / 2 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, 500, {}, 0);
    //doinker_piston.extend();
    intake.stop_upper();
    intake.move_lower();
    chassis.moveToPose(TILE * 3 - DRIVE_WIDTH / 2.0 - 0.5, -TILE * 3 + DRIVE_LENGTH / 2 + DOINKER_LENGTH, AUDIENCE, 2000, {}, false);
    intake.move_lower(100, false);
    pros::delay(500);
    chassis.moveToPoint(TILE * 2, -TILE * 2, 2000, {.forwards=false}, false);
    doinker_piston.retract();
}

void red_right_goal_rush () {
    pros::Task time_check(check_time);
    red_right();
    chassis.moveToPose(-DRIVE_LENGTH / 2 - 2, -TILE * 3 + 4 + DRIVE_WIDTH / 2, BLUE_STATION, 5000, {});
}

void red_right_awp () {
    red_right();
    chassis.moveToPose(-TILE, -TILE, BLUE_STATION, 2000, {.minSpeed=1, .earlyExitRange=2});
    chassis.moveToPoint(-DRIVE_LENGTH, -TILE, 2000, {});
}

void blue_left_goal_rush () {
    pros::Task time_check(check_time);
    blue_left();
    chassis.moveToPose(DRIVE_LENGTH / 2 + 2, -TILE * 2.5 + 4 + DRIVE_WIDTH / 2, RED_STATION, 5000, {});
    pros::delay(500);
    doinker_piston.extend();
}

void pre_match_red_right_elims () {
    doinker_piston.extend();
    chassis.moveToPose(TILE * 3 - DRIVE_LENGTH / 2 - DOINKER_LENGTH, DRIVE_WIDTH / 2 + 0.5 - TILE * 3, BLUE_STATION, 5000, {.minSpeed=72, .earlyExitRange=6}, 0);
};

void pre_match_blue_left_elims () {
    chassis.moveToPose(-TILE * 2, DRIVE_WIDTH / 2 + 0.5 - TILE * 3, RED_STATION, 5000, {.minSpeed=72, .earlyExitRange=6}, 0);
};

void skills () {
    intake.move();
}