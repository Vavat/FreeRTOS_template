/*
 * CDebugController.cpp
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#include "CDebugController.h"
#include <cmsis_os.h>
#include "gpio.h"
#include "stm32l4xx.h"

#if __has_include("..\buildid.h")
#include "..\buildid.h"
#endif

#ifndef BUILD_ID
#define BUILD_ID "Version not defined. Do not use for testing."
#endif

#ifndef COMMIT_HASH
#define COMMIT_HASH "Commit hash not found. Do not use for testing."
#endif

/* internal temperature sensor calibration coefficients are stored in special
 * memory location
 */
#define TEMP110_CAL_VALUE		((uint16_t*)((uint32_t)0x1FFF75A8))
#define TEMP30_CAL_VALUE		((uint16_t*)((uint32_t)0x1FFF75CA))

CDebugController::CDebugController() :
m_Heartbeat(GPIOA, GPIO_PIN_2){
	// TODO Auto-generated constructor stub

}

CDebugController::~CDebugController() {
	// TODO Auto-generated destructor stub
}

void CDebugController::Run(void const * arg){
	uint32_t previousTime = osKernelSysTick() * portTICK_PERIOD_MS;

	for(;;){
		//TODO: job of this controller will be to collect performance data.
		m_Heartbeat.Set(true);
		osDelay(50);
		m_Heartbeat.Set(false);
		osDelayUntil(&previousTime, 1000);
	}
}

bool CDebugController::NewCommand(char const * const command, char * const reply){
	/* This command will reset the processor. */
	if(strcmp(command, "reset") == 0){
		NVIC_SystemReset();
	}

	/* this command will fail and cause watchdog reset. */
	if(strcmp(command, "fail") == 0){
		taskENTER_CRITICAL();
		for(;;){}
	}

	/* this command will succeed.*/
	if(strcmp(command, "succeed") == 0)	{
		sprintf(reply, "success");
		return true;
	}

	if(strcmp(command, "?version") == 0){
		sprintf(reply, "Version: %s", BUILD_ID);
		return true;
	}

	if(strcmp(command, "?hash") == 0){
		sprintf(reply, "Commit hash: %s", COMMIT_HASH);
		return true;
	}

	return false;
}
