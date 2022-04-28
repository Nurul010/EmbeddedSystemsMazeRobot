/*
 * RTOSFlashing.h
 *
 *  Created on: Oct 2, 2021
 *  THIS FILE USED TO SEE THE STATUS OF DEVICE BASED ON LED FLASHING ON BOARD
 *  Interrupt Clock need to create in .cfg as below:
 *  	Clock Handle: RTOSFlashingHandle
 *  	Clock Function: RTOSFlashing_ClockHandler
 *  	Initial Timeout: 5000 (as 5 seconds)
 *  	Clock period: 5000 (as 5 seconds)
 *  Code Status:
 *  case 1: // general error {2, 0, 2, 0, 2, 0};
    case 2: // No bluetooth connection {2, 0, 4, 0, 8, 0};
	default: // normal {8, 0, 8, 0, 8, 0};
 */

#ifndef RTOS_RTOSFLASHING_H_
#define RTOS_RTOSFLASHING_H_

#include <xdc/std.h>

void RTOSFlashingInit();
void RTOSFlashingRun();
void RTOSFlashingSet(int flashingCode);
int RTOSFlashingGet();
void RTOSFlashingTaskFunc(UArg arg0, UArg arg1);
void RTOSFlashing_ClockHandler(UArg arg);
// LED values - 2=RED, 4=BLUE, 8=GREEN
void RTOSFlashingSetColor(uint8_t color);


#endif /* RTOS_RTOSFLASHING_H_ */
