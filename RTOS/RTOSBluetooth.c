/*
 * RTOSBluetooth.c
 *
 *  Created on: Oct 2, 2021
 */

#include "RTOSBluetooth.h"
#include "RTOSCommander.h"
#include "RTOSFlashing.h"

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Constants */
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_gpio.h>

/* XDC */
#include <xdc/runtime/System.h>
#include <xdc/runtime/Log.h>
#include <xdc/cfg/global.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTTiva.h>

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>


// Configure Bluetooth Port Here
typedef struct {
	const uint32_t periph_gpio;
	const uint32_t periph_uart;
	const uint32_t gpio_rx;
	const uint32_t gpio_tx;
	const uint32_t pin_rx;
	const uint32_t pin_tx;
	const uint32_t gpio_base;
	const uint32_t uart_base;
	const uint32_t baudRate;
} RTOSTConfig;
const RTOSTConfig rtosbtconfig = {
	// setup the port here
	.periph_gpio = SYSCTL_PERIPH_GPIOB,// SYSCTL_PERIPH_GPIOE,
	.periph_uart = SYSCTL_PERIPH_UART1, //SYSCTL_PERIPH_UART5,
	.gpio_tx = GPIO_PB1_U1TX,//GPIO_PE5_U5TX,
	.gpio_rx = GPIO_PB0_U1RX,//GPIO_PE4_U5RX,
	.pin_tx = GPIO_PIN_1,//GPIO_PIN_5,
	.pin_rx = GPIO_PIN_0,//GPIO_PIN_4,
	.gpio_base = GPIO_PORTB_BASE,// GPIO_PORTE_BASE,
	.uart_base = UART1_BASE,//UART5_BASE,
	.baudRate = 9600
};


// *** UART Configuration ***
UARTTiva_Object uartTivaObjects[1];
unsigned char uartTivaRingBuffer[1][32];
// UART configuration structures
const UARTTiva_HWAttrs uartTivaHWAttrs[1] = {
    {
        .baseAddr = UART1_BASE,// UART5_BASE,
        .intNum = INT_UART1,//INT_UART5,
        .intPriority = (~0),
        .flowControl = UART_FLOWCONTROL_NONE,
        .ringBufPtr  = uartTivaRingBuffer[0],
        .ringBufSize = sizeof(uartTivaRingBuffer[0])
    }
};
const UART_Config UART_config[] = {
    {
        .fxnTablePtr = &UARTTiva_fxnTable,
        .object = &uartTivaObjects[0],
        .hwAttrs = &uartTivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};



// Task's stack
uint8_t RTOSBT_TXTaskStack[512];
uint8_t RTOSBT_RXTaskStack[512];
// Task object (to be constructed)
Task_Struct RTOSBT_TXTaskStruct;
Task_Struct RTOSBT_RXTaskStruct;
// Configure task
Task_Params RTOSBT_TXTaskParams;
Task_Params RTOSBT_RXTaskParams;

// Constructing a Semaphore
Semaphore_Handle RTOSBT_TXSem;
Semaphore_Handle RTOSBT_RXSem;
Semaphore_Struct RTOSBT_TXSemStruct; // Memory allocated at build time
Semaphore_Struct RTOSBT_RXSemStruct; // Memory allocated at build time

// Bluetooth UART
UART_Handle RTOSBT_Uart;
// Command storage
static char RTOSBT_Command[32];
static int RTOSBT_CommandLen = 0;
char* RTOSBT_WriteString;
int RTOSBT_WriteStringMaxLen = 256;

void RTOSBTInit() {
	// ****** UART Init ******
	SysCtlPeripheralEnable(rtosbtconfig.periph_gpio);
	SysCtlPeripheralEnable(rtosbtconfig.periph_uart);
	GPIOPinConfigure(rtosbtconfig.gpio_tx);
	GPIOPinConfigure(rtosbtconfig.gpio_rx);
	GPIOPinTypeUART(rtosbtconfig.gpio_base, rtosbtconfig.pin_tx | rtosbtconfig.pin_rx);
	UART_init();
	// Wait for the UART module to be ready
	//while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART5)) { }

// Configure task
//	Task_Params RTOSBT_TXTaskParams;
//	Task_Params RTOSBT_RXTaskParams;
	Task_Params_init(&RTOSBT_TXTaskParams);
	RTOSBT_TXTaskParams.stack = RTOSBT_TXTaskStack;
	RTOSBT_TXTaskParams.stackSize = 512;
	RTOSBT_TXTaskParams.priority = 11;
	Task_Params_init(&RTOSBT_RXTaskParams);
	RTOSBT_RXTaskParams.stack = RTOSBT_RXTaskStack;
	RTOSBT_RXTaskParams.stackSize = 512;
	RTOSBT_RXTaskParams.priority = 10;

	Task_construct(&RTOSBT_TXTaskStruct, RTOSBTWriteTaskFunc, &RTOSBT_TXTaskParams, NULL);
	Task_construct(&RTOSBT_RXTaskStruct, RTOSBTReadTaskFunc, &RTOSBT_RXTaskParams, NULL);

	// Configure semaphore
	Semaphore_Params RTOSBT_TXSemParams;
	Semaphore_Params RTOSBT_RXSemParams;

	Semaphore_Params_init(&RTOSBT_TXSemParams);
	Semaphore_Params_init(&RTOSBT_RXSemParams);
	Semaphore_construct(&RTOSBT_TXSemStruct, 0, &RTOSBT_TXSemParams);
	Semaphore_construct(&RTOSBT_RXSemStruct, 0, &RTOSBT_RXSemParams);

	//It's optional to store the handle
	RTOSBT_TXSem = Semaphore_handle(&RTOSBT_TXSemStruct);
	RTOSBT_RXSem = Semaphore_handle(&RTOSBT_RXSemStruct);

	//Configure the UART Module
	UART_Params uartParams;
	// Create a UART with data processing off.
	UART_Params_init(&uartParams);
	uartParams.writeDataMode = UART_DATA_BINARY;
	uartParams.readDataMode = UART_DATA_BINARY;
	uartParams.readReturnMode = UART_RETURN_FULL;
	uartParams.readEcho = UART_ECHO_ON;
	uartParams.baudRate = rtosbtconfig.baudRate;
	//uartParams.baudRate = 115200;
	RTOSBT_Uart = UART_open(0, &uartParams);

	if (RTOSBT_Uart == NULL) { // If UART fails
		// Use RTOSFlashing to update status
		RTOSFlashingSet(2);
		System_abort("\nRTOSBluetooth UART Failed to open.");
		return;
	}
}

void RTOSBTWriteTaskFunc(UArg arg0, UArg arg1) {
	char *c;
	int count = 0;

	while(1) {
		Semaphore_pend(RTOSBT_TXSem, BIOS_WAIT_FOREVER);

		c = RTOSBT_WriteString;
		count = 0;
		while (*c != '\0' | count > RTOSBT_WriteStringMaxLen) {
			UARTCharPut(rtosbtconfig.uart_base, *c);
			c++;
			count++; // insurance for infinite loop
		}

	}
}


void RTOSWriteString(char* cs) {

	RTOSBT_WriteString = cs;
	Semaphore_post(RTOSBT_TXSem);

}

void RTOSBTReadTaskFunc(UArg arg0, UArg arg1) {

	unsigned char rxBuffer[1];
	int readSize;
	int temp;

	while(1) {
		Semaphore_pend(RTOSBT_RXSem, BIOS_WAIT_FOREVER);

		readSize = UART_read(RTOSBT_Uart, rxBuffer, sizeof(rxBuffer));
		UART_write(RTOSBT_Uart, rxBuffer, sizeof(rxBuffer));
		if (readSize == 0) continue; // skip when no data

		// if c is not \n, append the command
		// otherwise, execute command
		if (rxBuffer[0] != '\n' && rxBuffer[0] != '\r') {
			// append the command
			RTOSBT_Command[RTOSBT_CommandLen++] = rxBuffer[0];

		} else {

			RTOSBT_Command[RTOSBT_CommandLen] = '\0'; // end of string
			// execute command
			temp = RTOSRunCommand(RTOSBT_Command);
			if (temp == 1) {
				RTOSWriteString("\n\rCommand Executed.\n\rEnter command: ");
			} else {
				// invalid command
				RTOSWriteString("\n\rInvalid command.\n\rEnter command: ");
			}


			RTOSBT_CommandLen = 0; // reset string length
			RTOSBT_Command[0] = '\0';
		}

	}
}


void RTOSBluetooth_ClockHandler(UArg arg) {

	Semaphore_post(RTOSBT_RXSem);
}


