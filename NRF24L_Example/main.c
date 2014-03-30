//Author: Nathan Zimmerman
//Date: 3/2/14
// Driver referenced from Brad S, supreme overlord of all C

//Launchpad CCS example of NRF24L Driver. Use P1.3 Button to toggle LED on other launchpad. Hence requires 2 NRF modules & 2 launchpads
//Note example uses a fixed packet length of 5.

//GPIO Pinouts

//P2.0 = IRQ
//P2.1 = CSN
//P2.2 = CE

//P1.5 SCLK
//P1.6 MISO
//P1.7 MOSI

//P1.3 Button Launchpad Rev 1.5
//P1.0 Red LED Launchpad Rev 1.5

#include "msp430g2553.h"
#include "stdint.h"
#include "Drivers/rtc.h"
#include "Drivers/clock.h"
#include "Drivers/usi.h"
#include "Drivers/External/NRF24L.h"
#include "Drivers/External/LAUNCHPAD_IO.h"
#include "Drivers/LCD/gui.h"
#include "Application/display.h"
#include "Schedule/os.h"
#include "Drivers/External/RGB.h"

void main(void)
{
	volatile uint8_t statusData = 0;
	setupRtcTick();
	setupCoreClock(CLK_16_MHZ);
	P3DIR |= BIT2;
	P3OUT |= BIT2;
	P2DIR |=BIT1;
	P2OUT |=BIT1;
	setupRGB();
	SERIAL_CLASSES spiHandle = { SPI, SMCLK_16MHZ_SPI_CLK_4MHZ, MODULE_B}; // NRF cannot run at max baud 16mhz
	initUSI(&spiHandle);


	initNRF24L();
	Setup_LCD();
	Draw_Home_Page();
    statusData = getNrfStatus();


    if(statusData != 0x0E)
    {
        while(1); // NRF failed to init, check pinout or device
    }

	Start_Scheduler();

}
