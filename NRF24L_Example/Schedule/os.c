/*
 * os.c
 *
 *  Created on: Apr 12, 2013
 *      Author: Nate
 */


#include "os.h"
#include "tasklist.h"
#include "msp430g2553.h"
#include "../Drivers/rtc.h"

#define MS_2_TASK		BIT0
#define MS_4_TASK		BIT1
#define MS_8_TASK		BIT2
#define MS_16_TASK		BIT3
#define MS_33_TASK		BIT4
#define MS_65_TASK		BIT5
#define MS_131_TASK		BIT6
#define MS_262_TASK		BIT7

void run_scheduler(uint8_t current_tick)
{
	if(current_tick & MS_2_TASK)
	{
		run_2_ms_task();
	}
	else if(current_tick & MS_4_TASK)
	{
		run_4_ms_task();
	}
	else if(current_tick & MS_8_TASK)
	{
		run_8_ms_task();
	}
	else if(current_tick & MS_16_TASK)
	{
		run_16_ms_task();
	}
	else if(current_tick & MS_33_TASK)
	{
		run_33_ms_task();
	}
	else if(current_tick & MS_65_TASK)
	{
		run_65_ms_task();
	}
	else if(current_tick & MS_131_TASK)
	{
		run_131_ms_task();
	}
	else if(current_tick & MS_262_TASK)
	{
		run_262_ms_task();
	}
}

void check_scheduler()
{
	static uint8_t previous_ticks=0;

	if(previous_ticks != get_system_tick())
	{
		run_scheduler(previous_ticks);
		previous_ticks++;
	}
}

void Start_Scheduler()
{
	while(1)
	{
		check_scheduler();
	}
}

