#pragma once

#include "constructors.hpp"
#include "dimentions.hpp"
#include "op_control.hpp"

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

ASSET(one_txt);
ASSET(two_txt);
ASSET(three_txt);
ASSET(four_txt);

void skills () {
    chassis.setPose(-62.7, -4.9, 90);
    pros::Task lb_task(lady_brown_intake_task);
    lady_brown.move_to_position(hold);
    intake.move_upper();
    pros::delay(500);
    chassis.moveToPoint(-TILE * 2, 0, 5000, {.minSpeed=31, .earlyExitRange=3}, false);
    intake.stop();
    mogo_mech_piston.retract();
    chassis.turnToHeading(REFEREE, 500);
    chassis.moveToPose(-TILE * 2, -TILE + MOGO_FLATEDGE, REFEREE, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-24 * 2, -TILE, REFEREE, 2000, {.forwards=false, .maxSpeed=31}, false);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.turnToPoint(-TILE, -TILE, 500);
    intake.move();
    //chassis.moveToPoint(-TILE, -TILE, 2000, {.minSpeed=1, .earlyExitRange=1});
    //chassis.moveToPoint(0, -TILE * 2, 5000, {.minSpeed=16, .earlyExitRange=3});
    //chassis.moveToPoint(TILE, -TILE * 2, 5000, {.minSpeed=1, .earlyExitRange=1});
    //chassis.turnToPoint(0, -TILE * 2.5, 500);
    //chassis.moveToPose(0, -TILE * 2.5, RED_STATION, 5000, {.minSpeed=16, .earlyExitRange=1});
    //chassis.moveToPose(-TILE, -TILE * 2, RED_STATION, 5000, {.minSpeed=16, .earlyExitRange=1});
    chassis.follow(one_txt, 6, 15000);
    chassis.moveToPoint(-TILE * 2, -TILE * 2, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=.5});
    chassis.turnToPoint(-TILE * 2, -TILE * 2.5 + DRIVE_LENGTH / 2 - RING / 2, 500, {}, false);
    intake.stop_upper();
    chassis.moveToPoint(-TILE * 2, -TILE * 2.5 + DRIVE_LENGTH / 2 - RING / 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-TILE * 2, -TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(45, 500);
    chassis.moveToPose(-58, -58, 45, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE * 2, -TILE * 2, 2000, {.minSpeed=31, .earlyExitRange=3}, false);
    intake.move();
    lady_brown.move_to_position(load);
    chassis.moveToPoint(0, -42, 5000, {.minSpeed=31, .earlyExitRange=4});
    pros::delay(500);
    intake.stop();
    lady_brown.move_to_position(score);
    chassis.moveToPose(0, -TILE * 3 + DRIVE_LENGTH / 2, AUDIENCE, 5000, {.minSpeed=1, .earlyExitRange=3}, false);
    lady_brown.move_to_position(hold);
    chassis.moveToPoint(0, -TILE * 2, 2000, {.forwards=false, .minSpeed=16, .earlyExitRange=2});
    chassis.turnToPoint(TILE, -TILE, 500);
    chassis.moveToPoint(TILE, -TILE, 5000, {.minSpeed=15, .earlyExitRange=2});
    chassis.turnToHeading(240, 500);
    chassis.moveToPose(TILE * 2 - MOGO_CORNER / 2.0, -MOGO_FLATEDGE / 2.0, 210, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=2});
    chassis.moveToPoint(TILE * 2, 0, 2000, {.forwards=false, .maxSpeed=31, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.follow(two_txt, 15, 10000);
    intake.move();
    chassis.turnToPoint(-TILE * 2, TILE *2, 500);
    chassis.moveToPoint(-TILE * 2, TILE * 2, 5000, {.minSpeed=16, .earlyExitRange=2});
    chassis.moveToPoint(-TILE * 2.5 + DRIVE_LENGTH / 2 - RING / 2, TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-TILE * 2, TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(-TILE * 2, TILE * 2.5, 500);
    chassis.moveToPoint(-TILE * 2, TILE * 2.5 - DRIVE_LENGTH / 2 + RING / 2, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    lady_brown.move_to_position(load);
    chassis.moveToPoint(-TILE * 2, TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(135, 500);
    chassis.moveToPose(-58, 58, 135, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE * 2, TILE * 2, 2000, {.minSpeed=31, .earlyExitRange=3}, false);
    intake.stop_upper();
    lady_brown.move_to_position(score);
    chassis.moveToPoint(0, 42, 5000, {.minSpeed=31, .earlyExitRange=4});
    chassis.moveToPose(0, TILE * 3 - DRIVE_LENGTH / 2, REFEREE, 5000, {.minSpeed=1, .earlyExitRange=3}, false);
    lady_brown.move_to_position(hold);
    intake.move();
    chassis.moveToPoint(0, TILE * 2, 2000, {.forwards=false, .minSpeed=16, .earlyExitRange=2});
    chassis.turnToHeading(90, 500);
    chassis.moveToPoint(TILE, TILE * 2, 5000, {.minSpeed=16, .earlyExitRange=2});
    chassis.moveToPoint(TILE * 2.5, TILE * 2, 5000, {.minSpeed=15, .earlyExitRange=2}, false);
    intake.stop_lower();
    chassis.moveToPoint(TILE * 2, TILE * 2, 2000, {.minSpeed=1, .earlyExitRange=2});
    chassis.turnToPoint(TILE * 2, TILE * 2.5, 500);
    chassis.moveToPoint(TILE * 2, TILE * 2.5 - DRIVE_LENGTH / 2 + RING / 2, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move();
    chassis.moveToPoint(TILE * 2, TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(225, 500);
    chassis.moveToPose(58, 58, 225, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    mogo_mech_piston.retract();
    chassis.follow(three_txt, 15, 15000);
    intake.stop_upper();
    chassis.moveToPoint(TILE * 2, -TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1});
    upper_intake_motor.move_relative(360 * 3, 100);
    intake.stop();
    chassis.follow(four_txt, 24, 10000, true, false);
    intake.move_upper();
    pros::delay(750);
    chassis.moveRelative(6, 1000);
}