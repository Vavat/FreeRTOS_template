/*
 * CBooleanIO.cpp
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#include "CBooleanIO.h"
#include <stm32l4xx_hal_gpio.h>

CBooleanIO::CBooleanIO(GPIO_TypeDef * port, uint16_t pin) :
m_pPort(port),
m_Pin(pin) {
	//assert(IS_GPIO_PIN(GPIO_Pin));
	//TODO: check if correctly configured. Not critical.
}

CBooleanIO::~CBooleanIO() {
	// TODO Auto-generated destructor stub
}
