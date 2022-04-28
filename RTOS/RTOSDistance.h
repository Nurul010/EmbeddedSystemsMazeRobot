/*
 * RTOSDistance.h
 *
 *  Created on: Oct 26, 2021
 */

#ifndef RTOS_RTOSDISTANCE_H_
#define RTOS_RTOSDISTANCE_H_

void RTOSDistanceInit(void);
// Get raw data from Distance sensor on the front and on the right
// We can update the calibration distance
void RTOSDistanceGet(uint32_t* front, uint32_t* right); // 10x[mm]
uint32_t VoltageToCmRight(uint32_t voltage); // 10x[mm]
uint32_t VoltageToCmFront(uint32_t voltage); // 10x[mm]


#endif /* RTOS_RTOSDISTANCE_H_ */
