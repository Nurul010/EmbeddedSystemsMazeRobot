/*
 * RTOSMotor.c
 *
 *  Created on: Oct 13, 2021
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"

int pwmMax = 0;
int pwmAdjust = 0;

void RTOSMotorInit(void) {

	// using PWM Generator 1
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	// using port A and B
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// PWM output PA6 and PA7
	GPIOPinTypePWM(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7); //PWM output
	//PB2 and PB3 for xPhase, PB6 for mode
	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6);
	GPIOPinConfigure(GPIO_PA7_M1PWM3);
	GPIOPinConfigure(GPIO_PA6_M1PWM2);

	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF); // Mode 0x00 to reverse
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF); // Phase
	//setxPhase(1);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	// get PWM clock rate
	//uint32_t PWMclockRate = SysCtlClockGet() / 64 ;
	// PWM clock rate / PWM base frequency = pwm max
	//pwmMax = (PWMclockRate / 100) - 1;
	pwmMax = 11362;
	pwmAdjust = 350;
	// set period for PWM
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, pwmMax);
	//PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, 1000);

	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 0); //right
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 0); //left
	PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
	PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_1);

}


void RTOSMotorDriveRight(int32_t freq) {
	if (freq == 0) {
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, 1);
		PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, false);
		return;
	}

	if (freq < 0) {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0xFF);// reverse
		// in reverse mode, the lower value will be run higher
		freq = pwmMax + freq;
	}
	else {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0x00);// forward
	}

	//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, abs(freq) * pwmMax / 1000); // right
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, abs(freq)); // right
	PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);

}

void RTOSMotorDriveLeft(int32_t freq) {
	if (freq == 0) {
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, 1);
		PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, false);
		return;
	}

	if (freq < 0) {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0xFF);// reverse
		// in reverse mode, the lower value will be run higher
		freq = pwmMax + freq;
	}
	else {
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);// forward
	}
	// adjust motor left +300 due to unbalance
	freq = abs(freq) + 300;
	//PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, abs(freq) * pwmMax / 1000); //left
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, abs(freq)); // right
	PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
}

void RTOSMotorStop() { // stop the motor
	RTOSMotorDriveRight(500);
	RTOSMotorDriveLeft(500);
	RTOSMotorDriveRight(1);
	RTOSMotorDriveLeft(1);

}

int RTOSMotorGetMaxPWM() {
	return pwmMax;
}
