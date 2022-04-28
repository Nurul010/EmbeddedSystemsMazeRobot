/*
 * RTOSCommander.h
 *
 *  Created on: Oct 3, 2021
 */

#ifndef RTOS_RTOSCOMMANDER_H_
#define RTOS_RTOSCOMMANDER_H_

#include <xdc/std.h>

// init commander
void RTOSCommanderInit();
// return 1 when executed
// return 0 when invalid command
int RTOSRunCommand(char *cmd);
void Float2Str(char *c, float f);

#endif /* RTOS_RTOSCOMMANDER_H_ */
