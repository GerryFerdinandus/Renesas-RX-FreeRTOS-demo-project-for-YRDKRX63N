/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  FreeRTOSDemo                           */
/*      FILE         :  FreeRTOSDemo.c                         */
/*      DESCRIPTION  :  Main Program                           */
/*      CPU SERIES   :  RX600                                  */
/*      CPU TYPE     :  RX63N                                  */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/                                
                                                                           
                                                                           
/************************************************************************/
/*    File Version: V1.00                                               */
/*    Date Generated: 08/07/2013                                        */
/************************************************************************/



/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "FreeRTOSConfig.h"



#include "iodefine.h"
#include <stdint.h>
#include "partest.h"


/*
 * Configure the hardware as necessary to run this demo.
 */
static void prvSetupHardware( void );

/* Prototypes for the standard FreeRTOS callback/hook functions implemented
within this file. */
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );
void vApplicationTickHook( void );

extern void main_blinky(void);


#ifdef CPPAPP
//Initialize global constructors
extern "C" void __main()
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) ();
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
    (*--p) ();

    }
}
#endif 


int main(void)
{
	/* Configure the hardware ready to run the demo. */
	prvSetupHardware();

	main_blinky();

  return 0;
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Set up the ports used by the LED outputs (the name ParTest is now
	obsolete - it originally came from "parallel port test"). */

	vParTestInitialise();

}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	taskDISABLE_INTERRUPTS();
	for( ;; )
	{
	// empty loop
	}

}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(__attribute__ ((unused)) TaskHandle_t pxTask, __attribute__ ((unused)) char *pcTaskName )
{

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; )
	{
	// empty loop
	}
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	size_t xFreeHeapSpace;

	/* This is just a trivial example of an idle hook.  It is called on each
	cycle of the idle task.  It must *NOT* attempt to block.  In this case the
	idle task just queries the amount of FreeRTOS heap that remains.  See the
	memory management section on the http://www.FreeRTOS.org web site for memory
	management options.  If there is a lot of heap memory free then the
	configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
	RAM. */
	*(volatile size_t *)(&xFreeHeapSpace) =  xPortGetFreeHeapSize();
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	//software interrupt tick


}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
unsigned long ul = 0;

	taskENTER_CRITICAL();

		/* Use the debugger to set ul to a non-zero value in order to step out
		of this function to determine why it was called. */
		while(*(volatile unsigned long *)(&ul) == 0 )
		{
			portNOP();
		}

	taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

/* The RX port uses this callback function to configure its tick interrupt.
This allows the application to choose the tick interrupt source. */
void vApplicationSetupTimerInterrupt( void )
{
const uint32_t ulEnableRegisterWrite = 0xA50BUL;
const uint32_t ulDisableRegisterWrite = 0xA500UL;

    /* Disable register write protection. */
    SYSTEM.PRCR.WORD = ulEnableRegisterWrite;

	/* Enable compare match timer 0. */
	MSTP( CMT0 ) = 0;

	/* Interrupt on compare match. */
	CMT0.CMCR.BIT.CMIE = 1;

	/* Set the compare match value. */
	CMT0.CMCOR = ( unsigned short ) ( ( ( configPERIPHERAL_CLOCK_HZ / configTICK_RATE_HZ ) -1 ) / 8 );

	/* Divide the PCLK by 8. */
	CMT0.CMCR.BIT.CKS = 0;

	/* Enable the interrupt... */
	_IEN( _CMT0_CMI0 ) = 1;

	/* ...and set its priority to the application defined kernel priority. */
	_IPR( _CMT0_CMI0 ) = configKERNEL_INTERRUPT_PRIORITY;

	/* Start the timer. */
	CMT.CMSTR0.BIT.STR0 = 1;

    /* Reneable register protection. */
    SYSTEM.PRCR.WORD = ulDisableRegisterWrite;
}
/*-----------------------------------------------------------*/

