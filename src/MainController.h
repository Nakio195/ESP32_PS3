/*
 * MainController.h
 *
 *  Created on: 31 janv. 2025
 *      Author: To
 */

#ifndef MAINCONTROLLER_H_
#define MAINCONTROLLER_H_

#include "Arduino.h"

#define STICK_DEADZONE 10


class MainController {
	public:
		MainController();
		virtual ~MainController();
		static void update();
		static void onConnect();
		static void onDisconnect();
		uint8_t* getControllerData();
		uint8_t* getControllerStatus();



	public:
		enum BatteryLevels{Undefined, Shutdown, Dying, Low, High, Full, Charging};
		//Analog stick
		int8_t	L_Stick_x;
		int8_t	L_Stick_y;
		int8_t	R_Stick_x;
		int8_t	R_Stick_y;
		// Analog Triggers
		int8_t L2_Trigger;
		int8_t R2_Trigger;
		// Symbols buttons
		bool Cross;
		bool Square;
		bool Triangle;
		bool Circle;
		// DPAD buttons
		bool DPAD_L;
		bool DPAD_U;
		bool DPAD_R;
		bool DPAD_D;
		//LR Buttons
		bool L1;
		bool R1;
		bool L3;
		bool R3;
		// Control buttons
		bool Start;
		bool Select;
		bool PlayStation;
		//Status information
		bool isConnected;
		uint8_t BatteryLevel;
		uint8_t LedControl;

		// Analog DeadZones
		bool L_AnalogDeadZone;
		bool R_AnalogDeadZone;
		bool L_TriggerDeadZone;
		bool R_TriggerDeadZone;

	private:
		uint8_t ControllerData[8];
		uint8_t ControllerStatus[6];
};

extern MainController Controller;

#endif /* MAINCONTROLLER_H_ */
