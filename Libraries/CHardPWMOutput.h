/*
 * CHardPWMOutput.h
 *
 *  Created on: 1 Nov 2018
 *      Author: User1
 */

#ifndef CHARDPWMOUTPUT_H_
#define CHARDPWMOUTPUT_H_

#include "stm32l4xx.h"

class CHardPWMOutput {
public:
	CHardPWMOutput();
	virtual ~CHardPWMOutput();

	unsigned int getDutyCycle() const {
		return m_DutyCycle;
	}

	void Initialise(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t dutyCycle = 0, bool inverted = false);

	void StartPWM() const { HAL_TIM_PWM_Start(m_pTimer, m_Channel); };

	void StopPWM() const{ HAL_TIM_PWM_Stop(m_pTimer, m_Channel); };

	void setDutyCycle(unsigned int dutyCycle);
	void setDutyCycle(float dutyCycle);

	unsigned int getLeftLimit() const {
		return m_LeftLimit;
	}

	void setRawPulse(unsigned int pulseDuration);

	unsigned int getRawPulse() const {return m_PulseDuration; };

	void setLeftLimit(unsigned int leftLimit);

	unsigned int getRightLimit() const {
		return m_RightLimit;
	}

	void setRightLimit(unsigned int rightLimit);

	void ExtendLimits();

	void RestartTimer();

private:

	TIM_HandleTypeDef *		m_pTimer;
	uint32_t 				m_Channel;
	volatile uint32_t*		m_pCCRegister;
	volatile uint32_t*		m_pCNTRegister;

	unsigned int			m_DutyCycle;
	unsigned int 			m_RightLimit;
	unsigned int 			m_LeftLimit;
	unsigned int 			m_PulseDuration;
	bool					m_bInverted;
};

#endif /* CHARDPWMOUTPUT_H_ */
