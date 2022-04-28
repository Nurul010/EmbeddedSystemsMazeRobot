/*
 * RTOSMasterInclude.h
 *
 *  Created on: Oct 2, 2021
 *  NOT RECEOMMEND TO USE THIS FILE, WILL CAUSE SLOW BUILD
 */

#ifndef RTOS_RTOSMASTERINCLUDE_H_
#define RTOS_RTOSMASTERINCLUDE_H_

/* Standard libs */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

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

#endif /* RTOS_RTOSMASTERINCLUDE_H_ */
