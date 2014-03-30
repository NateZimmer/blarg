/*
 * display.c
 *
 *  Created on: Jan 29, 2014
 *      Author: Nate
 */

#include "display.h"
#include "treadmill.h"
#include "../Drivers/LCD/gui.h"
#include "../Drivers/External/NRF24L.h"
#include "../Drivers/rtc.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

static Small_Text_Button plusButton = {20,80," + "};
static Small_Text_Button minusButton = {70,80," - "};
static Small_Text_Button startButton = {140,80,"Start"};

static bool debounced = true;

typedef enum
{
	PLUS_PRESSED,
	MINUS_PRESSED,
	START_PRESSED,
	NO_EVENT
}eventHandles;

static eventHandles eventHandle = NO_EVENT;

void Draw_Home_Page()
{
	setColor16(0xFFFF);
	clearScreen(1);
	setColor16(COLOR_16_BLUE);
	fillRect(0, 0, 220, 30);
	setColor16(COLOR_16_WHITE);
	setBackgroundColor16(COLOR_16_BLUE);
	drawString11_16(10, 10, "Da Uber Desk!");
	setBackgroundColor16(COLOR_16_BLACK);
	setColor16(COLOR_16_WHITE);
	drawString8_12(5,40, "Desk Controls:");
	drawString8_12(5,60, "Speed:");
	drawString8_12(5,120, "Time:");
	drawString8_12(80,120, "Feet:");
	drawString8_12(140,120, "Miles:");

	Draw_Small_Button(&plusButton);
	Draw_Small_Button(&minusButton);
	Draw_Small_Button(&startButton);

}

void handleHomePageTouch()
{

	uint16_t xval =0;
	uint16_t yval =0;
	TOUCH_VAL(&xval, &yval);

	if(Check_Small_Touch(&plusButton, xval, yval))
	{
		eventHandle = PLUS_PRESSED;
	}
	else if(Check_Small_Touch(&minusButton, xval, yval))
	{
		eventHandle = MINUS_PRESSED;
	}
	else if(Check_Small_Touch(&startButton, xval, yval))
	{
		eventHandle = START_PRESSED;
	}
}

void startStopPressed()
{
	if(!getTreadmillOnStatus())
	{
		clearDistance();
		clearSystemTime();
		Draw_Home_Page();
		setTreadmillStatus(true);
		setTreadmillState(MOVING);
		strcpy(startButton.text,"Stop");
		Draw_Small_Button(&startButton);
	}
	else
	{
		setTreadmillStatus(false);
		setTreadmillState(NO_MOVEMENT);
		strcpy(startButton.text,"Start");
		Draw_Small_Button(&startButton);
	}
	eventHandle = NO_EVENT;
}

void handleDebounce()
{
	static uint8_t i = 0;

	if(!debounced)
	{
		i++;
		if(i==2)
		{
			i=0;
		}
		debounced = true;
		eventHandle = NO_EVENT;
	}

}

void handleEvents()
{
	handleHomePageTouch();
	if(debounced)
	{
		switch(eventHandle)
		{
		case PLUS_PRESSED:
			incrementSpeed();
			debounced = false;
			break;
		case MINUS_PRESSED:
			decrementSpeed();
			debounced = false;
			break;
		case START_PRESSED:
			startStopPressed();
			debounced = false;
			break;
		case NO_EVENT:
			break;
		}
		eventHandle = NO_EVENT;
	}
}

void getTime(char * timeText)
{
	SYSTEM_TIME currentTime = {0,0,0};
	getSystemTime(&currentTime);
	timeText[0] = currentTime.hours + '0';
	timeText[1] = ':';
	timeText[2] = currentTime.minutes/10 + '0';
	timeText[3] = currentTime.minutes%10 + '0';
	timeText[4] = ':';
	timeText[5] = currentTime.seconds/10 + '0';
	timeText[6] = currentTime.seconds%10 + '0';
	timeText[7] = 0;

}

void displayDistance()
{
	char buffer[10];
	setColor16(COLOR_16_GREEN);
	getMilesArray(buffer);
	drawString8_12(140, 140, buffer);
	getFeetArray(buffer);
	drawString8_12(80, 140, buffer);


}


void handlePageDisplay()
{
	char buffer[10];
	setColor16(COLOR_16_GREEN);
	getSpeedText(buffer);
	drawString8_12(60, 60, buffer);
	if(getTreadmillOnStatus())
	{
		getTime(buffer);
		drawString8_12(5, 140, buffer);
	}

}

