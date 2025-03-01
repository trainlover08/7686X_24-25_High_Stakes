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
    chassis.moveRelative(-24, 5000); // move forward 24 tiles. Allows 2000 milliseconds for the movement. Backup.
    intake.stop(); // stop both stages of the intake while moving backwards.
    chassis.moveRelative(-6, 2000); // move back 6 inches ending with the back to the mogo. Do not execute the following code after the motion finished.
    mogo_mech_piston.extend(); // extend the clamp.
    chassis.turnRelative(-90, 1000); // put the back of the robot 90 degrees to the left. Wait for the motion to finish before dropping the mogo.
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

Lady_Brown::position* pose;

void lb_auto_task () {
    while (1) {
        lady_brown.move_to_position(pose);
        pros::delay(20);
    }
}

Lady_Brown::position ladder_pose ({.angle=3000, .pid=&lady_brown_score_controller});

void skills () {
    chassis.setPose(-62.7, -4.9, 90);
    pros::Task lb_task(lb_auto_task);
    pose = &hold;
    intake.move_upper();
    pros::delay(500);
    chassis.moveToPoint(-TILE * 2, -4.9, 5000, {.minSpeed=31, .earlyExitRange=3}, false);
    intake.stop();
    mogo_mech_piston.retract();
    chassis.turnToHeading(REFEREE, 500);
    chassis.moveToPose(-TILE * 2, -TILE + MOGO_FLATEDGE, REFEREE, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-24 * 2, -24, REFEREE, 2000, {.forwards=false, .maxSpeed=72}, false);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.turnToPoint(-TILE, -TILE, 500);
    intake.move();
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
    pose = &load;
    chassis.moveToPoint(0, -42, 5000, {.minSpeed=31, .earlyExitRange=4});
    pros::delay(500);
    intake.stop();
    pose = &score;
    chassis.moveToPose(0, -TILE * 3 + DRIVE_LENGTH / 2, AUDIENCE, 5000, {.minSpeed=1, .earlyExitRange=3}, false);
    pose = &hold;
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
    pose = &load;
    chassis.moveToPoint(-TILE * 2, TILE * 2, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(135, 500);
    chassis.moveToPose(-58, 58, 135, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-TILE * 2, TILE * 2, 2000, {.minSpeed=31, .earlyExitRange=3}, false);
    intake.stop_upper();
    pose = &score;
    chassis.moveToPoint(0, 42, 5000, {.minSpeed=31, .earlyExitRange=4});
    chassis.moveToPose(0, TILE * 3 - DRIVE_LENGTH / 2, REFEREE, 5000, {.minSpeed=1, .earlyExitRange=3}, false);
    pose = &hold;
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

void auto_stopper () {
    unsigned int start = pros::millis();
    while (pros::millis() - start < 15000) pros::delay(10);
    pros::Mutex mut;
    mut.take();
    chassis.cancelAllMotions();
    intake.stop();
    lady_brown_motor.move(0);
    while (1) pros::delay(1000);
}

void awp_red () {
    chassis.setPose(-55.2, 46.9, 90);
    intake.move_lower();
    mogo_mech_piston.retract();
    chassis.moveToPose(-23.5, 46.2, 90, 5000, {.minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    chassis.turnToPoint(-23, 28, 250, {.forwards=false, .direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPose(-23, 28, 0, 1500, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-42.5, 6.3, 500, {}, true);
    intake.move_upper();
    chassis.moveToPoint( -38.0, 18.35, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_upper();
    intake.raise_intake(true);
    chassis.moveToPose(-47.5, 6.3, -161.6, 1500, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.raise_intake(false);
    intake.move_lower();
    pros::delay(250);
    chassis.moveToPose(-48.0, 1.2, -133.0, 1000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_lower();
    chassis.turnRelative(-45, 250);
    chassis.moveRelative(-1, 150);
    intake.move_lower();
    doinker_piston.extend();
    pros::delay(250);
    chassis.turnToHeading(-344, 500, {.direction=lemlib::AngularDirection::CW_CLOCKWISE}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-48.4, -6.4, 1000, {.forwards=false}, false);
    upper_intake_motor.move_relative(360 * -0.5, 100);
    mogo_mech_piston.retract();
    chassis.moveRelative(6, 200);
    chassis.turnToHeading(-290, 500);
    upper_intake_motor.move_relative(360 * 2.5, 600);
    chassis.moveToPose(-69.2, 0.75, -270, 1500, {.forwards=false}, false);
    intake.stop_lower();
    intake.move_upper();
    pros::delay(250);
    chassis.moveToPoint(-48, 0, 2000, {.minSpeed=1, .earlyExitRange=2}, true);
    chassis.turnToPoint(-24.0, -24.0, 500, {.forwards=false}, true);
    intake.stop_upper();
    chassis.moveToPose(-24.0, -24.0, 296.8, 3000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-17.3, 34.7, 500, {.forwards=false}, true);
    intake.move();
    chassis.moveToPose(-24.7, -48.6 - RING, -182, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    intake.move_lower();
    chassis.moveToPose(-22, -16, 45, 750, {.forwards=false}, true);
    intake.raise_intake(false);
    intake.move();
    pros::delay(500);
    lady_brown_motor.move_velocity(-200);
    pros::delay(260);
    lady_brown_motor.move_velocity(0);
}

void awp_blue () {
    chassis.setPose(-55.6, 29.7, 90);
    intake.move_lower();
    mogo_mech_piston.retract();
    chassis.moveToPoint(-23.5, 46.2, 5000, {.minSpeed=1, .earlyExitRange=7}, false);
    pros::delay(250);
    chassis.turnToPoint(-23, 28, 250, {.forwards=false, .direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPose(-23, 28, 0, 1500, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-42.5, 6.3, 500, {}, true);
    intake.move_upper();
    chassis.moveToPoint( -38.0, 18.35, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_upper();
    intake.raise_intake(true);
    chassis.moveToPose(-47.5, 6.3, -161.6, 1500, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.raise_intake(false);
    intake.move_lower();
    pros::delay(250);
    chassis.moveToPose(-48.0, 1.2, -133.0, 1000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_lower();
    chassis.turnRelative(-45, 250);
    chassis.moveRelative(2, 100);
    chassis.moveRelative(-3, 150);
    intake.move_lower();
    doinker_piston.extend();
    pros::delay(250);
    chassis.turnToHeading(-344, 500, {.direction=lemlib::AngularDirection::CW_CLOCKWISE}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-48.4, -6.4, 1000, {.forwards=false}, false);
    upper_intake_motor.move_relative(360 * -0.5, 100);
    mogo_mech_piston.retract();
    chassis.moveRelative(6, 200);
    chassis.turnToHeading(-290, 500);
    upper_intake_motor.move_relative(360 * 2.25, 600);
    chassis.moveToPose(-69.2, 0.0, -270, 1500, {.forwards=false}, false);
    intake.stop_lower();
    intake.move_upper();
    pros::delay(250);
    chassis.moveToPoint(-48, 0, 2000, {.minSpeed=1, .earlyExitRange=2}, true);
    chassis.turnToPoint(-24.0, -24.0, 500, {.forwards=false}, true);
    intake.stop_upper();
    chassis.moveToPose(-24.0, -24.0, 296.8, 3000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-17.3, 34.7, 500, {.forwards=false}, true);
    intake.move();
    chassis.moveToPose(-30.7, -48.6, -182, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    intake.move_lower();
    chassis.moveToPose(-26, -20, 45, 750, {.forwards=false}, true);
    intake.raise_intake(false);
    intake.move();
    pros::delay(500);
    lady_brown_motor.move_velocity(-200);
    pros::delay(260);
    lady_brown_motor.move_velocity(0);
}

void goal_rush () {
    chassis.setPose(-54.2, -62.0, 90);
    mogo_mech_piston.retract();
    chassis.turnToHeading(430.6, 100);
    intake.move_lower();
    chassis.moveToPoint(-15.7, -48.8, 5000, {.minSpeed=1, .earlyExitRange=.5}, false);
    doinker_piston.extend();
    chassis.moveToPoint(-31.0, -51.9, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-36.0, -51.9, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    chassis.turnToHeading(250, 1000);
    chassis.moveToPoint(-19.9, -52.2, 2000, {.forwards=false}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-26.9, -47.8, 116, 2000, {.forwards=false}, true);
    upper_intake_motor.move_relative(360 * 2, 600);
    chassis.moveToPoint(-26.0, -50.0, 2000);
    mogo_mech_piston.retract();
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(-24.0, -24.0, 2000, {.forwards=false, .maxSpeed=72}, false); // do not change speed back
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-11.8, -59.0, 0, 2000, {}, true);
    intake.stop_lower();
    intake.move_upper();
}

void skills_2 () {
    chassis.setPose(-63, 0, 90);
    intake.move();
    mogo_mech_piston.retract();
    pros::delay(1000);
    chassis.moveRelative(24, 3000);
    chassis.turnToPoint(-TILE * 2, -TILE, 1000, {.forwards=false});
    chassis.moveRelative(-24, 3000);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-TILE * 2.5, -TILE * 2.5, 45, 5000, {.forwards=false}, false);
    mogo_mech_piston.retract();
    pros::delay(500);
    chassis.moveToPose(-TILE * 2, TILE, 180, 7500, {.forwards=false}, true);
    pros::delay(500);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-TILE * 2.5, TILE * 2.5, 135, 5000, {.forwards=false}, false);
    mogo_mech_piston.retract();
    pros::delay(500);
    chassis.moveRelative(24, 2000);
}