#include "main.h"
#include "autoSelect/selection.h"


void initialize() {
	selector::init();
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {
	if(selector::auton == 1){
		//Red Front
	}else if(selector::auton == 2){
		//Red Back
	}else if(selector::auton == 3){
		//Nothing
	}else if(selector::auton == -1){
		//Blue Front
	}else if(selector::auton == -2){
		//Blue Back
	}else if(selector::auton == -3){
		//Nothing
	}else if(selector::auton == 0){
		//Skills
	}
}

void opcontrol() {
	while (true) {
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		left_mtr = left;
		right_mtr = right;

		pros::delay(20);
	}
}
