/*
 * RTOSCAN.h
 *
 *  Created on: Oct 10, 2021
 *  Please run RTOSCANInit() before using the library
 */

#ifndef RTOS_RTOSCAN_H_
#define RTOS_RTOSCAN_H_


#include <xdc/std.h>

void RTOSCANInit();
void RTOSCANIntHandler();
void RTOSCANTXTaskFunc(UArg arg0, UArg arg1);
void RTOSCANSend(uint32_t msgID, uint8_t *data);


#endif /* RTOS_RTOSCAN_H_ */
