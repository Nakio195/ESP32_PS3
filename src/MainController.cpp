/*
 * MainController.cpp
 *
 *  Created on: 31 janv. 2025
 *      Author: To
 */

#include "MainController.h"
#include <Ps3Controller.h>

MainController Controller;

MainController::MainController() {
	// TODO Auto-generated constructor stub
	L_Stick_x = 0;
	L_Stick_y = 0;
	R_Stick_x = 0;
	R_Stick_y = 0;
	// Analog Triggers
	L2_Trigger = 0;
	R2_Trigger = 0;
	// Symbols buttons
	Cross = 0;
	Square = 0;
	Triangle = 0;
	Circle = 0;
	// DPAD buttons
	DPAD_L = 0;
	DPAD_U = 0;
	DPAD_R = 0;
	DPAD_D = 0;
	//LR Buttons
	L1 = 0;
	R1 = 0;
	L3 = 0;
	R3 = 0;
	// Control buttons
	Start = 0;
	Select = 0;
	PlayStation = 0;
	//Status information
	isConnected = 0;
	BatteryLevel = 0;
	LedControl = 0;

	// Analog DeadZones
	L_AnalogDeadZone = 0;
	R_AnalogDeadZone = 0;
	L_TriggerDeadZone = 0;
	R_TriggerDeadZone = 0;

}

MainController::~MainController() {
	// TODO Auto-generated destructor stub
}

uint8_t* MainController::getControllerData()
{
	ControllerData[0] = L_Stick_x;
	ControllerData[1] = L_Stick_y;
	ControllerData[2] = R_Stick_x;
	ControllerData[3] = R_Stick_y;
	ControllerData[4] = L2_Trigger;
	ControllerData[5] = R2_Trigger;

	//Symbols and DPAD buttons
	ControllerData[6] = 	Cross |
								Square 		<< 1 |
								Triangle 	<< 2 |
								Circle 		<< 3 |
								DPAD_L 		<< 4 |
								DPAD_U 		<< 5 |
								DPAD_R 		<< 6 |
								DPAD_D 		<< 7;

	ControllerData[7] =  L1 |
								R1 			<< 1 |
								L3 			<< 2 |
								R3 			<< 3 |
								Start 		<< 4 |
								Select 		<< 5 |
								PlayStation	<< 6 |
								isConnected << 7;
	return ControllerData;
}

uint8_t* MainController::getControllerStatus()
{
	ControllerStatus[0] = BatteryLevel;
	ControllerStatus[1] = isConnected;

	uint32_t timestamp = millis();
	ControllerStatus[2] = timestamp >> 24 & 0xFF;
	ControllerStatus[3] = timestamp >> 16 & 0xFF;
	ControllerStatus[4] = timestamp >> 8 & 0xFF;
	ControllerStatus[5] = timestamp & 0xFF;

	return ControllerStatus;
}

void MainController::onConnect()
{
	Controller.isConnected = true;
}

void MainController::onDisconnect()
{
	Controller.isConnected = false;
}

void MainController::update()
{
    //--- Digital cross/square/triangle/circle button events ---
    if( Ps3.event.button_down.cross )
        Controller.Cross = true;
    if( Ps3.event.button_up.cross )
    	Controller.Cross = false;

    if( Ps3.event.button_down.square )
    	Controller.Square = true;
    if( Ps3.event.button_up.square )
    	Controller.Square = false;

    if( Ps3.event.button_down.triangle )
    	Controller.Triangle = true;
    if( Ps3.event.button_up.triangle )
    	Controller.Triangle = false;

    if( Ps3.event.button_down.circle )
    	Controller.Circle = true;
    if( Ps3.event.button_up.circle )
    	Controller.Circle = false;

    //--------------- Digital D-pad button events --------------
    if( Ps3.event.button_down.up )
    	Controller.DPAD_U = true;
    if( Ps3.event.button_up.up )
    	Controller.DPAD_U = false;

    if( Ps3.event.button_down.right )
    	Controller.DPAD_R = true;
    if( Ps3.event.button_up.right )
    	Controller.DPAD_R = false;

    if( Ps3.event.button_down.down )
    	Controller.DPAD_D = true;
    if( Ps3.event.button_up.down )
    	Controller.DPAD_D = false;

    if( Ps3.event.button_down.left )
    	Controller.DPAD_L = true;
    if( Ps3.event.button_up.left )
    	Controller.DPAD_L = false;

    //------------- Digital shoulder button events -------------
    if( Ps3.event.button_down.l1 )
    	Controller.L1 = true;
    if( Ps3.event.button_up.l1 )
    	Controller.L1 = false;

    if( Ps3.event.button_down.r1 )
    	Controller.R1 = true;
    if( Ps3.event.button_up.r1 )
    	Controller.R1 = false;

    //--------------- Digital stick button events --------------
    if( Ps3.event.button_down.l3 )
    	Controller.L3 = true;
    if( Ps3.event.button_up.l3 )
    	Controller.L3 = false;

    if( Ps3.event.button_down.r3 )
    	Controller.R3 = true;
    if( Ps3.event.button_up.r3 )
    	Controller.R3 = false;

    //---------- Digital select/start/ps button events ---------
    if( Ps3.event.button_down.select )
    	Controller.Select = true;
    if( Ps3.event.button_up.select )
    	Controller.Select = false;

    if( Ps3.event.button_down.start )
    	Controller.Start = true;
    if( Ps3.event.button_up.start )
    	Controller.Start = false;

    if( Ps3.event.button_down.ps )
    	Controller.PlayStation = true;
    if( Ps3.event.button_up.ps )
    	Controller.PlayStation = false;


    //---------------- Analog stick value events ---------------
   if( abs(Ps3.event.analog_changed.stick.lx) || abs(Ps3.event.analog_changed.stick.ly))
   {
	   if(abs(Ps3.data.analog.stick.lx) > STICK_DEADZONE || abs(Ps3.data.analog.stick.ly) > STICK_DEADZONE)
	   {
		   Controller.L_Stick_x = Ps3.data.analog.stick.lx;
		   Controller.L_Stick_y = Ps3.data.analog.stick.ly * -1;
		   Controller.L_AnalogDeadZone = false;
	   }

	   else
	   {
		   if(!Controller.L_AnalogDeadZone)
		   {
			   Controller.L_Stick_x = 0;
			   Controller.L_Stick_y = 0;
		   }

		   Controller.L_AnalogDeadZone = true;
	   }
   }

   if( abs(Ps3.event.analog_changed.stick.rx) || abs(Ps3.event.analog_changed.stick.ry))
   {
	   if(abs(Ps3.data.analog.stick.rx) > STICK_DEADZONE || abs(Ps3.data.analog.stick.ry) > STICK_DEADZONE)
	   {
		   Controller.R_Stick_x = Ps3.data.analog.stick.rx;
		   Controller.R_Stick_y = Ps3.data.analog.stick.ry * -1;
		   Controller.R_AnalogDeadZone = false;
	   }

	   else
	   {
		   if(!Controller.R_AnalogDeadZone)
		   {
			   Controller.R_Stick_x = 0;
			   Controller.R_Stick_y = 0;
		   }

		   Controller.R_AnalogDeadZone = true;
	   }
   }

   //---------------------- Triggers events --------------------

   if(abs(Ps3.event.analog_changed.button.l2))
   {
	   if(abs(Ps3.data.analog.button.l2) > STICK_DEADZONE || abs(Ps3.data.analog.button.l2) > STICK_DEADZONE)
	   {
		   Controller.L2_Trigger = Ps3.data.analog.button.l2;
		   Controller.L_TriggerDeadZone = false;
	   }

	   else
	   {
		   if(!Controller.L_TriggerDeadZone)
			   Controller.L2_Trigger = 0;

		   Controller.L_TriggerDeadZone = true;
	   }
   }


   if(abs(Ps3.event.analog_changed.button.r2))
   {
	   if(abs(Ps3.data.analog.button.r2) > STICK_DEADZONE || abs(Ps3.data.analog.button.r2) > STICK_DEADZONE)
	   {
		   Controller.R2_Trigger = Ps3.data.analog.button.r2;
		   Controller.R_TriggerDeadZone = false;
	   }

	   else
	   {
		   if(!Controller.R_TriggerDeadZone)
			   Controller.R2_Trigger = 0;

		   Controller.R_TriggerDeadZone = true;
	   }
   }

   //---------------------- Battery events ---------------------
   int battery = Controller.BatteryLevel;
    if( battery != Ps3.data.status.battery ){
        battery = Ps3.data.status.battery;
        if( battery == ps3_status_battery_charging )      Controller.BatteryLevel = MainController::Charging;
        else if( battery == ps3_status_battery_full )     Controller.BatteryLevel = MainController::Full;
        else if( battery == ps3_status_battery_high )     Controller.BatteryLevel = MainController::High;
        else if( battery == ps3_status_battery_low)       Controller.BatteryLevel = MainController::Low;
        else if( battery == ps3_status_battery_dying )    Controller.BatteryLevel = MainController::Dying;
        else if( battery == ps3_status_battery_shutdown ) Controller.BatteryLevel = MainController::Shutdown;
        else Controller.BatteryLevel = MainController::Undefined;
    }

}
