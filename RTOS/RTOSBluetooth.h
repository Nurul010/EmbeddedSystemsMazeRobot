/*
 * RTOSBluetooth.h
 *
 *  Created on: Oct 2, 2021
 *  Summary: This RTOSBluetooth is used to initialize and run bluetooth UART by using Swi, Sophomore, and Task
 *  Interrupt Clock need to create in .cfg as below:
 *  	Clock Handle: RTOSBluetoothHandler
 *  	Clock Function: RTOSBluetooth_ClockHandler
 *  	Initial Timeout: 500 (as 0.5 seconds)
 *  	Clock period: 100 (as 100 ms) recommend minimum value at 10 for Bluetooth Low Energy
 */

#ifndef RTOS_RTOSBLUETOOTH_H_
#define RTOS_RTOSBLUETOOTH_H_

#include <xdc/std.h>

void RTOSBTInit();
void RTOSBTWriteLine(char *c);
void RTOSBTReadLine(char *arr);
void RTOSBTWriteTaskFunc(UArg arg0, UArg arg1);
void RTOSBTReadTaskFunc(UArg arg0, UArg arg1);
void RTOSWriteString(char* cs);

#endif /* RTOS_RTOSBLUETOOTH_H_ */
