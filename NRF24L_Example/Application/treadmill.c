/*
 * treadmill.c
 *
 *  Created on: Mar 8, 2014
 *      Author: Nate
 */


#include "treadmill.h"
#include "stdio.h"
#include "../Drivers/External/NRF24L.h"
#include "../Drivers/rtc.h"
#include "../Drivers/External/RGB.h"
#include "../Drivers/timer.h"
#include "../Drivers/LCD/colorLCD.h"
#include "../Drivers/LCD/touch.h"

#define FT_PER_ONE_SECOND 1.4667
#define MAX_FEET_BUCKET_SIZE 5
#define FEET_PER_DECI_MILE 528

static float distanceBucket = 0;
static uint16_t distanceFeet = 0;
static uint16_t totalFeet = 0;
static uint8_t deciMiles =0;

static uint8_t speed = 10;
static bool start = false;

#define SECONDS_TO_SCREEN_TIMEOUT 60

static uint8_t txMessage[PAYLOAD_WIDTH]= {15,0,0,0,0};
TREADMILL_STATES TREADMILL_STATE = NO_MOVEMENT;

secondCounters screenTimeout = {SECONDS_TO_SCREEN_TIMEOUT,0};

void runScreenTimeoutTimer()
{
	uint16_t xval = 0;
	uint16_t yval = 0;
	TOUCH_VAL(&xval, &yval);

	if(yval | xval)
	{
		resetTimer(&screenTimeout,SECONDS_TO_SCREEN_TIMEOUT);
		TREADMILL_STATE=NO_MOVEMENT;
		startTimer(&screenTimeout);
	}
	else if(TREADMILL_STATE==NO_MOVEMENT)
	{
		runSecondsTimer(&screenTimeout);
		if(checkTimerIfExpired(&screenTimeout))
		{
			TREADMILL_STATE=TIMED_OUT;
		}
	}
	else if(TREADMILL_STATE==MOVING)
	{
		resetTimer(&screenTimeout,SECONDS_TO_SCREEN_TIMEOUT);
	}

}



void setTreadmillState(TREADMILL_STATES TREADMILL)
{
	TREADMILL_STATE = TREADMILL;
}

TREADMILL_STATES getTreadmilllState()
{
	return TREADMILL_STATE;
}


void handleLEDs()
{
	static TREADMILL_STATES previousTreadState = NO_MOVEMENT;

	if( getTreadmilllState()!=previousTreadState)
	{
		previousTreadState = getTreadmilllState();

		switch(previousTreadState)
		{
		case NO_MOVEMENT:
			LCD_BLIGHT_ON;
			rgbOff();
			break;
		case MOVING:
			LCD_BLIGHT_ON;
			rgbOff();
			break;
		case TIMED_OUT:
			LCD_BLIGHT_OFF;
			rgbOn();
			break;
		default:
			break;
		}
	}

}

void incrementSpeed()
{
	if(speed<SPEED_LIMIT)
	{
		speed++;
	}
}

void decrementSpeed()
{
	if(speed>SPEED_MIN)
	{
		speed--;
	}
}

bool getTreadmillOnStatus()
{
	return start;
}

void setTreadmillStatus(bool startTread)
{
	start = startTread;
}

uint8_t getTreadmillSpeed()
{
	return speed;
}



void handleTreadmillSpeed()
{
	if(getTreadmillOnStatus())
	{
		txMessage[0] = getTreadmillSpeed();
		transmitTxData((uint8_t *)txMessage);
	}
}

void calculateDistance()
{
	static uint8_t previousSecond =0;
	uint8_t currentSpeed = getTreadmillSpeed();
	uint8_t currentSecond = getSystemSeconds();

	if(!getTreadmillOnStatus())
	{
		currentSpeed = 0;
	}


	while(previousSecond != currentSecond)
	{
		previousSecond = (previousSecond + 1)%60;
		distanceBucket = distanceBucket + (((float)currentSpeed)/10)*((float)FT_PER_ONE_SECOND);
		if(distanceBucket>MAX_FEET_BUCKET_SIZE)
		{
			distanceBucket -=MAX_FEET_BUCKET_SIZE;
			distanceFeet +=MAX_FEET_BUCKET_SIZE;
			totalFeet += MAX_FEET_BUCKET_SIZE;

			if(distanceFeet>FEET_PER_DECI_MILE)
			{
				distanceFeet-=FEET_PER_DECI_MILE;
				deciMiles++;
			}
		}
	}
}

void handleScreenTimeout()
{
	static uint8_t previousSecond =0;
	static uint8_t totalSecondCounter = 0;
	uint8_t currentSpeed = getTreadmillSpeed();
	uint8_t currentSecond = getSystemSeconds();

	if(getTreadmilllState()==NO_MOVEMENT)
	{
		while(previousSecond != currentSecond)
		{
			previousSecond = (previousSecond + 1)%60;
			totalSecondCounter++;
			if(totalSecondCounter>SECONDS_TO_SCREEN_TIMEOUT)
			{
				setTreadmillState(TIMED_OUT);
				totalSecondCounter=0;
			}
		}
	}
	else if(getTreadmilllState()==MOVING)
	{
		totalSecondCounter=0;
	}
}





void getFeetArray(char * feet)
{
	sprintf(feet,"%d  ",totalFeet);
}

void getMilesArray(char * miles)
{
	miles[0] = deciMiles/10 +'0';
	miles[1] = '.';
	miles[2] = deciMiles%10 +'0';
	miles[3] = 0;
}

void getSpeedText(char * speedText)
{
	speedText[0] = speed/10 + '0';
	speedText[1] = '.';
	speedText[2] = speed%10 + '0';
	speedText[3] = 'm';
	speedText[4] = 'p';
	speedText[5] = 'h';
	speedText[6] = 0;
}


void clearDistance()
{
	distanceBucket = 0;
	distanceFeet = 0;
	totalFeet = 0;
	deciMiles =0;
}



