/*
 * SoftTimer.cpp
 *
 *  Created on: 12 Jul 2017
 *      Author: salavat.magazov
 */

#include "SoftTimer.h"
#include "cmsis_os.h"

CSoftTimer::CSoftTimer() :
m_State(STOPPED),
m_Start(0),
m_Stop()
{
}

CSoftTimer::~CSoftTimer() {
	// TODO Auto-generated destructor stub
}

void CSoftTimer::Start(){
	m_Start = osKernelSysTick();
	m_State = RUNNING;
}

void CSoftTimer::Stop(){
	m_Stop = osKernelSysTick();
	m_State = STOPPED;
}

void CSoftTimer::Reset(){
	m_Start = 0;
	m_Stop = 0;
	m_State = STOPPED;
}

uint32_t CSoftTimer::GetElapsedTime(){
	uint32_t elapsedTime;
	if(m_State == STOPPED){
		if(m_Stop >= m_Start){
			elapsedTime = m_Stop - m_Start;
		}
		else{
			elapsedTime = 0xFFFFFFFF - m_Start;
			elapsedTime += m_Stop;
		}
	}
	else{
		uint32_t currentTime = osKernelSysTick();
		if(currentTime > m_Start){
			elapsedTime = currentTime - m_Start;
		}
		else{
			elapsedTime = 0xFFFFFFFF - m_Start;
			elapsedTime += currentTime;
		}
	}
	return elapsedTime;
}

bool CSoftTimer::IsRunning(){
	return (m_State == RUNNING);
}

bool CSoftTimer::Advance(uint32_t time){
	bool bAdvanced = false;
	uint32_t tempTime;
	if(time <= GetElapsedTime()){
		tempTime = m_Start + time;
		if(tempTime > m_Start){
			m_Start = tempTime;
		}
		else{
			tempTime = 0xFFFFFFFF - m_Start;
			m_Start = time - tempTime;
		}
		bAdvanced = true;
	}
	return bAdvanced;
}
