/*
 * CBooleanOutput.cpp
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#include <CBooleanOutput.h>

CBooleanOutput::CBooleanOutput(GPIO_TypeDef * port, uint16_t pin) :
CBooleanIO(port, pin) {
}

CBooleanOutput::~CBooleanOutput() {
	// TODO Auto-generated destructor stub
}

void CBooleanOutput::Set(bool state){
	if(state){
		m_pPort->BSRR = (uint32_t)m_Pin;
	}else{
		m_pPort->BRR = (uint32_t)m_Pin;
	}
}

void CBooleanOutput::Toggle(){
	if((m_pPort->IDR & m_Pin)){
		m_pPort->BRR = (uint32_t)m_Pin;
	}else{
		m_pPort->BSRR = (uint32_t)m_Pin;
	}
}
