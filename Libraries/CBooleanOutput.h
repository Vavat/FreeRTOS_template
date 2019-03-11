/*
 * CBooleanOutput.h
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#ifndef CBOOLEANOUTPUT_H_
#define CBOOLEANOUTPUT_H_

#include "CBooleanIO.h"

class CBooleanOutput: protected CBooleanIO {
public:
	CBooleanOutput(GPIO_TypeDef * port, uint16_t pin);
	virtual ~CBooleanOutput();

	void Set(bool state);

	void Toggle();
};


#endif /* CBOOLEANOUTPUT_H_ */

