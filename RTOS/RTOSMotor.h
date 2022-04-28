/*
 * RTOSMotor.h
 *
 *  Created on: Oct 13, 2021
 */

#ifndef RTOS_RTOSMOTOR_H_
#define RTOS_RTOSMOTOR_H_

void RTOSMotorInit(void);
// negative for going backward
void RTOSMotorDriveLeft(int32_t freq);
void RTOSMotorDriveRight(int32_t freq);
int RTOSMotorGetMaxPWM();
void RTOSMotorStop();

#endif /* RTOS_RTOSMOTOR_H_ */
