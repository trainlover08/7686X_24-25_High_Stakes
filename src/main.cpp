// includes the other code that get called in "main.cpp"
#include "common.cpp"

// makes a char enum value for all of our pneumatic ports
// naming convention 'solinoid' used for the enums, for a more long term project I would make it a class, but this is subject to change at the drop of a hat
#define intake_solenoid 'E'
#define wrist_solenoid 'B'
#define arm_solenoid 'C'
#define mogo_solenoid 'D'
#define chopper_solenoid 'F'

// just as before, makes an enum value for the intake ports; the sign of the enum signifies direction the motor spins
#define intake_stage_1 -10
#define intake_stage_2 -9

// creates a controller object for the two controllers we use during matches
pros::Controller controller_1(pros::E_CONTROLLER_MASTER);
pros::Controller controller_2(pros::E_CONTROLLER_PARTNER);

// a constructor for the pros::Motor objects used
pros::Motor lm1(-15);
pros::Motor lm2(14);
pros::Motor lm3(-13);
pros::Motor rm1(16);
pros::Motor rm2(-12);
pros::Motor rm3(11);
pros::Motor intake_lower(intake_stage_1);
pros::Motor intake_upper(intake_stage_2);

// constructs an easily manipulatable std::vector<pros::Motor> for use in the program
pros::MotorGroup left_motors({lm1.get_port(), lm2.get_port(), lm3.get_port()}, pros::MotorGears::blue);

pros::MotorGroup right_motors({rm1.get_port(), rm2.get_port(), rm3.get_port()}, pros::MotorGears::blue);

pros::MotorGroup intake({intake_lower.get_port(), intake_upper.get_port()}, pros::MotorGears::green);

// constructs the drivetrain from the builder classes used in lemlib
lemlib::Drivetrain drivetrain(&left_motors, &right_motors, 11.625, 3.25, 480, 8); //left motor group, right motor group, track width, wheel diameter, drive rpm, horizontal drift

// constructs the imu used in heading tracking for odometery
pros::IMU imu(5);

// more lemlib builder class objects for odom
lemlib::TrackingWheel leftVerticalWheel(&left_motors, lemlib::Omniwheel::NEW_325, -5.825, 480);
lemlib::TrackingWheel rightVerticalWheel(&right_motors, lemlib::Omniwheel::NEW_325, 5.825, 480);

lemlib::OdomSensors sensors(nullptr, nullptr, nullptr, nullptr, &imu); //vertical tracking wheels 1 & 2, horizontal tracking wheels 3 & 4, imu

// pneumatics objects for the solinoids using the enums defined earlier, the second variable (bool) is used to denote startng position
pros::adi::Pneumatics mogo_mech(mogo_solenoid, false);
pros::adi::Pneumatics arm_mech(arm_solenoid, false);
pros::adi::Pneumatics intake_mech(intake_solenoid, false);
pros::adi::Pneumatics wrist_mech(wrist_solenoid, false);
pros::adi::Pneumatics chopper_mech(chopper_solenoid, false);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              18, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              7, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(2, // joystick deadband out of 127
                                     0, // minimum output where drivetrain will move out of 127
                                     1.023 // expo curve gain
);

// same thing for steering curves
lemlib::ExpoDriveCurve steer_curve(2, 
                                   0,
                                   1.020
);

// create the chassis object
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
                        &throttle_curve,
                        &steer_curve
);

// a function called later in the program that determines which controller is the one that allows input used in controlling the robot
std::vector<cmn::my_controller> get_active_controller(cmn::my_controller controller_1_input, cmn::my_controller controller_2_input) {
    if(controller_1_input.is_master_controller) {
        return {controller_1_input, controller_2_input};
    } else {
        return {controller_2_input, controller_1_input};
    }
}

// the specific instance of a class that stores the varibles for the drive controls used by our driver Chase
cmn::my_controller chase_config(cmn::my_controller chase) {
    chase.arm_pneumatic_button = pros::E_CONTROLLER_DIGITAL_UP;
    chase.wrist_pneumatic_button = pros::E_CONTROLLER_DIGITAL_LEFT;
    chase.intake_pneumatic_button = pros::E_CONTROLLER_DIGITAL_RIGHT;
    chase.mogo_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
    chase.switch_controller_button = pros::E_CONTROLLER_DIGITAL_B;
    chase.intake_motor_button = pros::E_CONTROLLER_DIGITAL_R1;
    chase.extake_motor_button = pros::E_CONTROLLER_DIGITAL_L1;
    chase.load_arm_button = pros::E_CONTROLLER_DIGITAL_A;
    chase.chopper_mech = pros::E_CONTROLLER_DIGITAL_L2;
    chase.reverse = pros::E_CONTROLLER_DIGITAL_R2;

    return chase;
}

// the same thing but this is for our driver miles
cmn::my_controller miles_config(cmn::my_controller miles) {
    miles.arm_pneumatic_button = pros::E_CONTROLLER_DIGITAL_L1;
    miles.wrist_pneumatic_button = pros::E_CONTROLLER_DIGITAL_L2;
    miles.mogo_pneumatic_button = pros::E_CONTROLLER_DIGITAL_DOWN;
    miles.switch_controller_button = pros::E_CONTROLLER_DIGITAL_RIGHT;
    miles.intake_motor_button = pros::E_CONTROLLER_DIGITAL_LEFT;
    miles.extake_motor_button = pros::E_CONTROLLER_DIGITAL_UP;
    miles.intake_pneumatic_button = pros::E_CONTROLLER_DIGITAL_A;
    miles.extake_motor_button = pros::E_CONTROLLER_DIGITAL_UP;
    miles.shift_button = pros::E_CONTROLLER_DIGITAL_X;

    return miles;
}

// construct an object for the brain printing class
cmn::GUI gui;

// include the assets from the static folder used in lemlib pure pursuit, not used (yet)
ASSET(path_jerryio_left_red_v1_1_txt);
ASSET(path_jerryio_left_red_v1_2_txt);
ASSET(path_jerryio_left_red_v1_3_txt);
ASSET(path_jerryio_left_red_v1_4_txt);
ASSET(path_jerryio_left_red_v1_5_txt);
ASSET(example_txt);
ASSET(ppTest_txt);


// declare objects for the air tanks, tubes, pistons, and pneumatics components
// declarations can get tedious because for every piece of real pneumatic components that have volume, an object must be constructed in the code
cmn::Air_Tank tank1;
cmn::Air_Tank tank2;

cmn::Tube tank1_to_T;
cmn::Tube t1_main;
cmn::Tube doinker;
cmn::Tube arm;
cmn::Tube connector;
cmn::Tube intake_tb;
cmn::Tube clamp;
cmn::Tube aux;
cmn::Tube intake2;
cmn::Tube main2;

cmn::Piston ArmP;
cmn::Piston DoinkerP;
cmn::Piston intakeP;
cmn::Piston auxP;
cmn::Piston clamp1P;
cmn::Piston clamp2P;
cmn::Piston intake2P;

cmn::Pneumatics_Component ArmC;
cmn::Pneumatics_Component DoinkerC;
cmn::Pneumatics_Component AuxC;
cmn::Pneumatics_Component IntakeC;
cmn::Pneumatics_Component ClampC;
cmn::Pneumatics_Component TanksC;

cmn::Pneumatics_System p_system;

cmn::Controller_Print controller_print;

// constructs and initalizes the pose of the robot within lemlib
lemlib::Pose pose = chassis.getPose();

void initialize() {
    // start be converting then calculating the volume of all the components used
    // #must be floats because it will not work properly when trying to preform floating point operations on an int
    tank1_to_T.length = (3.0f / 2.54);
    t1_main.length = (64.0f / 2.54);
    doinker.length = (39.0f / 2.54);
    arm.length = (42.0f / 2.54);
    connector.length = ((18.0f + 19.0f + 3.0f) / 2.54);
    intake_tb.length = (24.0f / 2.54);
    clamp.length = (41.0f / 2.54);
    aux.length = (3.0f / 2.54);
    intake2.length = ((6.0f + 3.0f) / 2.54);
    main2.length = (20.0f / 2.54);

    ArmP.stroke = 75.0f;
    DoinkerP.stroke = 75.0f;
    intakeP.stroke = 75.0f;
    auxP.stroke = 75.0f;
    clamp1P.stroke = 50.0f;
    clamp2P.stroke = 50.0f;
    intake2P.stroke = 50.0f;

    // calculate the actual volumes of each object
    ArmP.init();
    arm.init();
    DoinkerP.init();
    t1_main.init();
    doinker.init();
    aux.init();
    auxP.init();
    intakeP.init();
    intake2P.init();
    intake_tb.init();
    intake2.init();
    clamp1P.init();
    clamp2P.init();
    clamp.init();
    tank1_to_T.init();
    main2.init();

    ArmC.volumes.push_back(ArmP.volume);
    ArmC.volumes.push_back(arm.volume);

    DoinkerC.volumes.push_back(DoinkerP.volume);
    //DoinkerC.volumes.push_back(t1_main.volume);
    DoinkerC.volumes.push_back(doinker.volume);

    AuxC.volumes.push_back(aux.volume);
    AuxC.volumes.push_back(auxP.volume);

    IntakeC.volumes.push_back(intakeP.volume);
    IntakeC.volumes.push_back(intake_tb.volume);
    IntakeC.volumes.push_back(intake2P.volume);
    IntakeC.volumes.push_back(intake2.volume);

    ClampC.volumes.push_back(clamp1P.volume);
    ClampC.volumes.push_back(clamp2P.volume);
    ClampC.volumes.push_back(clamp.volume);

    TanksC.volumes.push_back(tank1_to_T.volume);
    TanksC.volumes.push_back(t1_main.volume);
    TanksC.volumes.push_back(main2.volume);
    TanksC.volumes.push_back(tank2.capacity);
    TanksC.volumes.push_back(tank1.capacity);

    ArmC.compute_system_volume();
    DoinkerC.compute_system_volume();
    AuxC.compute_system_volume();
    IntakeC.compute_system_volume();
    ClampC.compute_system_volume();
    TanksC.compute_system_volume();

    p_system.system_init({ArmC, DoinkerC, AuxC, IntakeC, ClampC, TanksC});

    // prepare the screen for printing
    pros::lcd::initialize();

    // the prefered drivetrain setup by both drivers
    chassis.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    // this is where we can name individual autons for display on the brain during selection
    gui.auton_name_list.push_back("RL AWP");
    gui.auton_name_list.push_back("RR E");
    gui.auton_name_list.push_back(" ");
    gui.auton_name_list.push_back(" ");
    gui.auton_name_list.push_back(" ");
    gui.auton_name_list.push_back(" ");
    gui.auton_name_list.push_back(" ");
    gui.auton_name_list.push_back(" ");

    intake.set_encoder_units(pros::E_MOTOR_ENCODER_DEGREES);

    // a critical function in order to get odemetry to work
    chassis.calibrate();
}

void red_right_elims(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 180);

    // all the code for the autons
    // basically just scripting, not to exiting
    chassis.moveToPose(1, 13, 150, 2100, {.forwards=false, .maxSpeed= 71, .minSpeed=1, .earlyExitRange=0.25});
    chassis.moveToPose(-5, 29, 150, 1500, {.forwards=false, .maxSpeed=71});
    arm_mech.extend();
    pros::delay(1400);
    mogo_mech.extend();
    pros::delay(500);
    chassis.moveToPose(5, 18, 30, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_upper.move_velocity(200);
    intake_lower.move_velocity(200);
    chassis.moveToPose(17, 34, 30, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_lower.move_velocity(200);
    intake_upper.move_velocity(200);
    chassis.moveToPoint(-12, 24, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=3});
    chassis.moveToPoint(-33, 8, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_mech.extend();
    intake.move_velocity(200);
    pros::delay(750);
    intake.move_velocity(200);
    chassis.turnToPoint(24, 2.5, 500, {.maxSpeed=71, .minSpeed=1, .earlyExitRange=.25});
    intake_mech.retract();
    arm_mech.retract();
    intake_upper.move_velocity(200);
    intake.move_velocity(200);
    pros::delay(500);
    intake_lower.move_velocity(0);
    chassis.moveToPose(20, 6, 105, 3000, {.maxSpeed=71, .minSpeed = 31, .earlyExitRange = 1});
    intake.move_velocity(200);
    pros::delay(500);
    intake_lower.move_velocity(0);
    intake_upper.move_velocity(200);
    chassis.moveToPose(16, 12, 315, 2000, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    pros::delay(100);
    mogo_mech.retract();
    intake_upper.move_velocity(0);
    intake_upper.move_relative(-360, 200);
    chassis.moveToPose(22, 44, 180, 2750, {.forwards=false});
    

    /*chassis.setPose(-50, -30, 270);
    arm_mech.extend();
    chassis.moveToPose(-31.75, -27.75, 240, 2000, {.forwards=false, .minSpeed=31, .earlyExitRange = .5});
    chassis.moveToPose(-19, -22.5, 240, 2000, {.forwards=false, .minSpeed = 1, .earlyExitRange=.25});
    chassis.moveToPose(-33, -33.5, 180, 1200, {.minSpeed = 31, .earlyExitRange = .5});
    pros::delay(10);
    mogo_mech.extend();
    intake.move_velocity(200);
    chassis.moveToPoint(-20, -52, 2000, {.minSpeed = 28, .earlyExitRange =.5});
    intake.move_velocity(200);
    chassis.moveToPose(-33, -8, 315, 2000, {.minSpeed = 31, .earlyExitRange =1});
    intake_upper.move_velocity(200);
    chassis.moveToPose(-51, 4, 270, 2000, {.minSpeed = 1, .earlyExitRange = .1});
    intake.move_velocity(200);
    intake_mech.extend();
    chassis.moveToPose(-52, -52, 135, 3000, {.minSpeed = 31, .earlyExitRange = .1});
    intake_mech.retract();
    intake.move_velocity(200);
    mogo_mech.retract();
    chassis.moveToPose(-30, -30, 200, 5000, {.minSpeed = 1, .earlyExitRange = .5});
    chassis.moveToPose(-12, -45.5, 270, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=0.1});
    mogo_mech.retract();*/
}

void blue_left_elims(lemlib::Chassis& chassis) {
    chassis.setPose(0, 0, 180);

    // all the code for the autons
    // basically just scripting, not to exiting
    chassis.moveToPose(-1, 13, -150, 2100, {.forwards=false, .maxSpeed= 71, .minSpeed=1, .earlyExitRange=0.25});
    arm_mech.extend();
    chassis.moveToPose(7, 30, -150, 1500, {.forwards=false, .maxSpeed=71});
    pros::delay(1400);
    mogo_mech.extend();
    pros::delay(500);
    chassis.moveToPose(-5, 18, -30, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_upper.move_velocity(200);
    intake_lower.move_velocity(200);
    chassis.moveToPose(-17, 34, -30, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_lower.move_velocity(200);
    intake_upper.move_velocity(200);
    chassis.moveToPoint(12, 24, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=3});
    chassis.moveToPoint(33, 8, 2500, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    intake_mech.extend();
    intake.move_velocity(200);
    pros::delay(750);
    intake.move_velocity(200);
    chassis.turnToPoint(-24, 2.5, 500, {.maxSpeed=71, .minSpeed=1, .earlyExitRange=.25});
    intake_mech.retract();
    arm_mech.retract();
    intake_upper.move_velocity(200);
    intake.move_velocity(200);
    pros::delay(500);
    intake_lower.move_velocity(0);
    chassis.moveToPose(-20, 6, -105, 3000, {.maxSpeed=71, .minSpeed = 31, .earlyExitRange = 1});
    intake.move_velocity(200);
    pros::delay(500);
    intake_lower.move_velocity(0);
    intake_upper.move_velocity(200);
    chassis.moveToPose(-16, 12, -315, 2000, {.maxSpeed=71, .minSpeed=10, .earlyExitRange=1});
    pros::delay(100);
    mogo_mech.retract();
    intake_upper.move_velocity(0);
    intake_upper.move_relative(-360, 200);
    chassis.moveToPose(-22, 44, 180, 2750, {.forwards=false});
    

    /*chassis.setPose(-50, -30, 270);
    arm_mech.extend();
    chassis.moveToPose(-31.75, -27.75, 240, 2000, {.forwards=false, .minSpeed=31, .earlyExitRange = .5});
    chassis.moveToPose(-19, -22.5, 240, 2000, {.forwards=false, .minSpeed = 1, .earlyExitRange=.25});
    chassis.moveToPose(-33, -33.5, 180, 1200, {.minSpeed = 31, .earlyExitRange = .5});
    pros::delay(10);
    mogo_mech.extend();
    intake.move_velocity(200);
    chassis.moveToPoint(-20, -52, 2000, {.minSpeed = 28, .earlyExitRange =.5});
    intake.move_velocity(200);
    chassis.moveToPose(-33, -8, 315, 2000, {.minSpeed = 31, .earlyExitRange =1});
    intake_upper.move_velocity(200);
    chassis.moveToPose(-51, 4, 270, 2000, {.minSpeed = 1, .earlyExitRange = .1});
    intake.move_velocity(200);
    intake_mech.extend();
    chassis.moveToPose(-52, -52, 135, 3000, {.minSpeed = 31, .earlyExitRange = .1});
    intake_mech.retract();
    intake.move_velocity(200);
    mogo_mech.retract();
    chassis.moveToPose(-30, -30, 200, 5000, {.minSpeed = 1, .earlyExitRange = .5});
    chassis.moveToPose(-12, -45.5, 270, 5000, {.forwards=false, .minSpeed=1, .earlyExitRange=0.1});
    mogo_mech.retract();*/
}

void autonomous() {
    // Switch the brain to displaying current motor temperature status in preperation for the match
    gui.current_view = 1;

    blue_left_elims(chassis);

    // determines which autons run based on the one selected on the previous screen
    /*switch (gui.selected_auton) {
        case 0:
            red_right_elims(chassis);
            break;
        case 1:
            blue_left_elims(chassis);
            break;
        default:
            break;
        
    }*/
}

void opcontrol() {

    gui.current_view = 1;

    // gets a baseline for when opcontrol starts
    unsigned int current_time = pros::millis();

    // constructs objects, initializes objects, etc...
    bool arm_toggle = 0;

    cmn::my_controller chase;
    cmn::my_controller miles;

    chase.controller = &controller_1;
    miles.controller = &controller_2;

    chase = chase_config(chase);
    miles = miles_config(miles);

    chase.is_master_controller = true;
    miles.is_master_controller = false;

    cmn::my_controller master;
    cmn::my_controller slave;

    gui.motor_list.push_back(lm1);
    gui.motor_list.push_back(lm2);
    gui.motor_list.push_back(lm3);
    gui.motor_list.push_back(rm1);
    gui.motor_list.push_back(rm2);
    gui.motor_list.push_back(rm3);
    gui.motor_list.push_back(intake_lower);
    gui.motor_list.push_back(intake_upper);

    master = get_active_controller(chase, miles)[0];
    slave = get_active_controller(chase, miles)[1];

    controller_print.cont_drivetrain.push_back(lm1);
    controller_print.cont_drivetrain.push_back(lm2);
    controller_print.cont_drivetrain.push_back(lm3);
    controller_print.cont_drivetrain.push_back(rm1);
    controller_print.cont_drivetrain.push_back(rm2);
    controller_print.cont_drivetrain.push_back(rm3);

    controller_print.init(chase, miles);

    // runs until the program is turned off
	while(1){
        // finds the current time relative to when the opcontrol function was called relitive to the initalialize funtion
        unsigned int opcontrol_time = pros::millis() - current_time;
        // set the current pneumatics for printing on the controller
        controller_print.p_sys = p_system;
        // creates an inverse of opcontrol_time for keeping track of remaining time during a match
        unsigned int time_left;

        if(opcontrol_time > 105000) {
            time_left = 0;
        } else {
            time_left = 105000 - opcontrol_time;
        }

        controller_print.time = time_left;

        // a check to see which controller is the one controlling the robot and to see which one is reciving the stats
        if(master.controller->get_digital(master.switch_controller_button)) {
            chase.is_master_controller = !chase.is_master_controller;
            miles.is_master_controller = !miles.is_master_controller;
            master = get_active_controller(chase, miles)[0];
            slave = get_active_controller(chase, miles)[1];
            master.controller->rumble("-");
            chase.controller->clear();
            miles.controller->clear();
        }

        // a hacky way to get the "master" and "slave"
        master = get_active_controller(chase, miles)[0];
        slave = get_active_controller(chase, miles)[1];

        // all of the basic robot control scriping

        double throttle, steer;

        throttle = master.controller->get_analog(master.throttle_axis);
        steer = master.controller->get_analog(master.steer_axis);

        // uses curvature drive when not at slow speeds, otherwise uses arcarde drive
        if((throttle > 2) && (steer > 2)){
		    chassis.curvature(steer, throttle);
        } else {
            chassis.arcade(steer, throttle);
        }

        if(master.controller->get_digital(master.arm_pneumatic_button)) {
            arm_mech.toggle();
            p_system.cycled(ArmC);
            while(master.controller->get_digital(master.arm_pneumatic_button)) {
                pros::delay(2);
            }
        }

        if(master.controller->get_digital(master.wrist_pneumatic_button)) {
            wrist_mech.toggle();
            p_system.cycled(AuxC);
            while(master.controller->get_digital(master.wrist_pneumatic_button)) {
                pros::delay(2);
            }
        }

        if(master.controller->get_digital(master.intake_pneumatic_button)) {
            intake_mech.toggle();
            p_system.cycled(IntakeC);
            while(master.controller->get_digital(master.intake_pneumatic_button)) {
                pros::delay(2);
            }
        }

        if(master.controller->get_digital(master.mogo_pneumatic_button)) {
            mogo_mech.toggle();
            if(mogo_mech.is_extended() == false) {
                intake_upper.move_relative(-600.0f, 200);
            }
            p_system.cycled(ClampC);
            while(master.controller->get_digital(master.mogo_pneumatic_button)) {
                pros::delay(2);
            }
        }

        if (master.controller->get_digital(master.intake_motor_button)) {
            intake.move_velocity(200);
        } else if (master.controller->get_digital(master.extake_motor_button)) {
            intake.move_velocity(-200);
        } else {
            intake.move_velocity(0);
        }

        if (master.controller->get_digital(master.chopper_mech)) {
            chopper_mech.extend();
            p_system.cycled(DoinkerC);
        } else {
            chopper_mech.retract();
        }

        // renders the brain
        gui.render();

        // renders the controllers
        controller_print.print_master(master);
        controller_print.print_slave(slave);

        // would be more important if we had thread scheduling, but for now its just there as good practice
		pros::delay(15);
	}
}