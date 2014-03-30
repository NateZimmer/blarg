/*
 * treadmill.h
 *
 *  Created on: Mar 8, 2014
 *      Author: Nate
 */

#ifndef TREADMILL_H_
#define TREADMILL_H_

#include "msp430g2553.h"
#include "stdint.h"
#include "stdbool.h"


#define SPEED_LIMIT 99
#define SPEED_MIN 10

typedef enum
{
	MOVING,
	NO_MOVEMENT,
	TIMED_OUT
}TREADMILL_STATES;


void handleTreadmillSpeed();
void incrementSpeed();
void decrementSpeed();
bool getTreadmillOnStatus();
uint8_t getTreadmillSpeed();
void calculateDistance();
void getFeetArray(char * feet);
void getMilesArray(char * miles);
void clearDistance();
void getSpeedText(char * speedText);
void setTreadmillStatus(bool startTread);
void handleLEDs();
void runScreenTimeoutTimer();


void setTreadmillState(TREADMILL_STATES TREADMILL);
TREADMILL_STATES getTreadmilllState();


#endif /* TREADMILL_H_ */
