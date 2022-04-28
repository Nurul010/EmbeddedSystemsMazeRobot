/*
 * RTOSFlashing.c
 *
 *  Created on: Oct 2, 2021
 *  THIS FILE USED TO SEE THE STATUS OF DEVICE BASED ON LED FLASHING ON BOARD
 *  Interrupt Clock need to create in .cfg as below:
 *  	Clock Handle: RTOSFlashingHandle
 *  	Clock Function: RTOSFlashing_ClockHandler
 *  	Initial Timeout: 5000 (as 5 seconds)
 *  	Clock period: 5000 (as 5 seconds)
 *
 */
#include "RTOSFlashing.h"
#include <xdc/std.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/uart/UARTTiva.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

int currentFlashingCode = 0;
bool isLEDFlashing = true;
// LED values - 2=RED, 4=BLUE, 8=GREEN
//uint8_t ledCode[6] = {8, 0, 8, 0, 8, 0};
typedef struct {
	uint8_t ledCode[6];
} RTOSFlashingLEDCode;

RTOSFlashingLEDCode flashingCode = {8, 0, 8, 0, 8, 0};
uint8_t blinkingColor = 0;

/* Task's stack */
uint8_t RTOSFlashingTaskStack[512];
/* Task object (to be constructed) */
Task_Struct RTOSFlashingtask0;
// Constructing a Semaphore
Semaphore_Handle RTOSFlashingSem;
Semaphore_Struct RTOSFlashingStructSem; /* Memory allocated at build time */


void RTOSFlashingInit() {
	// Config LED on board
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	Task_Params RTOSFlashingTaskParams;
	Semaphore_Params RTOSFlashingSemParams;

	// Configure task
	Task_Params_init(&RTOSFlashingTaskParams);
	RTOSFlashingTaskParams.stack = RTOSFlashingTaskStack;
	RTOSFlashingTaskParams.stackSize = 512;
	RTOSFlashingTaskParams.priority = 1;

	Task_construct(&RTOSFlashingtask0, RTOSFlashingTaskFunc, &RTOSFlashingTaskParams, NULL);

	// Configure semaphore
	Semaphore_Params_init(&RTOSFlashingSemParams);
	Semaphore_construct(&RTOSFlashingStructSem, 0, &RTOSFlashingSemParams);
	//It's optional to store the handle
	RTOSFlashingSem = Semaphore_handle(&RTOSFlashingStructSem);
}


/* Task function */
void RTOSFlashingTaskFunc(UArg arg0, UArg arg1)
{

    /* Local variables. Variables here go onto task stack!! */
	Uint32 ms = 50;
	int i = 0;
    /* Run one-time code when task starts */

    while (1) /* Run loop forever (unless terminated) */
    {
    	Semaphore_pend(RTOSFlashingSem, BIOS_WAIT_FOREVER);
    	if (isLEDFlashing == false) {
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, blinkingColor);
    		Task_sleep(50);
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
    		blinkingColor = 0;
    		continue;
    	}

    	switch(currentFlashingCode) {
    		// LED values - 2=RED, 4=BLUE, 8=GREEN
    		case 1: // general error
    			flashingCode = (RTOSFlashingLEDCode) {2, 0, 2, 0, 2, 0};
    			break;
    		case 2: // No bluetooth connection
    			flashingCode = (RTOSFlashingLEDCode) {2, 0, 4, 0, 8, 0};
				break;
    		case 3: // green
    			flashingCode = (RTOSFlashingLEDCode) {8, 8, 8, 8, 8, 8};
    			break;
    		case 4: //blue
    			flashingCode = (RTOSFlashingLEDCode) {4, 4, 4, 4, 4, 4};
    			break;
			default: // set to 0 - normal status
				flashingCode = (RTOSFlashingLEDCode) {8, 0, 8, 0, 8, 0};

    	}

    	for (i = 0; i < 6; i++) {
    		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, flashingCode.ledCode[i]);
    		Task_sleep(ms);
    	}
    }
}


void RTOSFlashingSet(int flashingCode) {
	isLEDFlashing = true;
	currentFlashingCode = flashingCode;
}

int RTOSFlashingGet() {
	return currentFlashingCode;
}

void RTOSFlashing_ClockHandler(UArg arg) {
	Semaphore_post(RTOSFlashingSem);
}

void RTOSFlashingSetColor(uint8_t color) {
	isLEDFlashing = false;
	blinkingColor = color;
}


