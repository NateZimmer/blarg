/*
 * soft_tasks.c
 *
 *  Created on: Sep 23, 2013
 *      Author: Nate
 */

#include "stdint.h"
#include "soft_tasks.h"
#include "msp430g2553.h"

void run_524_ms_task()
{

}

void run_1048_ms_task()
{

}

void manage_soft_tasks()
{
	static uint8_t softTicks =0;

	if(softTicks & BIT0)
	{
		run_524_ms_task();
	}
	else if(softTicks & BIT1)
	{
		run_1048_ms_task();
	}
	softTicks++;
	if(softTicks>3)
	{
		softTicks = 0;
	}
}

