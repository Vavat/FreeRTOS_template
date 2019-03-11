/*
 * CDebugController.h
 *
 *  Created on: 18 Oct 2018
 *      Author: User1
 */

#ifndef CDEBUGCONTROLLER_H_
#define CDEBUGCONTROLLER_H_

#include "CThreadBase.h"
#include "..\Libraries\CBooleanOutput.h"

class CDebugController : public CThreadBase {
public:
	CDebugController();
	virtual ~CDebugController();

	virtual void Run(void const * arg = NULL);

	virtual bool NewCommand(char const * const command, char * const reply);

private:
	CBooleanOutput		m_Heartbeat;

};

#endif /* CDEBUGCONTROLLER_H_ */
