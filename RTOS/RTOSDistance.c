/*
 * RTOSDistance.c
 *
 *  Created on: Oct 26, 2021
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

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
#include <RTOS/RTOSDistance.h>

uint32_t RTOSDistanceFrontVol, RTOSDistanceRightVol;
//float RTOSDistanceFrontCm, RTOSDistanceRightCm;
uint32_t RTOSDistanceFrontTemp, RTOSDistanceRightTemp;
uint16_t RTOSILoop;


void RTOSDistanceInit(void) {

//	RTOSDistanceFrontVol = RTOSDistanceRightVol = 0;
//	RTOSDistanceFrontCm = RTOSDistanceRightCm = 0.0;
	// Setup ADC PE3 for front distance sensor - PE2 for right distance sensor
	// Enable ADC0 module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	//SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	// configure PE3 for input
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	// Configure sample sequencer
	ADCSequenceDisable(ADC0_BASE, 0);
	ADCSequenceDisable(ADC0_BASE, 1);
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);

	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCSequenceEnable(ADC0_BASE, 1);

}


void RTOSDistanceGet(uint32_t* front, uint32_t* right) {
	RTOSDistanceRightTemp = 0;
	RTOSDistanceFrontTemp = 0;

	for (RTOSILoop = 0; RTOSILoop < 5; RTOSILoop++) {
		// clear ADC interrupt
		ADCIntClear(ADC0_BASE, 0);
		ADCIntClear(ADC0_BASE, 1);
		// trigger ADC sampling
		ADCProcessorTrigger(ADC0_BASE, 0);
		ADCProcessorTrigger(ADC0_BASE, 1);
		// read voltage
		ADCSequenceDataGet(ADC0_BASE, 0, &RTOSDistanceRightVol);
		ADCSequenceDataGet(ADC0_BASE, 1, &RTOSDistanceFrontVol);
		RTOSDistanceRightTemp += VoltageToCmRight(RTOSDistanceRightVol);
		RTOSDistanceFrontTemp += VoltageToCmFront(RTOSDistanceFrontVol);
		//RTOSDistanceRightTemp += RTOSDistanceRightVol;
		//RTOSDistanceFrontTemp += RTOSDistanceFrontVol;
	}

	RTOSDistanceRightTemp = RTOSDistanceRightTemp / 5;
	RTOSDistanceFrontTemp = RTOSDistanceFrontTemp / 5;

	if (RTOSDistanceRightTemp < 400) *right = 400; // minimum of 4cm
	else if (RTOSDistanceRightTemp > 4000) *right = 4000; // maximum of 40cm
	else *right = RTOSDistanceRightTemp;
	//*right = RTOSDistanceRightTemp;
	if (RTOSDistanceFrontTemp < 400) *front = 400; // minimum of 4cm
	else if (RTOSDistanceFrontTemp > 3000) *front = 3000; // maximum of 40cm
	else *front = RTOSDistanceFrontTemp;
	//*front = RTOSDistanceFrontTemp;

	//printf("%d",RTOSDistanceFrontTemp);
}
// return [mm]
uint32_t VoltageToCmRight(uint32_t voltage) {
	// Calibration to get the voltage equation to 10x[mm]
	// power factor 0.984 < 1 should cause error
	// replace with 1
	return (13764.0 / pow((int)voltage, 0.984)) * 100;
	//return (31165.0 / pow((int)voltage, 1.101)) * 100;
}
uint32_t VoltageToCmFront(uint32_t voltage) {
	// Calibration to get the voltage equation to 10x[mm]
	return (31165.0 / pow((int)voltage, 1.101)) * 100;
}
