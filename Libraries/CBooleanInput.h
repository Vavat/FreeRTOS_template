/*
 * CBooleanInput.h
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#ifndef CBOOLEANINPUT_H_
#define CBOOLEANINPUT_H_

#include "CBooleanIO.h"

class CBooleanInput : protected CBooleanIO {
public:
	CBooleanInput(GPIO_TypeDef * port, uint16_t pin);
	virtual ~CBooleanInput();

	bool Read();
};

#endif /* CBOOLEANINPUT_H_ */
