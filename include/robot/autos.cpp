#include "robot.cpp"

void red_right_awp () {
    chassis.setPose(-48-8, -24, 270);
    // start with one already in the intake, reflect change on arm math
    chassis.moveToPoint(-24, -24, 3000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-24, -48, 500, {});
    chassis.moveToPoint(-24, -48, 3000, {.minSpeed=1, .earlyExitRange=1});
    lower_intake_first_stage.move(12000);
    lower_intake_second_stage.move(12000);
    upper_intake.move(12000);
    chassis.turnToPoint(-48, -24, 500, {});
    /**upper_intake.move(0);
    // extend intake?
    chassis.moveToPoint(-48, -24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-48, 0, 0, 5000, {.minSpeed=1, .earlyExitRange=1});
    mogo_mech_piston.retract();
    chassis.turnToPoint(-72+8, 0, 500, {.forwards=0});
    chassis.moveToPose(-72+8, 0, 90, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5}, 0);
    upper_intake.move(12000);*/
}

void blue_right_awp () {
    chassis.setPose(48+8, 24, 90);
    // start with one already in the intake, reflect change on arm math
    chassis.moveToPoint(24, 24, 3000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(24, 48, 500, {});
    chassis.moveToPoint(24, 48, 3000, {.minSpeed=1, .earlyExitRange=1});
    lower_intake_first_stage.move(12000);
    lower_intake_second_stage.move(12000);
    upper_intake.move(12000);
    chassis.turnToPoint(48, 24, 500, {});
    /**upper_intake.move(0);
    // extend intake?
    chassis.moveToPoint(-48, -24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-48, 0, 0, 5000, {.minSpeed=1, .earlyExitRange=1});
    mogo_mech_piston.retract();
    chassis.turnToPoint(-72+8, 0, 500, {.forwards=0});
    chassis.moveToPose(-72+8, 0, 90, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5}, 0);
    upper_intake.move(12000);*/
}

void blue_left_awp () {
    chassis.setPose(48+8, -24, 90);
    // start with one already in the intake, reflect change on arm math
    chassis.moveToPoint(24, -24, 3000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(24, -48, 500, {});
    chassis.moveToPoint(24, -48, 3000, {.minSpeed=1, .earlyExitRange=1});
    lower_intake_first_stage.move(12000);
    lower_intake_second_stage.move(12000);
    upper_intake.move(12000);
    chassis.turnToPoint(48, -24, 500, {});
    /**upper_intake.move(0);
    // extend intake?
    chassis.moveToPoint(-48, -24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-48, 0, 0, 5000, {.minSpeed=1, .earlyExitRange=1});
    mogo_mech_piston.retract();
    chassis.turnToPoint(-72+8, 0, 500, {.forwards=0});
    chassis.moveToPose(-72+8, 0, 90, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5}, 0);
    upper_intake.move(12000);*/
}

void red_left_awp () {
    chassis.setPose(-48-8, 24, 270);
    // start with one already in the intake, reflect change on arm math
    chassis.moveToPoint(-24, 24, 3000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    mogo_mech_piston.extend();
    pros::delay(500);
    chassis.turnToPoint(-24, 48, 500, {});
    chassis.moveToPoint(-24, 48, 3000, {.minSpeed=1, .earlyExitRange=1});
    lower_intake_first_stage.move(12000);
    lower_intake_second_stage.move(12000);
    upper_intake.move(12000);
    chassis.turnToPoint(-48, 24, 500, {});
    /**upper_intake.move(0);
    // extend intake?
    chassis.moveToPoint(-48, -24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(-48, 0, 0, 5000, {.minSpeed=1, .earlyExitRange=1});
    mogo_mech_piston.retract();
    chassis.turnToPoint(-72+8, 0, 500, {.forwards=0});
    chassis.moveToPose(-72+8, 0, 90, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=.5}, 0);
    upper_intake.move(12000);*/
}

void six_ring_red () {
    chassis.setPose(-48-8, 24, 270);
    // start with one already in the intake, reflect change on arm math
    chassis.moveToPoint(-24, 24, 5000, {.forwards=0}, 0);
    // extend mogo
    chassis.turnToPoint(-24, 48, 500, {});
    chassis.moveToPoint(-24, 48, 5000, {});
    // spin front stage
    chassis.moveToPose(0, 52, 90, 5000, {});
    chassis.moveToPoint(-12, 48, 2000, {.forwards=false});
    chassis.moveToPose(0, 44, 90, 5000, {});
    chassis.moveToPose(-52, 52, 315, 5000, {}, 0);
    // doinker down
    chassis.moveToPoint(-24, 24, 5000, {.forwards=0}, 0);
    // doinker up
    chassis.moveToPose(-54, 54, 315, 5000, {});
    chassis.moveToPoint(-48, 48, 5000, {.forwards=0});
    chassis.moveToPoint(-48, -48, 5000, {});
}

void skills_auto () {
    chassis.setPose(-48, 0, 0);
    chassis.moveToPose(-72+8, 0, 90, 5000, {.forwards=0}, 0);
    // spin intake upper
    chassis.moveToPoint(-48, 0, 5000, {.minSpeed=15, .earlyExitRange=4});
    chassis.moveToPose(-48, 24, 180, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    // mogo mech extend
    chassis.turnToPoint(-24, 24, 500, {});
    chassis.moveToPoint(-24, 24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(0, 60, 500);
    chassis.moveToPoint(0, 60, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(-24+12, 24, 500);
    chassis.moveToPose(-24, 24, 270, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-48, 48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-60, 48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(-48, 60, 500);
    chassis.moveToPoint(-48, 60, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-54, 54, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(135, 500);
    chassis.moveToPoint(-60, 60, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=2});
    // mogo mech retract
    chassis.moveToPoint(-48, 48, 5000, {.minSpeed=1, .earlyExitRange=4});
    chassis.moveToPose(-48, -24, 180, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    // mogo extend
    chassis.turnToPoint(-24, -24, 500);
    chassis.moveToPoint(-24, -24, 5000, {.minSpeed=1, .earlyExitRange=.75});
    chassis.moveToPoint(0, -60, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(-24+12, -48, 500);
    chassis.moveToPose(-24, -48, 270, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-48, -48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(-60, -48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(-48, -60, 500);
    chassis.moveToPoint(-48, -60, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.swingToHeading(45, lemlib::DriveSide::LEFT, 500, {}, 0);
    // drop mogo
    chassis.moveToPose(0, 0, 45, 5000, {.minSpeed=31, .earlyExitRange=2.5});
    chassis.moveToPoint(24, 24, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(48, 0, 500, {.forwards=0});
    chassis.moveToPoint(48, 0, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    // mogo extend
    chassis.turnToPoint(24, 48, 500);
    chassis.moveToPoint(24, 48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(48, 48, 500);
    chassis.moveToPoint(48, 48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(60, 48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.swingToPoint(48, 60, lemlib::DriveSide::LEFT, 500);
    // need 1 red loaded to bottom, 1 blue loaded on top in the arm
    chassis.moveToPoint(48, 60, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPose(62, 62, 225, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    // mogo retract
    chassis.moveToPose(0, 60, 0, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(0, 64, 5000, {.minSpeed=1, .earlyExitRange=0.5}, 0);
    // arm down
    chassis.moveToPoint(24, 48, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=2.5});
    chassis.moveToPoint(48, 0, 5000, {.minSpeed=31, .earlyExitRange=4});
    chassis.moveToPoint(24, -24, 5000, {.minSpeed=1, .earlyExitRange=2});
    // load 2 into arm
    chassis.moveToPoint(24, -48, 5000, {.minSpeed=1, .earlyExitRange=2});
    chassis.turnToHeading(270, 500);
    chassis.moveToPose(0, -64, 180, 5000, {.minSpeed=1, .earlyExitRange=0.5}, 0);
    // arm down
    chassis.moveToPoint(0, -60, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=2});
    // load 2 red
    chassis.moveToPose(48, -48, 90, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(60, -48, 5000, {.minSpeed=1, .earlyExitRange=1});
    chassis.moveToPoint(0, -48, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1});
    chassis.turnToHeading(180, 500);
    chassis.moveToPoint(0, -64, 5000, {.minSpeed=1, .earlyExitRange=1}, 0);
    // arm down
    chassis.moveToPoint(0, -60, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1});
    chassis.turnToPoint(48, -60, 5000, {.minSpeed=1, .earlyExitRange=1});
    // intake red then blue, hold til allinace stake
    chassis.moveToPoint(36, -24, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=3});
    chassis.moveToPose(68, 0, 270, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=1}, 0);
    //intake to stake
    chassis.moveToPoint(60, 0, 5000, {.minSpeed=1, .earlyExitRange=2});
    chassis.turnToPoint(60, -24, 500, {.forwards=0});
    chassis.moveToPoint(60, -24, 5000, {.forwards=0, .minSpeed=1, .earlyExitRange=2});
    chassis.moveToPoint(62, -60, 5000);
}

rd::Selector selector({{"Blue Right AWP", blue_right_awp, "/usd/images/blue_rightawp.bin"}, {"Blue Left AWP", blue_left_awp, "/usd/images/blue_left_awp.bin"}, {"Red Left AWP", red_left_awp, "/usd/images/red_left_awp.bin"}, {"Red Right AWP", red_right_awp, "/usd/images/red_right_awp.bin"}, {"Red Six Ring", six_ring_red, "/usd/images/red_left_elim.bin"}, {"Skills", skills_auto, "/usd/images/skills.bin"}});