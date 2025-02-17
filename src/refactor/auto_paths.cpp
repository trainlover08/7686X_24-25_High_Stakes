#include "../refactor/auto_paths.hpp"
#include "constructors.hpp"

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