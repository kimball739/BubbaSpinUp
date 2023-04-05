#include "main.h"
#include "autoSelect/selection.h"


void initialize() {
	selector::init();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	//tracking wheel odom
	///*
	std::shared_ptr<OdomChassisController> chassis =
        ChassisControllerBuilder()
            .withMotors({-1, 2, -3}, {4, -5, 6})
            // blue cartridge, 36:84 external gearing, 4.125 in wheel diam, 10.25 in wheel track
			.withDimensions({AbstractMotor::gearset::blue, (36.0 / 84.0)}, {{4.125_in, 10.25_in}, imev5GreenTPR})
			.withSensors(RotationSensor{7}, RotationSensor{8, true})
			.withOdometry({{1.5625_in, 7_in}, quadEncoderTPR})
			.buildOdometry();
	//*/
	/*
	//integrated encoder odom
	std::shared_ptr<OdomChassisController> chassis =
        ChassisControllerBuilder()
            .withMotors({-1, 2, -3}, {4, -5, 6})
			.withGains(
				{0.001, 0, 0.0001}, // Distance controller gains
				{0.001, 0, 0.0001}, // Turn controller gains
				{0.001, 0, 0.0001}  // Angle controller gains (helps drive straight)
			)
            // blue cartridge, 36:84 external gearing, 4.125 in wheel diam, 10.25 in wheel track
			.withDimensions({AbstractMotor::gearset::blue, (36.0 / 84.0)}, {{4.125_in, 10.25_in}, imev5GreenTPR})
			.withOdometry(okapi::StateMode::CARTESIAN)
			.buildOdometry();
	*/
	chassis->setState({0_in, 0_in, 0_deg});
	switch(selector::auton){
		case 1:
			//red left
			chassis->driveToPoint({1_ft, 1_ft});
			chassis->driveToPoint({0_ft, 2_ft});
			chassis->turnToAngle(0_deg);
		break;
		case 2:
		//red right
		break;
		case 3:
		//red nothing
		break;
		case -1:
		//blue left
		break;
		case -2:
		//blue right
		break;
		case -3:
		//blue nothing
		break;
		case 0:
		//skills
		break;
	}
}

void opcontrol() {
	Controller controller;
	std::shared_ptr<ChassisController> drive =
        ChassisControllerBuilder()
            .withMotors({-1, 2, -3}, {4, -5, 6})
            // blue cartridge, 36:84 external gearing, 4 in wheel diam, 11.5 in wheel track
			.withDimensions({AbstractMotor::gearset::blue, (36.0 / 84.0)}, {{4.125_in, 10.25_in}, imev5GreenTPR})
            .build();

	while (true) {
		drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
        	                  	controller.getAnalog(ControllerAnalog::rightY));
		pros::delay(20);
	}
}
