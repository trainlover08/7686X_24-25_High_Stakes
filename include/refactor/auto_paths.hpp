#pragma once

#include "constructors.hpp"
#include "dimentions.hpp"

using namespace dimensions::all;

constexpr double DOINKER_LENGTH = 5.5;

bool wall_stake_to_load = 0;

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

double target = 3775.0;
void lb_auto_task () {
    while (1) {
        double kP = 0.1;
        double kD = 0.0325;
        double error = 0;
        while (1) {
            double error2 = error;
            double error = target - lady_brown_rotation.get_value();
            if (error < 0.5 && error > -0.5) break;
            double output = kP * error + kD * error - error2;
            lady_brown_motor.move_velocity(output);
            pros::delay(10);
        }
        pros::delay(10);
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
    chassis.moveToPose(-23, 26, 0, 1500, {.forwards=false, .minSpeed=31, .earlyExitRange=2}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-42.5, 6.3, 500, {}, true);
    pros::delay(250);
    intake.move_upper();
    chassis.moveToPoint( -38.0, 18.35, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_upper();
    intake.raise_intake(true);
    chassis.moveToPose(-47.5, 6.3, -161.6, 1500, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.raise_intake(false);
    intake.move_lower();
    pros::delay(250);
    chassis.moveToPose(-48.0, 1.2, -133.0, 1000, {.minSpeed=1, .earlyExitRange=1}, false);
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
    upper_intake_motor.move_relative(360 * 2.0, 600);
    chassis.moveToPose(-69.2, 0.75, -270, 1500, {.forwards=false}, false);
    intake.move_upper();
    pros::delay(350);
    chassis.moveToPoint(-48, 0, 2000, {.minSpeed=1, .earlyExitRange=2}, true);
    chassis.turnToPoint(-24.0, -24.0, 500, {.forwards=false}, true);
    intake.stop_upper();
    chassis.moveToPose(-24.0, -24.0, 296.8, 3000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-17.3, 34.7, 500, {.forwards=false}, true);
    intake.move();
    chassis.moveToPose(-24.7, -48.6 - RING * 1.5, -182, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    intake.move_lower();
    chassis.moveToPose(-22, -16, 45, 750, {.forwards=false}, true);
    intake.raise_intake(false);
    intake.move();
    pros::delay(500);
    lady_brown_motor.move_velocity(-200);
    pros::delay(250);
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
    pros::delay(250);
    intake.move_upper();
    chassis.moveToPoint( -38.0, 18.35, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_upper();
    intake.raise_intake(true);
    chassis.moveToPose(-47.5, 6.3, -161.6, 1500, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.raise_intake(false);
    intake.move_lower();
    pros::delay(250);
    chassis.moveToPose(-48.0, 1.2, -133.0, 1000, {.minSpeed=1, .earlyExitRange=1}, false);
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
    upper_intake_motor.move_relative(360 * 2.0, 600);
    chassis.moveToPose(-69.2, 0.0, -270, 1500, {.forwards=false}, false);
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
    pros::delay(240);
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
    chassis.turnToHeading(270, 1000);
    chassis.moveToPoint(-15.9, -56.2, 2000, {.forwards=false, .minSpeed=72}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-26.9, -47.8, 116, 2000, {.forwards=false}, true);
    upper_intake_motor.move_relative(360 * 2, 600);
    chassis.moveToPoint(-26.0, -50.0, 2000);
    mogo_mech_piston.retract();
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(-26.0, -21.0, 2000, {.forwards=false, .maxSpeed=72}, false); // do not change speed back
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-15.8, -59.0, 90, 2000, {}, true);
    intake.stop_lower();
    intake.move_upper();
}

void skills_2 () {
    chassis.setPose(-63, 0, 90);
    chassis.moveToPoint(-68, 0, 60000, {.forwards=false});
    pros::delay(1000);
    intake.move();
}

void special_red () {
    chassis.setPose(-55.2, 46.9, 90);
    intake.move_lower();
    mogo_mech_piston.retract();
    chassis.moveToPose(-23.5, 46.2, 90, 5000, {.minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    chassis.turnToPoint(-23, 28, 250, {.forwards=false, .direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPose(-23, 26, 0, 1500, {.forwards=false, .minSpeed=31, .earlyExitRange=2}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-42.5, 6.3, 500, {}, true);
    pros::delay(250);
    intake.move_upper();
    chassis.moveToPoint( -38.0, 18.35, 2000, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.stop_upper();
    intake.raise_intake(true);
    chassis.moveToPose(-47.5, 6.3, -161.6, 1500, {.minSpeed=1, .earlyExitRange=1}, false);
    intake.raise_intake(false);
    intake.move_lower();
    pros::delay(250);
    chassis.moveToPose(-48.0, 1.2, -133.0, 1000, {.minSpeed=1, .earlyExitRange=1}, false);
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
    upper_intake_motor.move_relative(360 * 2.0, 600);
    chassis.moveToPose(-69.2, 0.75, -270, 1500, {.forwards=false}, false);
    intake.move_upper();
    pros::delay(350);
    chassis.moveToPoint(-48, 0, 2000, {.minSpeed=1, .earlyExitRange=2}, true);
    chassis.turnToPoint(-24.0, -24.0, 500, {.forwards=false}, true);
    intake.stop_upper();
    chassis.moveToPose(-24.0, -24.0, 296.8, 3000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-17.3, 34.7, 500, {.forwards=false}, true);
    intake.move();
    chassis.moveToPose(-24.7, -48.6 - RING * 1.5, -182, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    intake.move_lower();
    intake.raise_intake(false);
    intake.move();
}

void old_goal_rush_four_ring () {
    pros::Task lb_auto (lb_auto_task);
    lb_auto.suspend();
    chassis.setPose(-54.2, -62.0, 90);
    mogo_mech_piston.retract();
    chassis.turnToHeading(430.6, 100);
    intake.move_lower();
    chassis.moveToPoint(-15.7, -48.8, 5000, {.minSpeed=1, .earlyExitRange=.5}, false);
    doinker_piston.extend();
    chassis.moveToPoint(-31.0, -51.9, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-36.0, -51.9, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    chassis.turnToHeading(270, 1000);
    chassis.moveToPoint(-15.9, -56.2, 2000, {.forwards=false, .minSpeed=72}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-26.9, -47.8, 116, 2000, {.forwards=true}, true);
    upper_intake_motor.move_relative(360 * 2, 600);
    chassis.moveToPoint(-26.0, -50.0, 2000, {.minSpeed=1, .earlyExitRange=false});
    mogo_mech_piston.retract();
    chassis.turnToHeading(180, 400);
    chassis.moveToPoint(-26.0, -21.0, 2000, {.forwards=false, .maxSpeed=72}, false); // do not change speed back
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    intake.stop_lower();
    upper_intake_motor.move_relative(360 * 3, 600);
    chassis.turnToPoint(0, 0, 300);
    chassis.moveToPose(-10.3, -10.4, 35.0, 6000, {}, false);
    doinker_piston.extend();
    pros::delay(500);
    chassis.turnToHeading(65, 250);
    chassis.moveToPoint(-25.4, -17.4, 2000, {.forwards=false, .minSpeed=15, .earlyExitRange=1});
    chassis.moveToPose(-24.1, -17.5, 118.0, 2000, {.forwards=false}, false);
    chassis.turnToHeading(180, 100);
    lb_auto.resume();
    doinker_piston.retract();
    intake.move();
    chassis.moveToPose(-15.7, -24.5, 112.0, 2000, {.minSpeed=72, .earlyExitRange=2}, false);
    chassis.moveToPose(-10.7, -60.0, 133, 3000, {}, true);
    while (upper_intake_motor.get_actual_velocity() > 5) {intake.move(); pros::delay(10);}
    for (int i = 0; i < 2; ++i) {
        intake.stop_upper();
        pros::delay(200);
        intake.move_upper();
        pros::delay(200);
    }
    while (chassis.isInMotion()) pros::delay(10);
    intake.stop();
    lb_auto.suspend();
    lady_brown_motor.move_velocity(-200);
    pros::delay(2000);
}

void auto_sort_task () {
    while (1) {
        intake.color_sort(pros::Color::red);
        pros::delay(10);
    }
}

void goal_rush_four_ring () {
    pros::Task lb_auto (lb_auto_task);
    pros::Task auto_sort(auto_sort_task);
    auto_sort.suspend();
    chassis.setPose(-54.2, -62.0, 90);
    mogo_mech_piston.retract();
    chassis.turnToHeading(430.6, 100);
    intake.move_lower();
    chassis.moveToPoint(-15.7, -48.8, 5000, {.minSpeed=1, .earlyExitRange=.5}, false);
    doinker_piston.extend();
    chassis.moveToPoint(-31.0, -51.9, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-36.0, -51.9, 2000, {.minSpeed=1, .earlyExitRange=1}, true);
    chassis.turnToHeading(270, 1000);
    chassis.moveToPoint(-15.9, -56.2, 2000, {.forwards=false, .maxSpeed=50, .minSpeed=1, .earlyExitRange=1}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.moveToPose(-20, -48, 130, 1500, {.forwards=false});
    chassis.moveToPose(-10.2, -59.0, 130, 2500, {.maxSpeed=50, .minSpeed=1, .earlyExitRange=0.25}, false);
    lb_auto.suspend();
    lady_brown_motor.move_velocity(-200);
    pros::delay(350);
    intake.move();
    pros::delay(500);
    chassis.moveToPoint(-36.2, -38.9, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, true);
    pros::delay(300);
    mogo_mech_piston.retract();
    chassis.turnToPoint(-24.0, -24.0, 300, {.forwards=false});
    chassis.moveToPoint(-24.0, -24.0, 1500, {.forwards=false, .maxSpeed=71, .minSpeed=1, .earlyExitRange=1}, false);
    lady_brown_motor.move_velocity(200);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(500);
    intake.stop_lower();
    upper_intake_motor.move_relative(360 * 3, 600);
    chassis.turnToPoint(0, 0, 300);
    lady_brown_motor.move_velocity(0);
    chassis.moveToPose(-12.0, -7.9, 42.0, 6000, {.maxSpeed=50, .minSpeed=1, .earlyExitRange=0.5}, false);
    doinker_piston.extend();
    pros::delay(500);
    chassis.turnToHeading(65, 250);
    chassis.moveToPoint(-25.4, -17.4, 2000, {.forwards=false, .minSpeed=15, .earlyExitRange=1});
    chassis.moveToPose(-24.1, -17.5, 118.0, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=2}, false);
    doinker_piston.retract();
    chassis.turnToHeading(180, 150);
    auto_sort.resume();
    chassis.moveToPoint(-18, -28, 500);
    pros::delay(5000);
    lb_auto.remove();
    auto_sort.remove();
}

ASSET(rednegringrush_txt);

void exp_rr () {
    /*chassis.setPose(-60.4, 37.7, 156.6);
    doinker_piston.extend();
    pros::Task lb(lb_auto_task);
    target = 3400;
    pros::delay(250);
    pros::Task sort(auto_sort_task);
    chassis.turnToHeading(141.2, 250);
    intake.raise_intake(true);
    intake.move_lower();
    chassis.moveToPoint(-54.4, 29.8, 2000, {}, false);
    intake.raise_intake(false);
    chassis.moveToPoint(-54.4, 29.8, 1000, {}, false);
    doinker_piston.retract();
    intake.stop_lower();
    chassis.moveToPoint(-50.6, 24.6, 1000, {.forwards=false});
    chassis.turnToHeading(90, 500);
    chassis.moveToPoint(-62.4, 24.7, 2000, {.forwards=false}, false);
    intake.move_upper();
    pros::delay(500);
    intake
    point -57.4, 24.6
    turn 231.9
    point back -32.8, 43.3
    mogo (up first)
    turn 57cw
    pose -16.6, 54.2, 33
    pose -12.5, 66.5, 0
    point -14.5 78?
    turn 13
    lb*/
    /*chassis.setPose(-57.0, 39.3, 231);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-63.4, 34.3, 1000, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(1000);
    chassis.moveToPoint(-34.5, 48.0, 2000, {.forwards=false, .maxSpeed=50}, false);
    pros::Task lb_a(lb_auto_task);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.turnToHeading(52, 600, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    chassis.moveToPoint(-12.2, 56.0, 2000, {.maxSpeed=50}, true);
    intake.move();
    chassis.moveToPose(-10.0, 61.5, 18, 2000, {.maxSpeed=50}, true);
    chassis.moveToPose(-9.3, 71.5, 0, 2000, {.maxSpeed=50}, true);
    target = 3000;
    chassis.moveToPoint(-17.0, 84.0, 2000, {.maxSpeed=50});
    chassis.turnToHeading(11.7, 500, {}, false);
    lb_a.suspend();
    lady_brown_motor.move_velocity(-200);*/

    chassis.setPose(0, 0, 180);
    mogo_mech_piston.retract();
    intake.move_lower(-100);
    chassis.moveToPose(-6.3, -10.7, 239.2, 1500, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(500);
    chassis.moveToPoint(22.8, 6.9, 600, {.forwards=false, .minSpeed=1, .earlyExitRange=.5}, false);
    chassis.moveToPoint(22.8, 5.1, 1500, {.forwards=false, .maxSpeed=31, .minSpeed=1, .earlyExitRange=.5}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::Task sort(auto_sort_task);
    pros::delay(250);
    chassis.turnToHeading(50, 500, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    lady_brown_motor.move(0);
    chassis.moveToPoint(37.6, 14.5, 1000, {.minSpeed=1, .earlyExitRange=3});
    intake.move();
    chassis.turnToHeading(30, 250);
    chassis.moveToPose(41.5, 28.0, 0, 5000, {.maxSpeed=40, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(41.5, 36.0, 5000, {.maxSpeed=60, .minSpeed=1, .earlyExitRange=1});
    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 500, {.maxSpeed=50});
    chassis.turnToPoint(30.4, 31.0, 500, {.maxSpeed=72});
    chassis.moveToPoint(30.4, 31.0, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move();
    chassis.moveToPose(6.3, -11.6, -176.2, 5000, {.maxSpeed=72, .minSpeed=16, .earlyExitRange=1});
    intake.move();
    pros::delay(2000);
    intake.raise_intake(true);
    intake.move();
    chassis.moveToPoint(-5.4, -68.2, 1000, {.maxSpeed=72});
    intake.raise_intake(false);
    intake.move();
    doinker_piston.extend();
    chassis.moveToPoint(-7.4, -70.2, 2500, {.forwards=true, .minSpeed=31, .earlyExitRange=1});
    intake.move();
    chassis.turnToHeading(90, 500);


    /*intake.stop_upper();
    pros::Task lb(lb_auto_task);
    pros::delay(500);
    chassis.turnToHeading(31, 1000);
    pros::delay(750);
    intake.move();
    chassis.moveToPose(41.0, 41.2, 31, 2000, {}, true);
    pros::delay(250);
    for (int i = 0; i < 3; ++i) {
        pros::delay(150);
        intake.stop();
        pros::delay(150);
        intake.move_upper();
    }
    while (chassis.isInMotion()) pros::delay(10);
    target = 2200;
    intake.stop();
    while (lady_brown_rotation.get_value() > 2350) pros::delay(10);
    pros::delay(300);
    chassis.moveToPoint(34, 29.3, 500, {.forwards=false, .minSpeed=1, .earlyExitRange=1});
    //intake.raise_intake(true);
    intake.move();
    chassis.moveToPoint(10.1, -8.2, 3000, {.minSpeed=50}, false);

    //chassis.moveToPoint(10.1, -8.2, 1000, {.maxSpeed=50}, false);
    //intake.raise_intake(false);
    //chassis.moveToPoint(14.4, -1.6, 500, {.minSpeed=1, .earlyExitRange=1});
    //chassis.moveToPoint(5.3, -15.7, 500);*/
    pros::delay(5000);
    //lb.remove();
}

void neg_awp () {
    chassis.setPose(0, 0, 180);
    mogo_mech_piston.retract();
    intake.move_lower(-100);
    chassis.moveToPose(-6.5, -11.8, 117.5, 1500, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(500);
    chassis.moveToPoint(22.8, 6.9, 600, {.forwards=false, .minSpeed=1, .earlyExitRange=.5}, false);
    chassis.moveToPoint(22.8, 5.1, 1500, {.forwards=false, .maxSpeed=31, .minSpeed=1, .earlyExitRange=.5}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::Task sort(auto_sort_task);
    pros::delay(250);
    chassis.turnToHeading(50, 500, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    lady_brown_motor.move(0);
    chassis.moveToPoint(37.6, 14.5, 1000, {.minSpeed=1, .earlyExitRange=3});
    intake.move();
    chassis.turnToHeading(30, 250);
    chassis.moveToPose(41.5, 28.0, 0, 5000, {.maxSpeed=40, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(41.5, 36.0, 5000, {.maxSpeed=60, .minSpeed=1, .earlyExitRange=1});
    chassis.swingToHeading(270, lemlib::DriveSide::LEFT, 500, {.maxSpeed=50});
    chassis.turnToPoint(30.4, 31.0, 500, {.maxSpeed=72});
    chassis.moveToPoint(30.4, 31.0, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move();
    chassis.moveToPose(6.3, -11.6, -176.2, 5000, {.maxSpeed=72, .minSpeed=16, .earlyExitRange=1});
    intake.move();
    pros::delay(2000);
    intake.raise_intake(true);
    intake.move();
    chassis.moveToPoint(-5.4, -68.2, 1000, {.maxSpeed=72});
    intake.raise_intake(false);
    intake.move();
    chassis.moveToPoint(5.0, -31.1, 1000);
    intake.move();
    chassis.moveToPose(36.9, -25.4, -328, 3000, {.minSpeed=40}, false);
    intake.move();
    pros::delay(5000);
}

void neg_awp_blue () {
    chassis.setPose(0, 0, 180);
    mogo_mech_piston.retract();
    intake.move_lower(-100);
    chassis.moveToPose(-6.5, -11.8, 117.5, 1500, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(500);
    chassis.moveToPoint(-22.8, 6.9, 600, {.forwards=false, .minSpeed=1, .earlyExitRange=.5}, false);
    chassis.moveToPoint(-22.8, 5.1, 1500, {.forwards=false, .maxSpeed=31, .minSpeed=1, .earlyExitRange=.5}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::Task sort(auto_sort_task);
    pros::delay(250);
    chassis.turnToHeading(-50, 500, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    lady_brown_motor.move(0);
    chassis.moveToPoint(-37.6, 14.5, 1000, {.minSpeed=1, .earlyExitRange=3});
    intake.move();
    chassis.turnToHeading(-30, 250);
    chassis.moveToPose(-41.5, 28.0, 0, 5000, {.maxSpeed=40, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-41.5, 36.0, 5000, {.maxSpeed=60, .minSpeed=1, .earlyExitRange=1});
    chassis.swingToHeading(-270, lemlib::DriveSide::LEFT, 500, {.maxSpeed=50});
    chassis.turnToPoint(-30.4, 31.0, 500, {.maxSpeed=72});
    chassis.moveToPoint(-30.4, 31.0, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move();
    chassis.moveToPose(-6.3, -11.6, 176.2, 5000, {.maxSpeed=72, .minSpeed=16, .earlyExitRange=1});
    intake.move();
    pros::delay(2000);
    intake.raise_intake(true);
    intake.move();
    chassis.moveToPoint(5.4, -68.2, 1000, {.maxSpeed=72});
    intake.raise_intake(false);
    intake.move();
    chassis.moveToPoint(-5.0, -31.1, 1000);
    intake.move();
    chassis.moveToPose(-36.9, -25.4, 328, 3000, {.minSpeed=40}, false);
    intake.move();
    pros::delay(5000);
}

void neg_clear_blue () {
    chassis.setPose(0, 0, 180);
    mogo_mech_piston.retract();
    intake.move_lower(-100);
    chassis.moveToPose(7.1, -10.9, -239.2, 1500, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(500);
    chassis.moveToPoint(-22.8, 6.9, 600, {.forwards=false, .minSpeed=1, .earlyExitRange=.5}, false);
    chassis.moveToPoint(-22.8, 5.1, 1500, {.forwards=false, .maxSpeed=31, .minSpeed=1, .earlyExitRange=.5}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::Task sort(auto_sort_task);
    pros::delay(250);
    chassis.turnToHeading(-50, 500, {.direction=lemlib::AngularDirection::CCW_COUNTERCLOCKWISE});
    lady_brown_motor.move(0);
    chassis.moveToPoint(-37.6, 14.5, 1000, {.minSpeed=1, .earlyExitRange=3});
    intake.move();
    chassis.turnToHeading(-30, 250);
    chassis.moveToPose(-41.5, 28.0, 0, 5000, {.maxSpeed=40, .minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-41.5, 36.0, 5000, {.maxSpeed=60, .minSpeed=1, .earlyExitRange=1});
    chassis.swingToHeading(-270, lemlib::DriveSide::LEFT, 500, {.maxSpeed=50});
    chassis.turnToPoint(-30.4, 31.0, 500, {.maxSpeed=72});
    chassis.moveToPoint(-30.4, 31.0, 2000, {.minSpeed=1, .earlyExitRange=1});
    intake.move();
    chassis.moveToPose(-6.3, -11.6, 176.2, 5000, {.maxSpeed=72, .minSpeed=16, .earlyExitRange=1});
    intake.move();
    pros::delay(2000);
    intake.raise_intake(true);
    intake.move();
    chassis.moveToPoint(5.4, -68.2, 1000, {.maxSpeed=72});
    intake.raise_intake(false);
    intake.move();
    chassis.moveToPoint(3.0, -60.0, 2500, {.forwards=true, .minSpeed=31, .earlyExitRange=1}, false);
    intake.move();
}

void really_new_pos_base () {
    chassis.setPose(0, 0, 270);
    mogo_mech_piston.retract();
    intake.move_lower();
    chassis.moveToPoint(-26.6, 10.0, 2000, {.minSpeed=31, .earlyExitRange=2}, false);
    chassis.turnToHeading(250, 250);
    chassis.moveToPoint(-36.2, 6.5, 2000, {.minSpeed=72, .earlyExitRange=1}, false);
    doinker_piston.extend();
    pros::delay(100);
    chassis.moveToPoint(-19.1, 13.1, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-17.0, 13.0, 1000);
    chassis.moveToPoint(-39.7, 11.8, 500, {.forwards=false}, false);
    chassis.moveToPoint(-39.7, 11.8, 750, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPoint(-19.1, 13.0, 1000);
    chassis.turnToPoint(-39.4, -0.0, 700, {.direction=lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.moveToPose(-39.4, -0.0, 578, 1900, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(750);
    intake.move();
    chassis.moveToPoint(-34.5, 3.4, 1000);
    lady_brown_motor.move_velocity(0);
    chassis.swingToHeading(674, lemlib::DriveSide::LEFT, 750, {}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-36.1, 9.8, 1000);
    chassis.turnToHeading(561, 500);
    chassis.moveToPoint(-29.2, 30.5, 500, {.forwards=false}, false);
    chassis.moveToPoint(-29.2, 30.5, 1000, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    
}

void really_new_pos_awp () {
    chassis.setPose(0, 0, 270);
    mogo_mech_piston.retract();
    intake.move_lower();
    chassis.moveToPoint(-26.6, 10.0, 2000, {.minSpeed=31, .earlyExitRange=2}, false);
    chassis.turnToHeading(250, 250);
    chassis.moveToPoint(-36.2, 6.5, 2000, {.minSpeed=72, .earlyExitRange=1}, false);
    doinker_piston.extend();
    pros::delay(100);
    chassis.moveToPoint(-19.1, 13.1, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-17.0, 13.0, 1000);
    chassis.turnToHeading(448.3, 500, {lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.moveToPoint(-39.7, 11.8, 500, {.forwards=false}, false);
    chassis.moveToPoint(-39.7, 11.8, 750, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPoint(-19.1, 13.0, 1000);
    chassis.turnToPoint(-39.4, -0.0, 700, {.direction=lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.moveToPose(-39.4, -0.0, 578, 1900, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(750);
    intake.move();
    chassis.moveToPoint(-34.5, 3.4, 1000);
    lady_brown_motor.move_velocity(0);
    chassis.swingToHeading(674, lemlib::DriveSide::LEFT, 750, {}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-36.1, 9.8, 1000);
    chassis.turnToHeading(561, 500);
    chassis.moveToPoint(-29.2, 30.5, 500, {.forwards=false}, false);
    chassis.moveToPoint(-29.2, 30.5, 1000, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPose(-39.9, 42.7, 495.4, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=0.5});
    chassis.moveToPose(-39.9, 42.7, 495.4, 3000, {.forwards=false});
    lady_brown_motor.move_velocity(200);
}

void really_new_pos_clear () {
    chassis.setPose(0, 0, 270);
    mogo_mech_piston.retract();
    intake.move_lower();
    chassis.moveToPoint(-26.6, 10.0, 2000, {.minSpeed=31, .earlyExitRange=2}, false);
    chassis.turnToHeading(250, 250);
    chassis.moveToPoint(-36.2, 6.5, 2000, {.minSpeed=72, .earlyExitRange=1}, false);
    doinker_piston.extend();
    pros::delay(100);
    chassis.moveToPoint(-19.1, 13.1, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-17.0, 13.0, 1000);
    chassis.moveToPoint(-39.7, 11.8, 500, {.forwards=false}, false);
    chassis.moveToPoint(-39.7, 11.8, 750, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPoint(-19.1, 13.0, 1000);
    chassis.turnToPoint(-39.4, -0.0, 700, {.direction=lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.moveToPose(-39.5, -0.0, 578, 1900, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(750);
    intake.move();
    chassis.moveToPoint(-34.5, 3.4, 1000);
    lady_brown_motor.move_velocity(0);
    chassis.swingToHeading(700, lemlib::DriveSide::LEFT, 750, {}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-36.1, 9.8, 1000);
    chassis.turnToHeading(561, 500);
    chassis.moveToPoint(-29.2, 30.5, 500, {.forwards=false}, false);
    chassis.moveToPoint(-29.2, 30.5, 1000, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPose(5.4, -6.4, 116.6, 2000, {}, true);
    doinker_piston.extend();
    lady_brown_motor.move_velocity(200);
    intake.stop();
    chassis.turnToHeading(315, 500, {}, false);
}

void really_new_pos_rush () {
    chassis.setPose(0, 0, 270);
    mogo_mech_piston.retract();
    intake.move_lower();
    chassis.moveToPoint(-26.6, 10.0, 2000, {.minSpeed=31, .earlyExitRange=2}, false);
    chassis.turnToHeading(250, 250);
    chassis.moveToPoint(-36.2, 6.5, 2000, {.minSpeed=72, .earlyExitRange=1}, false);
    doinker_piston.extend();
    pros::delay(100);
    chassis.moveToPoint(-19.1, 13.1, 2000, {.forwards=false, .minSpeed=1, .earlyExitRange=1}, false);
    doinker_piston.retract();
    chassis.moveToPoint(-17.0, 13.0, 1000);
    chassis.moveToPoint(-39.7, 11.8, 500, {.forwards=false}, false);
    chassis.moveToPoint(-39.7, 11.8, 750, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    chassis.moveToPoint(-19.1, 13.0, 1000);
    chassis.turnToPoint(-39.4, -0.0, 700, {.direction=lemlib::AngularDirection::CW_CLOCKWISE});
    chassis.moveToPose(-39.4, -0.0, 578, 1900, {}, false);
    lady_brown_motor.move_velocity(-200);
    pros::delay(750);
    intake.move();
    chassis.moveToPoint(-34.5, 3.4, 1000);
    lady_brown_motor.move_velocity(0);
    chassis.swingToHeading(674, lemlib::DriveSide::LEFT, 750, {}, false);
    mogo_mech_piston.retract();
    chassis.moveToPoint(-36.1, 9.8, 1000);
    chassis.turnToHeading(561, 500);
    chassis.moveToPoint(-29.2, 30.5, 500, {.forwards=false}, false);
    chassis.moveToPoint(-29.2, 30.5, 1000, {.forwards=false, .maxSpeed=50}, false);
    pros::delay(250);
    mogo_mech_piston.extend();
    pros::delay(250);
    lady_brown_motor.move_velocity(200);
    chassis.moveToPose(-37.0, 14.0, 251, 3000, {}, false);
}

void red_pos_rush_base () {
    chassis.setPose(0, 0, 90);
    intake.move();
    chassis.turnToHeading(75, 150);
    chassis.moveToPoint(38.5, 13.2, 5000, {.minSpeed=1, .earlyExitRange=.5}, false);
    doinker_piston.extend();
}