/*
 * RTOSModbus.h
 *
 *  Created on: Nov 15, 2021
 */

#ifndef RTOS_RTOSMODBUS_H_
#define RTOS_RTOSMODBUS_H_

// Use ping pong as requirement
void RTOSModbusInit();
void RTOSModbusSetID(uint16_t tm);
void RTOSModbusPrint(); // print completed value
void RTOSModbusPrintExisting(); // print existing value, use to print remaining values when robot is stopped
bool RTOSModbusPushData(uint32_t data);

#endif /* RTOS_RTOSMODBUS_H_ */
