/*
 * SoftTimer.h
 *
 *  Created on: 12 Jul 2017
 *      Author: salavat.magazov
 */

#include "stdint.h"

#ifndef SOFTTIMER_H_
#define SOFTTIMER_H_

class CSoftTimer {
public:
	CSoftTimer();
	virtual ~CSoftTimer();

	/* @brief:	Start timer.
	 * @note: 	If timer is already running calling this function will restart it.
	 */
	void Start();

	/* @brief:	Stop timer.
	 * @note:	If timer is not running calling this function has no effect.
	 */
	void Stop();

	/* @brief:	Reset timer.
	 * @note:	This method returns timer to constructed state.
	 * */
	void Reset();

	/* @brief:	Get elapsed time.
	 * @ret:	Elapsed time in milliseconds since timer was started.
	 * @note:	If timer is not running return time between start and stop.
	 */
	uint32_t GetElapsedTime();

	/* @brief:	Get timer state
	 * @ret:	Return true is timer is running.
	 */
	bool IsRunning();

	/* @brief:	Advance start time of timer by a number of milliseconds.
	 * @arg:	Number of milliseconds that start time is advanced.
	 * @ret:	Return true if advance was possible, i.e. advance time is
	 * 			less than number of elapsed milliseconds.
	 */
	bool Advance(uint32_t time);

	typedef enum{
		STOPPED = 0,
		RUNNING,
	} TimerState_t;
private:

	TimerState_t	m_State;
	unsigned int	m_Start;
	unsigned int	m_Stop;
};

#endif /* SOFTTIMER_H_ */
