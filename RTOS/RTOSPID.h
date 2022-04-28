/*
 * RTOSPID.h
 *
 *  Created on: Oct 11, 2021
 *  This file will read distance sensors and use PID to control the robot
 *  Hwi will be used on this one
 */


#ifndef RTOS_RTOSPID_H_
#define RTOS_RTOSPID_H_

#include <xdc/std.h>

void RTOSPIDInit(void);
// Get raw data from Distance sensor on the front and on the right
// We can update the calibration distance by using SetSetting and enum
//void RTOSPIDGetDistance(uint32_t* front, uint32_t* right);
void RTOSPIDGetSettings(char* s);
void RTOSPIDSetSetting(int32_t type, int32_t value);
void RTOSPIDCompute();
void RTOSPID_ClockHandler(UArg arg);
void RTOSPIDRun();
// Use this one to add +1 when plus = true and -1 when plus = false
void RTOSPIDTuneP(int8_t v);
void RTOSPIDTuneD(int8_t v);
void RTOSPIDTuneI(int8_t v);
void RTOSPIDGet(int* p, int* i, int* d);
void RTOSPIDPIDEnable(int8_t flag); // enable pid by set flag = 1
//enum PIDLogEnum {iKpLog, iKiLog, iKdLog, iKadjust, iPWMLeftLog, iPWMRightLog};
//void RTOSPIDLog(int32_t kp, int32_t ki, int32_t kd,int32_t kadjust,int32_t pwmleft,int32_t pwmright); // log infomation of pid changing
void RTOSPIDRunAlongWall(); // this state for the robot run a long a wall with a distance
void RTOSPIDRightTurn(); // the robot will make a right turn
bool RTOSPIDTurnAround(); // the robot keep turning left until find a path (u-turn or turn left)
						  // return true when the new path is found

#endif /* RTOS_RTOSPID_H_ */
