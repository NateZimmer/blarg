/*
 * display.h
 *
 *  Created on: Jan 29, 2014
 *      Author: Nate
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "msp430g2553.h"
#include "stdint.h"
#include "stdbool.h"

void Draw_Home_Page();
void updateWeight();
void weightState();
void adcFilter();
void getWeightString(char * buffer);
void handlePageDisplay();
void handleEvents();
void handleDebounce();
void displayDistance();



#endif /* DISPLAY_H_ */
