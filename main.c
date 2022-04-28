/*
Written By:
Dat Tran, Nurul Saiym, Damon Spencer, and Christopher Montealvo
*/
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

/* DriverLib */
#include <driverlib/sysctl.h>
#include <driverlib/pin_map.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/pwm.h>
#include <driverlib/timer.h>
#include <driverlib/interrupt.h>
/* TI-RTOS BIOS  */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

///* TI-RTOS Peripherals */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/uart/UARTTiva.h>


//
#include <RTOS/RTOSFlashing.h>
#include <RTOS/RTOSBluetooth.h>
#include <RTOS/RTOSDistance.h>
#include <RTOS/RTOSPID.h>
#include <RTOS/RTOSMotor.h>
#include <RTOS/RTOSLightSensor.h>
#include <RTOS/RTOSModbus.h>

/*
 * ======== Hardware Configuration ========
 */

//void Float2Str(char *c, float f);

uint32_t ui32Period;

// *** Board Initialization Function ***
void Board_Init() {
	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	//SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    RTOSFlashingInit();
    RTOSBTInit();
    RTOSDistanceInit();
    RTOSPIDInit();
    RTOSMotorInit();
    RTOSLightSensorInit();
    RTOSModbusInit();
}

// ======== main ========
int main(void){
	// Timer 3 Hardware Interrupt Setup
	// BIOS Setting in empty.cfg: 80000000 = 80 MHz
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3); // enable Timer 3 periph clks
	TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC); // cfg Timer 3 mode - periodic
	ui32Period = (SysCtlClockGet()/100); // period = CPU clk div 2 (50ms)
	TimerLoadSet(TIMER3_BASE, TIMER_A, ui32Period); // set Timer 2 period
	IntEnable(INT_TIMER3A);
	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT); // enables Timer 2 to interrupt CPU
	IntMasterEnable();
	TimerEnable(TIMER3_BASE, TIMER_A); // enable Timer 2


    Board_Init();

    BIOS_start();

    return (0);
}
