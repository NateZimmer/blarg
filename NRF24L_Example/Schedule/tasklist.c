/*
 * tasklist.c
 *
 *  Created on: Sep 22, 2013
 *      Author: Nate
 */


#include "tasklist.h"
#include "soft_tasks.h"
#include "../Drivers/External/NRF24L.h"
#include "../Application/display.h"
#include "../Application/treadmill.h"
#include "../Drivers/LCD/touch.h"
#include "../Drivers/External/RGB.h"

#define MS_2_TASK		BIT0
#define MS_4_TASK		BIT1
#define MS_8_TASK		BIT2
#define MS_16_TASK		BIT3
#define MS_33_TASK		BIT4
#define MS_65_TASK		BIT5
#define MS_131_TASK		BIT6
#define MS_262_TASK		BIT7

void run_2_ms_task()
{
    handleRxData();
    handleTxData();
}

void run_4_ms_task()
{

}

void run_8_ms_task()
{
	runScreenTimeoutTimer();
}

void run_16_ms_task()
{
	handleTreadmillSpeed();
	run_touch_task();
}

void run_33_ms_task()
{
	handleEvents();
	fadeRgb();
}

void run_65_ms_task()
{
	handleLEDs();
}

void run_131_ms_task()
{
	calculateDistance();
}

void run_262_ms_task()
{
	handleDebounce();
	displayDistance();
	handlePageDisplay();
}





