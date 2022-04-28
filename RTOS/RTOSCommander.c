/*
 * RTOSCommander.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <RTOS/RTOSFlashing.h>
#include <RTOS/RTOSBluetooth.h>
#include <math.h>
#include <xdc/runtime/System.h>
#include <RTOS/RTOSMotor.h>
#include <RTOS/RTOSPID.h>
#include <RTOS/RTOSCommander.h>
#include <RTOS/RTOSDistance.h>
#include "driverlib/sysctl.h"
#include <RTOS/RTOSLightSensor.h>

char RTOSCommander_Raw[32]; // store command before proceed
char *RTOSCommander_token;
// temperory output command
char RTOSCommanderOutString[256];
unsigned int commandCount = 0;
int RTOSCommanderTemp[3];
uint32_t RTOSCommanderTempU[3];
char RTOSChar1[10];
char RTOSChar2[10];

void RTOSCommanderInit() {
	// For advance only
	// Use queue interrupt to push command
}

// return 1 when executed
// return 0 when invalid command
// for advance: this one should put in queue
int RTOSRunCommand(char *cmd) {
	int r = 1;
	// copy raw command
	strcpy(RTOSCommander_Raw, cmd);

	// split command to args
	RTOSCommander_token = strtok(RTOSCommander_Raw, " ");

	// execute command
	if (strcmp(RTOSCommander_token, "d") == 0) {
		// get 1st argument
		RTOSCommander_token = strtok(NULL, " ");
		if (RTOSCommander_token == NULL) {
			RTOSWriteString("\n\rAn arg is required for [debug] command.");
			return 0;
		}
		if (strcmp(RTOSCommander_token, "normal") == 0) {
			RTOSFlashingSet(0);
		} else if (strcmp(RTOSCommander_token, "error") == 0) {
			RTOSFlashingSet(1);
		} else if (strcmp(RTOSCommander_token, "bt") == 0) {
			RTOSFlashingSet(2);
		} else if (strcmp(RTOSCommander_token, "mr") == 0) {
			// send command: debug motor_right
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSMotorDriveRight(atoi(RTOSCommander_token));
		} else if (strcmp(RTOSCommander_token, "ml") == 0) {
			// send command: debug motor_left
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSMotorDriveLeft(atoi(RTOSCommander_token));
		}else if (strcmp(RTOSCommander_token, "d") == 0) {

			RTOSDistanceGet(&RTOSCommanderTempU[0], &RTOSCommanderTempU[1]);
//			Float2Str(&RTOSChar1[0], RTOSCommanderTemp[0]);
//			Float2Str(&RTOSChar2[0], RTOSCommanderTemp[1]);
			System_sprintf(RTOSCommanderOutString, "\n\rDistance to front: %d mm \n\rDistance to right: %d mm\n\r",
					RTOSCommanderTempU[0]/10, RTOSCommanderTempU[1]/10);
			RTOSWriteString(RTOSCommanderOutString);
		}
		else if (strcmp(RTOSCommander_token, "clk") == 0) {

			System_sprintf(RTOSCommanderOutString, "\n\rSystem Clock: %d \n\r", SysCtlClockGet());
			RTOSWriteString(RTOSCommanderOutString);
		}
		else if (strcmp(RTOSCommander_token, "t") == 0) {
			System_sprintf(RTOSCommanderOutString, "\n\rLight Timer Count: %d \n\r", RTOSLightSensorCount());
			RTOSWriteString(RTOSCommanderOutString);
		}
		else if (strcmp(RTOSCommander_token, "timer") == 0) {
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSCommanderTempU[0] = atoi(RTOSCommander_token);
			System_sprintf(RTOSCommanderOutString, "\n\rLight Timer for %d line is: %d \n\r", RTOSCommanderTempU[0],
					RTOSLightSensorGet(RTOSCommanderTempU[0]));

			RTOSWriteString(RTOSCommanderOutString);
		}
		else {
			r = 0;
			System_sprintf(RTOSCommanderOutString, "\n\rArg=%s is not valid in [debug] command.", RTOSCommander_token);
			RTOSWriteString(RTOSCommanderOutString);
		}
	}
	// setting
	else if(strcmp(RTOSCommander_token, "pid") == 0) {
		// get 1st argument
		RTOSCommander_token = strtok(NULL, " ");

		if (RTOSCommander_token == NULL) {
			RTOSWriteString("\n\rAn arg is required for [pid] command.");
			return 0;
		}
		if (strcmp(RTOSCommander_token, "get") == 0) {
			RTOSPIDGetSettings(RTOSCommanderOutString);
			RTOSWriteString(RTOSCommanderOutString);
		} else if (strcmp(RTOSCommander_token, "set") == 0) {
			// get 2nd, 3rd, 4th values as PID
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(0, atoi(RTOSCommander_token));
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(1, atoi(RTOSCommander_token));
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(2, atoi(RTOSCommander_token));
		} else if (strcmp(RTOSCommander_token, "setright") == 0) {
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(3, atoi(RTOSCommander_token));
		} else if (strcmp(RTOSCommander_token, "setfront") == 0) {
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(4, atoi(RTOSCommander_token));
		} else if (strcmp(RTOSCommander_token, "setpwmright") == 0) {
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(5, atoi(RTOSCommander_token));
		} else if (strcmp(RTOSCommander_token, "setpwmleft") == 0) {
			// get 2nd argument
			RTOSCommander_token = strtok(NULL, " ");
			RTOSPIDSetSetting(6, atoi(RTOSCommander_token));
		}
		else if (strcmp(RTOSCommander_token, "start") == 0) {
			RTOSPIDPIDEnable(1);
		}
		else if (strcmp(RTOSCommander_token, "stop") == 0) {
			RTOSPIDPIDEnable(0);
		}
		else {
			r = 0;
			System_sprintf(RTOSCommanderOutString, "\n\rArg=%s is not valid in [pid] command.", RTOSCommander_token);
			RTOSWriteString(RTOSCommanderOutString);
		}
	}
	else if(strcmp(RTOSCommander_token, "t") == 0) { // tune values in PID

		// get 1st argument
		RTOSCommander_token = strtok(NULL, " ");
		if (RTOSCommander_token == NULL) {
			RTOSWriteString("\n\rAn arg is required for [tune pid] command.");
			return 0;
		}
		if (strcmp(RTOSCommander_token, "p+") == 0) {
			RTOSPIDTuneP(1);
		}
		else if (strcmp(RTOSCommander_token, "p-") == 0) {
			RTOSPIDTuneP(-1);
		}
		else if (strcmp(RTOSCommander_token, "i+") == 0) {
			RTOSPIDTuneI(1);
		}
		else if (strcmp(RTOSCommander_token, "i-") == 0) {
			RTOSPIDTuneI(-1);
		}
		else if (strcmp(RTOSCommander_token, "d+") == 0) {
			RTOSPIDTuneD(1);
		}
		else if (strcmp(RTOSCommander_token, "d-") == 0) {
			RTOSPIDTuneD(-1);
		}

		// print PID values
		RTOSPIDGet(&RTOSCommanderTemp[0], &RTOSCommanderTemp[1], &RTOSCommanderTemp[2]);

		System_sprintf(RTOSCommanderOutString, "\n\rPID Values: %d %d %d\n\r",
				RTOSCommanderTemp[0], RTOSCommanderTemp[1], RTOSCommanderTemp[2]);
		RTOSWriteString(RTOSCommanderOutString);

	}
	else if(strcmp(RTOSCommander_token, "ss") == 0) {
		RTOSPIDPIDEnable(1);
	}
	else if(strcmp(RTOSCommander_token, "st") == 0) {
		RTOSPIDPIDEnable(0);
	}
	else {
		// invalid command
		r = 0;
	}

	commandCount++;
	return r;
}


int temp1;
int temp2;
void Float2Str(char *c, float f) {
	temp1 = f;
	f = f - temp1;
	temp2 = f * 10000;
	System_sprintf(c, "%d.%04d", temp1, temp2);
}
