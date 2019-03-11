/*
 * Threads.cpp
 *
 *  Created on: 3 May 2017
 *      Author: Vavat
 */

#include <cmsis_os.h>
#include "usart.h"

#include "UARTCommsController.h"
#include "CDebugController.h"

/* instantiate all controllers here. */
UARTCommsController DebugChannel;
CDebugController DebugController;

/* thread entry point function prototypes
 * must be enclosed in "C" definition to avoid C++ name mangling.
 */

extern "C"{
/* prototype function definition for thread entry points. */
/* Thread entry point function name template:
 * StartNameOfThread(void const * arg) _Noreturn;
 */

void StartDebugComsThread(void const * arg) _Noreturn;
void StartDebugController(void const * arg) _Noreturn;

void StartThreads(const void *){

	/* Initialise comms channel */
	DebugChannel.Initialise(&huart2, true);

	/* Register controllers with UART channels to allow them to respond to commands. */
	DebugChannel.RegisterController(&DebugController);

	/* create all threads here */
	osThreadId DebugComsThreadHandle;
	osThreadId DebugControllerThreadHandle;

	/* Comms threads */
	osThreadDef(DebugComsThread, StartDebugComsThread, osPriorityNormal, 0, 1000);
	DebugComsThreadHandle = osThreadCreate(osThread(DebugComsThread), NULL);
	configASSERT(DebugComsThreadHandle);

	/* Controller threads. */
	osThreadDef(DebugControllerThread, StartDebugController, osPriorityNormal, 0, 128);
	DebugControllerThreadHandle = osThreadCreate(osThread(DebugControllerThread), NULL);
	configASSERT(DebugControllerThreadHandle);

	/* Terminate this thread here.
	 * Other threads will not run until this one terminates because it has highest priority. */
	osThreadTerminate(osThreadGetId());
}


	/* Thread entry points. These entries should only call ::Run methods of associated CThread::
	 * class. Once that's done, control is in the hands of the class instance.
	 */

/* Controllers thread entry */
/* add controllers entry points here */

/* UART thread entry */
void StartDebugComsThread(void const * arg){
	DebugChannel.Run();
	/* should never get this far. Run method does not return. */
	Error_Handler();
}

/* Controllers thread entry */
void StartDebugController(void const * arg){
	DebugController.Run(arg);
	Error_Handler();
}

}; // end of extern "C" section
