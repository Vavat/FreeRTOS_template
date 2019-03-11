/*
 * CBooleanEXTI.cpp
 *
 *  Created on: 19 Oct 2018
 *      Author: User1
 */

#include "CBooleanEXTI.h"

extern "C"{
extern void UAOptoTrigger(uint16_t pin);
extern void OCPTrigger();
extern void KrupsSmallLEDTrigger();
extern void KrupsLargeLEDTrigger();
}

CBooleanEXTI* CBooleanEXTI::s_pEXTI0 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI1 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI2 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI3 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI4 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI5 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI6 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI7 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI8 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI9 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI10 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI11 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI12 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI13 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI14 = NULL;
CBooleanEXTI* CBooleanEXTI::s_pEXTI15 = NULL;

CBooleanEXTI::CBooleanEXTI(GPIO_TypeDef * port, uint16_t pin) :
CBooleanInput(port, pin){
}

CBooleanEXTI::~CBooleanEXTI() {
	// TODO Auto-generated destructor stub
}

void CBooleanEXTI::RegisterCallback(EXTI_Callback callback){
	switch (m_Pin){
	case GPIO_PIN_0:
		s_pEXTI0 = this;
		break;
	case GPIO_PIN_1:
		s_pEXTI1 = this;
		break;
	case GPIO_PIN_2:
		s_pEXTI2 = this;
		break;
	case GPIO_PIN_3:
		s_pEXTI3 = this;
		break;
	case GPIO_PIN_4:
		s_pEXTI4 = this;
		break;
	case GPIO_PIN_5:
		s_pEXTI5 = this;
		break;
	case GPIO_PIN_6:
		s_pEXTI6 = this;
		break;
	case GPIO_PIN_7:
		s_pEXTI7 = this;
		break;
	case GPIO_PIN_8:
		s_pEXTI8 = this;
		break;
	case GPIO_PIN_9:
		s_pEXTI9 = this;
		break;
	case GPIO_PIN_10:
		s_pEXTI10 = this;
		break;
	case GPIO_PIN_11:
		s_pEXTI11 = this;
		break;
	case GPIO_PIN_12:
		s_pEXTI12 = this;
		break;
	case GPIO_PIN_13:
		s_pEXTI13 = this;
		break;
	case GPIO_PIN_14:
		s_pEXTI14 = this;
		break;
	case GPIO_PIN_15:
		s_pEXTI15 = this;
		break;
	default:
		Error_Handler();
	}
	m_pEXTICallback = callback;
}

void CBooleanEXTI::InvokeCallback(){
	/* if callback is defined for this pin call it. */
	if(m_pEXTICallback != NULL){
		m_pEXTICallback(Read());
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t pin){
	switch (pin){
	case GPIO_PIN_0:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_1:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_2:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_3:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_4:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_5:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_6:
		KrupsLargeLEDTrigger();
		break;
	case GPIO_PIN_7:
		KrupsSmallLEDTrigger();
		break;
	case GPIO_PIN_8:
		//TODO: make a decision of this input is necessary based on roasting experiments. MainsZCTrigger();
		break;
	case GPIO_PIN_9:

		break;
	case GPIO_PIN_10:

		break;
	case GPIO_PIN_11:

		break;
	case GPIO_PIN_12:

		break;
	case GPIO_PIN_13:
		UAOptoTrigger(pin);
		break;
	case GPIO_PIN_14:

		break;
	case GPIO_PIN_15:
		//OCPTrigger();
		break;
	};
}

