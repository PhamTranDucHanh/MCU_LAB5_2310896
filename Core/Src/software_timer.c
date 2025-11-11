/*
 * software_timer.c
 *
 *  Created on: Sep 9, 2025
 *      Author: HANH
 */


#include "software_timer.h"
#define timer_cycle 	1
#define NO_OF_TIMER		10
struct TimerStruct timerArray[NO_OF_TIMER];

void timerSet(int idx, int counter){
	timerArray[idx].counter = counter / timer_cycle;
	timerArray[idx].flag = 0;
}

void timerRun(){
	for (int i = 0; i < NO_OF_TIMER; i++){
		if(timerArray[i].counter > 0){
			timerArray[i].counter--;
			if (timerArray[i].counter <= 0){
				timerArray[i].flag = 1;
			}
		}
	}
}

int timerFlag(int idx){
	if(timerArray[idx].flag){
		return 1;
	}
	return 0;
}
