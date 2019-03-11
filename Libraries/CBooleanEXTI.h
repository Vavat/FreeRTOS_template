/*
 * CBooleanEXTI.h
 *
 *  Created on: 19 Oct 2018
 *      Author: User1
 */

#ifndef CBOOLEANEXTI_H_
#define CBOOLEANEXTI_H_

#include <CBooleanInput.h>

class CBooleanEXTI: public CBooleanInput {
public:
	CBooleanEXTI(GPIO_TypeDef * port, uint16_t pin);
	virtual ~CBooleanEXTI();
	/* declare type of function that can be registered as callback when
	 * interrupt on an external pin is triggered. */
	typedef void (*EXTI_Callback)(bool state);

	void RegisterCallback(EXTI_Callback callback);

	void InvokeCallback();

	static CBooleanEXTI * s_pEXTI0;
	static CBooleanEXTI * s_pEXTI1;
	static CBooleanEXTI * s_pEXTI2;
	static CBooleanEXTI * s_pEXTI3;
	static CBooleanEXTI * s_pEXTI4;
	static CBooleanEXTI * s_pEXTI5;
	static CBooleanEXTI * s_pEXTI6;
	static CBooleanEXTI * s_pEXTI7;
	static CBooleanEXTI * s_pEXTI8;
	static CBooleanEXTI * s_pEXTI9;
	static CBooleanEXTI * s_pEXTI10;
	static CBooleanEXTI * s_pEXTI11;
	static CBooleanEXTI * s_pEXTI12;
	static CBooleanEXTI * s_pEXTI13;
	static CBooleanEXTI * s_pEXTI14;
	static CBooleanEXTI * s_pEXTI15;

private:
	EXTI_Callback m_pEXTICallback;
};

#endif /* CBOOLEANEXTI_H_ */
