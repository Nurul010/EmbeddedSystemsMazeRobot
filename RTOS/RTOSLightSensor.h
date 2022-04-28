/*
 * RTOSLightSensor.h
 *
 *  Created on: Nov 8, 2021
 */

#ifndef RTOS_RTOSLIGHTSENSOR_H_
#define RTOS_RTOSLIGHTSENSOR_H_

void RTOSLightSensorInit();
void RTOSLightSensorTick(); // keep increase 1ms when the function is called
void RTOSLightSensorResetTick();
uint32_t RTOSLightSensorCount();
uint32_t RTOSLightSensorGet(int16_t index);
int16_t RTOSLightGetLightCount();

#endif /* RTOS_RTOSLIGHTSENSOR_H_ */
