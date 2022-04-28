/*
 * RTOSCAN.c
 *
 *  Created on: Oct 10, 2021
 */
#include <RTOS/RTOSCAN.h>
#include <RTOS/RTOSBluetooth.h>

#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

#include "utils/uartstdio.h"

#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/uart/UARTTiva.h>

/* Task's stack */
uint8_t RTOSCANTXTaskStack[512];
/* Task object (to be constructed) */
Task_Struct RTOSCANTXTask;
// Constructing a Semaphore
Semaphore_Handle RTOSCANTXSem;
Semaphore_Struct RTOSCANTXStructSem; /* Memory allocated at build time */

uint8_t RTOSCANBufferOut[8];
uint32_t RTOSCANMsgID;


uint8_t RTOSCANEnableStatus = 0;
volatile bool RTOSCANerrFlag = 0; // transmission error flag

void RTOSCANSend(uint32_t msgID, uint8_t *data)
{
	int i;
	// copy message
	for (i = 0; i < 8; i++) {
		RTOSCANBufferOut[i] = data[i];
	}
	RTOSCANMsgID = msgID;

	Semaphore_post(RTOSCANTXSem);
}


// Enable CAN0
void RTOSCANInit(void) {

	// GPIO Port B needs to be enable
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	// Select which alternate function is available for PE4, and PE5
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	GPIOPinConfigure(GPIO_PE5_CAN0TX);
	// Enable the alternate function instead of GPIO for these pins
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);


	// Enable the CAN0 module
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	// Wait for the CAN0 module to be ready
	while (!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0)) { }
	// Reset the state of all the message objects and the state
	// of CAN module to a known state
	CANInit(CAN0_BASE);
	// The CAN bus is set to 0.5Mbit operation
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);

	// Use dynamic allocation of the vector table
	//CANIntRegister(CAN0_BASE, RTOSCANIntHandler);
	// Enable interrupts on the CAN peripheral
	//CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
	// Take the CAN0 Device out of INIT State
	CANEnable(CAN0_BASE);

	// Configure Task and Semaphore Interrupt
	Task_Params RTOSCANTXTaskParams;
	Semaphore_Params RTOSCANTXSemParams;

	// Configure task
	Task_Params_init(&RTOSCANTXTaskParams);
	RTOSCANTXTaskParams.stack = RTOSCANTXTaskStack;
	RTOSCANTXTaskParams.stackSize = 512;
	RTOSCANTXTaskParams.priority = 1;

	Task_construct(&RTOSCANTXTask, RTOSCANTXTaskFunc, &RTOSCANTXTaskParams, NULL);

	// Configure semaphore
	Semaphore_Params_init(&RTOSCANTXSemParams);
	Semaphore_construct(&RTOSCANTXStructSem, 0, &RTOSCANTXSemParams);
	//It's optional to store the handle
	RTOSCANTXSem = Semaphore_handle(&RTOSCANTXStructSem);

	// Flag as enable
	RTOSCANEnableStatus = 1;
}

void RTOSCANIntHandler(void) {
	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status

	if(status == CAN_INT_INTID_STATUS) { // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		RTOSCANerrFlag = 1;
	} else if(status == 1) { // message object 1
		CANIntClear(CAN0_BASE, 1); // clear interrupt
		RTOSCANerrFlag = 0; // clear any error flags
	}
}

void RTOSCANTXTaskFunc(UArg arg0, UArg arg1)
{
	/* Local variables. Variables here go onto task stack!! */
	tCANMsgObject sMsgObjTx;

	/* Run one-time code when task starts */

	while (1) /* Run loop forever (unless terminated) */
	{
	   Semaphore_pend(RTOSCANTXSem, BIOS_WAIT_FOREVER);

	   // configure and start transmit of message object
	   sMsgObjTx.ui32MsgID = RTOSCANMsgID;
	   sMsgObjTx.ui32MsgIDMask = 0;
	   sMsgObjTx.ui32Flags = 0;
	   sMsgObjTx.ui32MsgLen = 8;
	   sMsgObjTx.pui8MsgData = RTOSCANBufferOut;

	   CANMessageSet(CAN0_BASE, 1, &sMsgObjTx, MSG_OBJ_TYPE_TX);


	   RTOSWriteString(" (A CAN Command sent.) ");
	}
}
