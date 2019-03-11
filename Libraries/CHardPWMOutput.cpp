/*
 * CHardPWMOutput.cpp
 *
 *  Created on: 1 Nov 2018
 *      Author: User1
 */

#include "CHardPWMOutput.h"

CHardPWMOutput::CHardPWMOutput():
m_pCCRegister(0),
m_pCNTRegister(0)
{

}

void CHardPWMOutput::Initialise(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t dutyCycle, bool inverted){
	m_pTimer = htim;
	m_Channel = channel;
	m_bInverted = inverted;

	/* Setup capture-compare register for easy update of duty cycle. */
	switch(m_Channel){
	case TIM_CHANNEL_1:
		m_pCCRegister = &(m_pTimer->Instance->CCR1);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	case TIM_CHANNEL_2:
		m_pCCRegister = &(m_pTimer->Instance->CCR2);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	case TIM_CHANNEL_3:
		m_pCCRegister = &(m_pTimer->Instance->CCR3);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	case TIM_CHANNEL_4:
		m_pCCRegister = &(m_pTimer->Instance->CCR4);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	case TIM_CHANNEL_5:
		m_pCCRegister = &(m_pTimer->Instance->CCR5);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	case TIM_CHANNEL_6:
		m_pCCRegister = &(m_pTimer->Instance->CCR6);
		m_pCNTRegister = &(m_pTimer->Instance->CNT);
		break;
	}
	ExtendLimits();
	setDutyCycle((unsigned int)dutyCycle);
}

CHardPWMOutput::~CHardPWMOutput() {
	// TODO Auto-generated destructor stub
}

void CHardPWMOutput::setDutyCycle(unsigned int dutyCycle) {
	if(dutyCycle > 100){
		dutyCycle = 100;
	}
	m_DutyCycle = dutyCycle;

	/* calculate pulse duration based on required duty cycle and imposed limits.*/
	//TODO: update register only after when pulse is inactive.
	*m_pCCRegister = m_LeftLimit + (m_RightLimit - m_LeftLimit)*m_DutyCycle/100;
}

void CHardPWMOutput::setDutyCycle(float dutyCycle) {
	if(dutyCycle > 100){
		dutyCycle = 100;
	}
	m_DutyCycle = dutyCycle;

	/* calculate pulse duration based on required duty cycle and imposed limits.*/
	//TODO: update register only after when pulse is inactive.
	*m_pCCRegister = m_LeftLimit + (m_RightLimit - m_LeftLimit)*dutyCycle/100.0;
}

void CHardPWMOutput::setRawPulse(unsigned int pulseDuration){
	if(pulseDuration < m_LeftLimit){
		pulseDuration = m_LeftLimit;
	}
	if(pulseDuration > m_RightLimit){
		pulseDuration = m_RightLimit;
	}

	m_PulseDuration = pulseDuration;
	*m_pCCRegister = m_PulseDuration;
}

void CHardPWMOutput::setLeftLimit(unsigned int leftLimit){
	if(leftLimit < m_RightLimit){
		StopPWM();
		m_LeftLimit = leftLimit;
		setDutyCycle(m_DutyCycle);
		StartPWM();
	}
}

void CHardPWMOutput::setRightLimit(unsigned int rightLimit){
	if(m_LeftLimit < rightLimit && rightLimit <= m_pTimer->Instance->ARR){
		StopPWM();
		m_RightLimit = rightLimit;
		setDutyCycle(m_DutyCycle);
		StartPWM();
	}
}

void CHardPWMOutput::ExtendLimits(){
	StopPWM();
	m_LeftLimit = 0;
	m_RightLimit = m_pTimer->Init.Period;
	setDutyCycle(m_DutyCycle);
	StartPWM();
}

void CHardPWMOutput::RestartTimer(){
	if(m_pCNTRegister){
		*m_pCNTRegister = 0;
	}
}
