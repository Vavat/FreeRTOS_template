/*
 * CBooleanInput.cpp
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#include <CBooleanInput.h>
#include "stm32l4xx_hal_gpio.h"

CBooleanInput::CBooleanInput(GPIO_TypeDef * port, uint16_t pin):
CBooleanIO(port, pin) {
}

CBooleanInput::~CBooleanInput() {
	// TODO Auto-generated destructor stub
}

bool CBooleanInput::Read(){
	if((m_pPort->IDR & m_Pin)){
		return true;
	}else{
		return false;
	}
}
