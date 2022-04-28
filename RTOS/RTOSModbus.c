/*
 * RTOSModbus.c
 *
 *  Created on: Nov 15, 2021
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <RTOS/RTOSBluetooth.h>

char mudbusformat[41]; // store 40 char to print
char modbustouart[60];
uint32_t modbusping[20];
uint32_t modbuspong[20];
uint16_t modbusID; // use team member for this ID
uint16_t modbusIndex; // store index of ping/pong arr
uint16_t modbusArrNum; // total number in array
bool isPing; // isPing = true will send modbusping, otherwise

void RTOSModbusInit() {
	// initialize dynamic array
	modbusArrNum = 20; // store 20 of 2 hex value
//	modbusping =  (uint32_t*) malloc(modbusArrNum * sizeof(uint32_t));
//	modbuspong =  (uint32_t*) malloc(modbusArrNum * sizeof(uint32_t));
//	mudbusformat =  (char*) malloc(2 * modbusArrNum * sizeof(char));
	mudbusformat[40] = '\0'; // end string
	modbusID = 22; // change this one based on our team member
	modbusIndex = 0;
	isPing = true;


	int i = 0;
	for(i = 0; i < 41; i++) {
		mudbusformat[i] = '\0';
	}
}

void RTOSModbusSetID(uint16_t tm) {
	modbusID = tm;
}


// Push data to ping or pong depend on isPing
// return true if the index is reach to max array
bool RTOSModbusPushData(uint32_t data) {
	bool flag = false;

	if (isPing) {
		modbusping[modbusIndex++] = data;
	} else {
		modbuspong[modbusIndex++] = data;
	}

	if(modbusIndex >= modbusArrNum) {
		flag = true;
		modbusIndex = 0;
		isPing = !isPing;
	}
	return flag;
}


void RTOSModbusPrint() {
	// get the string of mudbus format
	//  start of frame ":", Team number (slave address), 20 bytes of data in ASCII, Team Number (checksum), CR, LF

	// store 20 bytes of data into mudbusformat
	uint16_t i, j, k;
	uint32_t* arr = isPing ? modbuspong : modbusping;

	for (i = 0; i < modbusArrNum; i++) {
		// [jk] is number of hex
		j = (arr[i] / 16) % 15;
		k = arr[i] % 15;
		// store it into mudbusformat
		j = j < 10 ? j + 48 : j + 65 - 10;
		k = k < 10 ? k + 48 : k + 65 - 10;
		mudbusformat[2*i] = j;
		mudbusformat[2*i+1] = k;
	}

	// print it out to input array
	sprintf(modbustouart, "\r\n:%d%s%dCRLF", modbusID, mudbusformat, modbusID);
	RTOSWriteString(modbustouart);

}

void RTOSModbusPrintExisting() {
	uint16_t i, j, k;
	// swap ping vs ping in RTOSModbusPrint()
	uint32_t* arr = isPing ? modbusping : modbuspong;

	for (i = 0; i < modbusArrNum; i++) {
		if (i < modbusIndex) {
			// [jk] is number of hex
			j = (arr[i] / 16) % 15;
			k = arr[i] % 15;
		} else {
			// write 0 since we do not want to write old value
			j = k = 0;
		}
		// store it into mudbusformat
		j = j < 10 ? j + 48 : j + 65 - 10;
		k = k < 10 ? k + 48 : k + 65 - 10;

		mudbusformat[2*i] = j;
		mudbusformat[2*i+1] = k;
	}

	// print it out to input array
	sprintf(modbustouart, "\r\n:%d%s%dCRLF", modbusID, mudbusformat, modbusID);
	RTOSWriteString(modbustouart);
}

