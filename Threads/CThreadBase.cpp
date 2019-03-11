/*
 * CThreadBase.cpp
 *
 *  Created on: 13 May 2017
 *      Author: Vavat
 */

#include "CThreadBase.h"
#include <string.h>
#include <stdio.h>

CThreadBase::CThreadBase() {
}

CThreadBase::~CThreadBase(){
}

bool CThreadBase::NewCommand(char const * const command, char * const reply){
	return false;
}

void CThreadBase::SetTaskHandle(TaskHandle_t taskHandle){
	if(taskHandle != NULL){
		m_TaskHandle = taskHandle;
	}else{
		m_TaskHandle = xTaskGetCurrentTaskHandle();
	}
}
