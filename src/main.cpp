#include "main.h"
#include "autoSelect/selection.h"


void initialize() {
	selector::init();
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	switch(selector::auton){
		case 1:
		//red left
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
			//.withSensors(RotationSensor{7}, RotationSensor{8, true})
			//.withOdometry()
			//.buildOdometry()
            .build();

	while (true) {
		drive->getModel()->tank(controller.getAnalog(ControllerAnalog::leftY),
                            	controller.getAnalog(ControllerAnalog::rightY));

		pros::delay(20);
	}
}
