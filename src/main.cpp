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
	Controller master(pros::ONTROLLER_MASTER);
	Motor_Group left_drive ({1, -2, 3}, MOTOR_GEARSET_6*36/84, MOTOR_ENCODER_DEGREES);
	Motor_Group right_drive ({-4, 5, -6}, MOTOR_GEARSET_6*36/84, MOTOR_ENCODER_DEGREES);
	//pros::Motor left_mtr(1);
	//pros::Motor right_mtr(2);

	while (true) {
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_drive = left;
		right_drive = right;

		pros::delay(20);
	}
}
