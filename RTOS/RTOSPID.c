/*
 * RTOSPID.c
 *
 *  Created on: Oct 11, 2021
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <xdc/runtime/System.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

#include "driverlib/sysctl.h"// to enable ports
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include <time.h>
#include <xdc/std.h>
#include <RTOS/RTOSPID.h>
#include <RTOS/RTOSBluetooth.h>
#include <RTOS/RTOSMotor.h>
#include <RTOS/RTOSDistance.h>
#include <RTOS/RTOSLightSensor.h>
#include <RTOS/RTOSModbus.h>
#include <RTOS/RTOSFlashing.h>

int32_t RTOSPIDSetting[20];
enum PIDEnum {PIDKp, PIDKi, PIDKd, PIDRightOffset, PIDFrontOffset, PIDPWMRight, PIDPWMLeft, PIDPWMMin, PIDPWMMax};
int32_t PIDPreError, PIDError, PIDtotalerror;
int32_t temp_p, temp_i, temp_d;
int32_t temp_dr, temp_df; // use to store error of PID
uint32_t RTOSDistanceFront, RTOSDistanceRight; // store data of distance, unit 10x[mm]
// temp_df = store distance to front
// temp_dr =  store distance to right
int PIDPWMAdjust;
int8_t RTOSPIDFlag = 0;
enum PIDStateConst { PIDSTATESTOPALL, PIDSTATEALONGWALL, PIDSTATETURNRIGHT, PIDSTATETURNAROUND };
uint8_t PIDState; // set the state of robot
uint16_t iloop = 0;

void RTOSPIDInit(void){
	// Init Values of Settings
	int i = 0;
	for (i = 0; i < 20; i++) {
		RTOSPIDSetting[i] = 0;
	}

	RTOSPIDSetting[PIDPWMMin] = 2000;
	RTOSPIDSetting[PIDPWMMax] = 4500; //5500
	RTOSPIDSetting[PIDPWMRight] = RTOSPIDSetting[PIDPWMMin];
	RTOSPIDSetting[PIDPWMLeft] = RTOSPIDSetting[PIDPWMMin];

	RTOSPIDSetting[PIDRightOffset] = 800; // 10x[milimeter]
	RTOSPIDSetting[PIDKp] = 6; RTOSPIDSetting[PIDKi] = 1; RTOSPIDSetting[PIDKd] = 3;
	// Ki should be smallest, Kd and Ki is smaller than Kp
	PIDPreError = PIDError = PIDtotalerror = 0;
	RTOSDistanceFront = RTOSDistanceRight = 0;
	// default robot state is stopped
	PIDState = PIDSTATESTOPALL;
}

// enum PIDEnum {PIDPropotional, PIDIntegral, PIDDerivative, PIDRightOffset, PIDFrontOffset, PIDPWMRight, PIDPWMLeft};
void RTOSPIDGetSettings(char* s) {

	System_sprintf(s, "\n\rPID values: %d %d %d \n\rRightoffset: %d\n\rFrontOffset: %d \n\rPWM Right: %d\n\rPWM Left: %d\n\rMin PWM: %d \n\rMax PWM: %d\n\rPID Error: %d \n\rPID Adj: %d \n\r",
			RTOSPIDSetting[PIDKp], RTOSPIDSetting[PIDKi],  RTOSPIDSetting[PIDKd],
			RTOSPIDSetting[PIDRightOffset], RTOSPIDSetting[PIDFrontOffset],
			RTOSPIDSetting[PIDPWMRight],RTOSPIDSetting[PIDPWMLeft], RTOSPIDSetting[PIDPWMMin], RTOSPIDSetting[PIDPWMMax],
			PIDError, PIDPWMAdjust);

}

void RTOSPIDSetSetting(int32_t type, int32_t value) {
	RTOSPIDSetting[type] = value;
}

void RTOSPIDTuneP(int8_t v) {
	RTOSPIDSetting[PIDKp] += v;
}
void RTOSPIDTuneD(int8_t v) {
	RTOSPIDSetting[PIDKd] += v;
}
void RTOSPIDTuneI(int8_t v) {
	RTOSPIDSetting[PIDKi] += v;
}

void RTOSPIDGet(int* p, int* i, int* d) {
	*p = RTOSPIDSetting[PIDKp];
	*i = RTOSPIDSetting[PIDKi];
	*d = RTOSPIDSetting[PIDKd];
}

// this state for the robot run a long a wall with a distance
void RTOSPIDRunAlongWall() {
	// BEGIN PID CONTROL
	//allow error within 2mm or 20 [10xmm]
	// PIDError = Right - Offset:  > 0 when it is far away from wall
	if (PIDError > 2) {
		// left wheel > right wheel
		// choose left wheel max, and right wheel decreasing
		RTOSPIDSetting[PIDPWMLeft] = RTOSPIDSetting[PIDPWMMax];
		RTOSPIDSetting[PIDPWMRight] -= abs(PIDPWMAdjust);
		//RTOSPIDSetting[PIDPWMRight] = RTOSPIDSetting[PIDPWMMin];
	}
	else if (PIDError < -2) { // PIDError < 0 when it is closed to the wall
		// rightwheel > left wheel
		// choose right wheel max, adjust left wheel
		RTOSPIDSetting[PIDPWMRight] = RTOSPIDSetting[PIDPWMMax];
		RTOSPIDSetting[PIDPWMLeft] -= abs(PIDPWMAdjust);
		//RTOSPIDSetting[PIDPWMLeft] = RTOSPIDSetting[PIDPWMMin];
	}
	// END PID CONTROL

	// range is between PWM Min and PWM Max
	// RTOSPIDSetting[PIDPWMMin], RTOSPIDSetting[PIDPWMMax]
	if (RTOSPIDSetting[PIDPWMRight] > RTOSPIDSetting[PIDPWMMax]) RTOSPIDSetting[PIDPWMRight] = RTOSPIDSetting[PIDPWMMax];
	if (RTOSPIDSetting[PIDPWMRight] < RTOSPIDSetting[PIDPWMMin]) RTOSPIDSetting[PIDPWMRight] = RTOSPIDSetting[PIDPWMMin];
	if (RTOSPIDSetting[PIDPWMLeft] > RTOSPIDSetting[PIDPWMMax]) RTOSPIDSetting[PIDPWMLeft] = RTOSPIDSetting[PIDPWMMax];
	if (RTOSPIDSetting[PIDPWMLeft] < RTOSPIDSetting[PIDPWMMin]) RTOSPIDSetting[PIDPWMLeft] = RTOSPIDSetting[PIDPWMMin];
} // end RTOSPIDRunAlongWall

// the robot will make a right turn
void RTOSPIDRightTurn() {
	RTOSPIDSetting[PIDPWMRight] = 800;
	RTOSPIDSetting[PIDPWMLeft] = 8500;
} // end RTOSPIDRightTurn

// the robot keep turning left until find a path (u-turn or turn left)
// return true when the new path is found
bool RTOSPIDTurnAround() {
	// perform turn left
	// slowly uturn until front greater than 2000 (20cm)
	// use d ml and d mr to find out the value for the motors
	RTOSPIDSetting[PIDPWMRight] = 2000;
	RTOSPIDSetting[PIDPWMLeft] = -6000;
	if (RTOSDistanceFront > 1500 && RTOSDistanceRight > 500) {
		// new path is found
		return true;
	}

	return false;

} // end RTOSPIDTurnAround


void RTOSPIDCompute() {
	// get distance sensor
	RTOSDistanceGet(&RTOSDistanceFront, &RTOSDistanceRight);
	PIDError = RTOSDistanceRight -  RTOSPIDSetting[PIDRightOffset]; // 10x[mm]
	// PIDError < 0: temp_dr < PIDRightOffset: when it is far away from the wall
	// PIDError > 0: temp_dr > PIDRightOffset: when it is too closed from the wall
    //PIDtotalerror += PIDError;
	temp_p = RTOSPIDSetting[PIDKp] * PIDError; // negative when far away
	temp_i = RTOSPIDSetting[PIDKi] * (PIDError + PIDPreError);
	temp_d = RTOSPIDSetting[PIDKd] * (PIDError - PIDPreError);
	PIDPWMAdjust = (temp_p + temp_i + temp_d);

	// send error to modbus
	if(RTOSLightGetLightCount() == 1) { // only send data when the first line is crossed
		// LED values - 2=RED, 4=BLUE, 8=GREEN
		if (iloop >= 20) {
			// recalculate every 5ms
			// get error data each 100ms => 100 / 5 = 20
			iloop = 0; // reset the loop

			if (RTOSModbusPushData(abs(PIDError/10))) { // /10 to get mm
				RTOSFlashingSetColor(8);
				RTOSModbusPrint();
			}
		}
	}

	// update Error
	PIDPreError = PIDError;

	// SET STATE OF ROBOT
	//enum PIDStateConst { PIDSTATESTOPALL, PIDSTATEALONGWALL, PIDSTATETURNRIGHT, PIDSTATETURNAROUND };
	switch (PIDState) {
		case PIDSTATEALONGWALL:
			// robot use the right sensor to keep a constant distance to the wall
			RTOSPIDRunAlongWall();
			break;
		case PIDSTATETURNRIGHT:
			// when sensor detects a possible way to turn right, robot will make a turn right
			RTOSPIDRightTurn();
			break;
		case PIDSTATETURNAROUND:
			// robot keep turning left to detect clear path
			if(RTOSPIDTurnAround())
				PIDState = PIDSTATEALONGWALL;
			else
				return;
			// new path still not found, return to prevent the state change
			break;
		default:
			break;
	}

	// change state
	if (RTOSDistanceFront < 1000 && RTOSDistanceRight < 1200) //  10*60m need to stop
	{
		RTOSMotorStop();
		PIDState = PIDSTATETURNAROUND;
	}
	else if (RTOSDistanceRight > 1200) {
		//  change to right turn mode when right distance > 15cm
		 PIDState = PIDSTATETURNRIGHT;
	}
	else {
		PIDState = PIDSTATEALONGWALL;
	}

	iloop++; // increase to count number of loop
} // end RTOSPIDCompute

// Use to enable the robot
void RTOSPIDPIDEnable(int8_t flag) {
	if (flag == 1) {
		RTOSPIDSetting[PIDPWMRight] = 3000;
		RTOSPIDSetting[PIDPWMLeft] = 3000;
		PIDState = PIDSTATEALONGWALL;

	} else {
		// stop the robot
		PIDState = PIDSTATESTOPALL;
		RTOSMotorStop();
	}
} // end RTOSPIDPIDEnable


void RTOSPIDRun() {
	// clear timer interrupt
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

	if (PIDState != PIDSTATESTOPALL) {
		RTOSPIDCompute();
		RTOSMotorDriveRight(RTOSPIDSetting[PIDPWMRight]);
		RTOSMotorDriveLeft(RTOSPIDSetting[PIDPWMLeft]);
	}
}

