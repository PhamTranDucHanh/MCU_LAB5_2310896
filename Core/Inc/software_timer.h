/*
 * software_timer.h
 *
 *  Created on: Sep 9, 2025
 *      Author: HANH
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include "main.h"

struct TimerStruct
{
	int counter;
	int flag;
};

void timerSet(int idx, int counter);
int timerFlag(int idx);
void timerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */
