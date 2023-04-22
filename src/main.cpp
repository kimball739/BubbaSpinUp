#include "main.h"
#include "autoSelect/selection.h"

bool shoot=0; //for auton and opcontrol to request catapult launch

void catapultControlFn(){ //this task continuously runs to automatically reset catapult
	//define catapult, its limit switches, and boost cylinder
		Motor catapult(19, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
		ADIButton limitSwitchBottom('A');
		ADIButton limitSwitchTop('B');
		pros::ADIDigitalOut boostCylinder ('F', 0);

	//main loop
		while(1){
			//shoots and rewwinds catapult
				if(!limitSwitchBottom.isPressed() || shoot==1){
					catapult.moveVoltage(11000);
				}else{
					catapult.moveVoltage(0);
				}

			//resets shoot boolean
				if(shoot==1 && !limitSwitchBottom.isPressed()){
					shoot=0;
				}
			//turns off boost
				if(limitSwitchTop.changedToPressed()){
					boostCylinder.set_value(0);
				}

			pros::delay(20);
		}
}

void initialize() {
	selector::init(); //auton selector
	pros::Task catapultControl(catapultControlFn); //starts catapult task
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	//3 wire devices
		//bottom limit switch is in port A, but is not used in auton
		//top limit switch is in port B, but is not used in auton
		pros::ADIDigitalOut expansionCylinder ('C', 0); //0 is expansion latched, 1 is expansion released
		pros::ADIDigitalOut intakeCylinder ('D', 0); //0 is intake up, 1 is intake down
		pros::ADIDigitalOut trackingCylinder ('E', 1); //0 is wheels up, 1 is wheels down
		pros::ADIDigitalOut boostCylinder ('F', 0); //0 is no boost, 1 is boost

	//motors
		//Catapult is run through the catapult control task so it is not defined here
		Motor intake(10, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
	
	//drive
		//this is the chassis constructor with pid and odom for autonomous
		std::shared_ptr<OdomChassisController> bot =
			ChassisControllerBuilder()
				.withMotors({-14, -15, 16}, {11, 12, -13})
				//blue cartridge, 36:84 external gearing, 4.125 in wheel diam, 10.25 in wheel track
				.withGains(
					{0.0006, 0.0, 0.00000}, //distance controller gains
					{0.00115, 0.0, 0}, //turn controller gains
					{0.0003, 0.0, 0.000}  //drive straight controller gains
				)
				.withSensors(RotationSensor{17}, RotationSensor{18, true})
				.withDimensions({AbstractMotor::gearset::blue, (36.0 / 84.0)}, {{4.125_in, 10.25_in}, imev5GreenTPR})
				.withOdometry({{1.5625_in, 6.8125_in}, quadEncoderTPR})
				.buildOdometry();
	
	bot->setState({0_in, 0_in, 173_deg}); //sets robot position

	switch(selector::auton){ //runs the program that was selected on the screen (red near is default)
		case 1: //red near
			boostCylinder.set_value(1);
			shoot=1;
			pros::delay(100);
			bot->moveDistance(5.9_in);
			intake.moveRelative(500, 200);
			pros::delay(320);
			bot->moveDistance(-5.9_in);
			intake.moveVoltage(11000);
			bot->driveToPoint({9.2_in, 24_in});
			intakeCylinder.set_value(1);
			pros::delay(1500);
			intakeCylinder.set_value(0);
			bot->driveToPoint({9_in, 27 _in}, true);
			intake.moveVoltage(0);
			intakeCylinder.set_value(1);
			boostCylinder.set_value(1);
			shoot=1;
			break;
		case 2: //red far
			break;
		case 3: //red none
			break;
		case -1: //blue near
			break;
		case -2: //blue far
			break;
		case -3: //blue none
			break;
		case 0: //skills
			break;
	}
}

void opcontrol() {
	//controller and its buttons
	Controller controller;
		ControllerButton shootButton(ControllerDigital::L1);
		ControllerButton intakeCylinderButton(ControllerDigital::L2);
		ControllerButton intakeButton(ControllerDigital::R1);
		ControllerButton outakeButton(ControllerDigital::R2);
		ControllerButton boostButton(ControllerDigital::up);
		ControllerButton expansionButton(ControllerDigital::A);

	//3 wire devices
		ADIButton limitSwitchBottom('A');
		//top limit switch is in port B, but is not used in opcontrol
		pros::ADIDigitalOut expansionCylinder ('C', 0); //0 is expansion latched, 1 is expansion released
		pros::ADIDigitalOut intakeCylinder ('D', 1); //0 is intake up, 1 is intake down
		pros::ADIDigitalOut trackingCylinder ('E', 0); //0 is wheels up, 1 is wheels down
		pros::ADIDigitalOut boostCylinder ('F', 0); //0 is no boost, 1 is boost

	//motors
		//Catapult is run through the catapult control task so it is not defined here
		Motor intake(10, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
		
	//drive
		//this is a basic chassis constructor for operator control
		std::shared_ptr<ChassisController> drive =
			ChassisControllerBuilder()
				.withMotors({-14, -15, 16}, {11, 12, -13})
				// blue cartridge, 36:84 external gearing, 4 in wheel diam, 11.5 in wheel track
				.withDimensions({AbstractMotor::gearset::blue, (36.0 / 84.0)}, {{4.125_in, 10.25_in}, imev5GreenTPR})
				.build();

	//main loop
	while (true) {
		//tank drive
			drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY), 
									controller.getAnalog(ControllerAnalog::rightY));

		//catapult
			if(boostButton.isPressed()){ //shoot with boosting
				shoot=1;
				boostCylinder.set_value(1);	
			}else if(shootButton.isPressed()){ //shoot without boosting
				shoot=1;
			}

		//intake
			//spin intake
			if(outakeButton.isPressed()){ //spins the intake outwards
				intake.moveVoltage(-10000);
			}else if(intakeButton.isPressed() && limitSwitchBottom.isPressed()){ //spins the intake in but only if the catapult is down
				intake.moveVoltage(10000);
			}else{
				intake.moveVoltage(0); //stops intake
			}

			//lift/lower intake
			if(intakeCylinderButton.isPressed()){ //lifts intake up
				intakeCylinder.set_value(0);
			}else{
				intakeCylinder.set_value(1); //lowers intake
			}

		//expansion
			if(expansionButton.isPressed()){ //releases expansion latch
				expansionCylinder.set_value(1); 
			}else{
				expansionCylinder.set_value(0); //closes latch
			}

		pros::delay(20);
	}
}