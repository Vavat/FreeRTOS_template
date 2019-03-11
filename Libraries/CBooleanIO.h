/*
 * CBooleanIO.h
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#ifndef CBOOLEANIO_H_
#define CBOOLEANIO_H_

#include <stm32L4xx.h>

class CBooleanIO {
public:
	CBooleanIO(GPIO_TypeDef * port, uint16_t pin);
	virtual ~CBooleanIO();

	GPIO_TypeDef * GetGPIOPortReference() const {return m_pPort;};

	uint16_t GetPin() const {return m_Pin;};

protected:
	GPIO_TypeDef * 	m_pPort;
	uint16_t		m_Pin;
};

#endif /* CBOOLEANIO_H_ */
