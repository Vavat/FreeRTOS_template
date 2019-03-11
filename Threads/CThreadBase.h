/*
 * CThreadBase.h
 *
 *  Created on: 13 May 2017
 *      Author: Vavat
 */
#pragma once

#include "cmsis_os.h"
#include <string.h>
#include <stm32l476xx.h>
#include <stdio.h>
#include <task.h>

#ifndef CTHREADBASE_H_
#define CTHREADBASE_H_

class UARTCommsController;

class CThreadBase {
public:
	CThreadBase();
	virtual ~CThreadBase();

	/* @brief:	Main thread entry point.
	 * @arg:	standard thread entry argument pointer.
	 */
	virtual void Run(void const * arg = NULL) _Noreturn = 0 ;

	virtual bool NewCommand(char const * const command, char * const reply);

	TaskHandle_t GetTaskHandle() const {return m_TaskHandle; };

	void SetTaskHandle(TaskHandle_t taskHandle = NULL);

protected:
	TaskHandle_t	m_TaskHandle;
};

#endif /* CTHREADBASE_H_ */
