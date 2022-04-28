/*
 * RTOSLightSensor.c
 *
 *  Created on: Nov 8, 2021
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include <xdc/runtime/System.h>

#include <RTOS/RTOSLightSensor.h>
#include <RTOS/RTOSBluetooth.h>
#include <RTOS/RTOSCommander.h>
#include <RTOS/RTOSFlashing.h>
#include <RTOS/RTOSModbus.h>


uint32_t RTOSLightDef = 20000; // thredhold value between white vs black, white ~ 8000, Black ~ 40,000
uint32_t RTOSLightValue = 0;
uint32_t RTOSLightCount = 0;
uint32_t RTOSLightSensorHolder[20]; // hold number values of crossing the line, 0th index is the initialize
int16_t RTOSLightSensorLineCount = 0; // count number of crossing line
uint32_t RTOSLightTemp1, RTOSLightTemp2, RTOSLightTemp3;
bool RTOSLightWhiteLine = false;
char RTOSLightSensorWriteStr[50];

// timing
uint32_t RTOSLightTimeCount = 0; // based on each

void RTOSLightSensorInit() {
	// configure gpio
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

	// Timer for charging/discharging light sensor
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // enable Timer 2 periph clks
	TimerConfigure(TIMER0_BASE, TIMER_CFG_A_ONE_SHOT_UP);
	TimerLoadSet(TIMER0_BASE, TIMER_A, 0xFFFFFFFF);
	TimerEnable(TIMER0_BASE, TIMER_A);

	// Timer for crossing line
	// This timer is used for calculate how long it takes to cross a black line
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // enable Timer 2 periph clks
	TimerConfigure(TIMER1_BASE, TIMER_CFG_A_ONE_SHOT_UP);
	TimerLoadSet(TIMER1_BASE, TIMER_A, 0xFFFFFFFF);
	TimerEnable(TIMER1_BASE, TIMER_A);


	RTOSLightValue = 0;
	RTOSLightWhiteLine = true;
}

void RTOSLightSensorTick() {
	uint32_t startTime, endTime, pinValue;
	HWREG(TIMER0_BASE + TIMER_O_TAV) = 0; // set to 0 to start timing
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0xFF); //write into pin C5

	//SysCtlDelay(6000); // Wait to finish charging
	RTOSLightCount = 0;
	while (RTOSLightCount < 1000) {
		RTOSLightCount++;
	}

	startTime = TimerValueGet(TIMER0_BASE, TIMER_A); // Capture startTime

	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5); // Make PortB pin1 input to time the decaying
	pinValue = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5); // Assign the value when sensor is in fully charged state
	while (pinValue && GPIO_PIN_5) { // Loop to compare to the current state to fully-charged state
		pinValue = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);
	}

	endTime = TimerValueGet(TIMER0_BASE, TIMER_A); // Capture endTime when fully decayed

	RTOSLightValue = endTime - startTime;

	if (RTOSLightValue < RTOSLightDef) { // robot cross white line
		// LED values - 2=RED, 4=BLUE, 8=GREEN
		RTOSFlashingSetColor(8);
		if (RTOSLightWhiteLine == false) {
			// stop the timer and store it
			RTOSLightTemp2 = TimerValueGet(TIMER1_BASE, TIMER_A); // Capture startTime
			RTOSLightSensorHolder[RTOSLightSensorLineCount++] = RTOSLightTemp2 - RTOSLightTemp1;
			RTOSLightWhiteLine = true;

			// print out when crossline
			if (RTOSLightSensorLineCount == 1) {
				RTOSWriteString("\n\r1st line initialized.");
				// start timing
				RTOSLightTimeCount = 0;
				//HWREG(TIMER2_BASE + TIMER_O_TAV) = 0; // set to 0 to start timing
				//RTOSLightTimeFrom = TimerValueGet(TIMER2_BASE, TIMER_A); // Capture startTime

			} else {
				if (RTOSLightSensorHolder[RTOSLightSensorLineCount-1] > RTOSLightSensorHolder[0] * 0.2 &&
						RTOSLightSensorHolder[RTOSLightSensorLineCount-1]  < RTOSLightSensorHolder[0] * 1.5) {
					// cross the thin line, write remaining data
					RTOSModbusPrintExisting();
					RTOSWriteString("\n\rRobot crossed thin line.");

				} else if (RTOSLightSensorHolder[RTOSLightSensorLineCount-1] > RTOSLightSensorHolder[0] * 1.5) {
					//RTOSWriteString("\n\rRobot crossed thick line.");
					RTOSRunCommand("st");

					// output the timing
					//RTOSLightTimeTo = TimerValueGet(TIMER2_BASE, TIMER_A);
					// 1 stick = 25ms
					//n tick = n * 25 [ms]
					System_sprintf(RTOSLightSensorWriteStr, "\n\rRobot crossed thick line.\r\n Total run time: %d [ms].",
							RTOSLightTimeCount * 25); // choose 26 for delay of coding
					RTOSWriteString(RTOSLightSensorWriteStr);


				} else {
					System_sprintf(RTOSLightSensorWriteStr, "\n\rInvalid range of line: %d%%.",
										RTOSLightSensorHolder[RTOSLightSensorLineCount-1] * 100 / RTOSLightSensorHolder[0] );
					RTOSWriteString(RTOSLightSensorWriteStr);
				}
			}


		}
	}
	else { // robot cross blackline
		// LED values - 2=RED, 4=BLUE, 8=GREEN
		RTOSFlashingSetColor(4);
		if (RTOSLightWhiteLine == true) {
			// need to start timer
			HWREG(TIMER1_BASE + TIMER_O_TAV) = 0; // set to 0 to start timing
			RTOSLightTemp1 = TimerValueGet(TIMER1_BASE, TIMER_A); // Capture startTime
			RTOSLightWhiteLine = false;
		}
	}

	RTOSLightTimeCount++;

}

void RTOSLightSensorResetTick() {
	RTOSLightCount = 0;
}

uint32_t RTOSLightSensorCount() {
	return RTOSLightValue;
}

uint32_t RTOSLightSensorGet(int16_t index) {
	return RTOSLightSensorHolder[index];
}

int16_t RTOSLightGetLightCount() {
	return RTOSLightSensorLineCount;
}
