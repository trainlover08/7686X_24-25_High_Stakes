#pragma once

#include "constructors.hpp"
#include "dimentions.hpp"

using namespace dimensions::all;

constexpr double DOINKER_LENGTH = 5.5;


void example_auto () {
    chassis.setPose(-48, 0, 90); // set the start position to 2 tiles left, centered on red side. Facing blue station
    mogo_mech_piston.retract(); // start with the clamp up.
    chassis.moveRelative(12, 2000); // move forward 12 tiles. Allows 2000 milliseconds for the movement.
    intake.move_lower(); // spin the lower intake at full speed forward while the drive is moving.
    chassis.turnRelative(90, 1000); // turn 90 degrees right. Allows 1 second for movement.
    intake.move_upper(50, false); // move the upper hooks in the opposite direction at half power at the same time as preforming the turn.
    chassis.moveRelative(-24, 5000, true); // move forward 24 tiles. Allows 2000 milliseconds for the movement. Backup.
    intake.stop(); // stop both stages of the intake while moving backwards.
    chassis.moveRelative(-6, 2000, true, false); // move back 6 inches ending with the back to the mogo. Do not execute the following code after the motion finished.
    mogo_mech_piston.extend(); // extend the clamp.
    chassis.turnRelative(-90, 1000, true, false); // put the back of the robot 90 degrees to the left. Wait for the motion to finish before dropping the mogo.
    mogo_mech_piston.retract();
}

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