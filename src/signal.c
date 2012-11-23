/**
* @file signal.c
*
* @brief Controls modulation of 38.4 kHz carrier wave and baud rate of ASCII characters
*
* Carrier wave is generated on P2 by flipping its signal every 26 micro s (empirically found to be 5 NOP cycles).
* Baud rate is determined by multiplying the carrier wave loop with a constant.
* Consequently, baud rate is controlled by changing the value of the constant.
*
* This integer constant is determined by the following formula:
*             _                             _
*            |          38400 Hz             |
*  Cycles =  | ----------------------------  |  - 1
*            |  Required baud rate (in Hz)   |
*            |_                             _|
*
* @note Carrier wave is transmitted only during the interval that ASCII signals are being transmitted
* @note Since 'cycles' is an integer, baud rate cannot go beyond 38400Hz.
*      In fact, since 38.4 kHz is the carrier wave frequency, it was empirically found 
*      that any ASCII transmission frequency beyond 4800 Hz clashes with the carrier 
*      signal, and hence hinders communication.
*      Therefore, this application limits baud rate to 4800 Hz.
*/

#include <msp430.h>                // Req'd because we refer to P6OUT
#include <salvo.h>                 // Req'd because we call sprintf()

#include "signal.h"                // Good to self-reference
#include "main.h"                  // Application header
#include "msg.h"                   // Req'd because we call MsgTS()

static unsigned int arrayCounter = 1;     // Default value corresponds to 2400 bps
static unsigned int cycles = 31;          //  "  
static unsigned int delay;

// Contains different baud rates and the respective number of NOP cycles required to attain them.
int baud[3][2] ={
					63,                         // Value of 'cycles' required to achieve 1200 bps
					1200,                       // Baud rate of 1200 bps stored to be displayed in UI

					31,                         // Value of 'cycles' required to achieve 2400 bps
					2400,                       // Baud rate of 2400 bps stored to be displayed in UI

					15,                         // Value of 'cycles' required to achieve 4800 bps
					4800                        // Baud rate of 4800 bps stored to be displayed in UI
				};

/**
increaseBaud()

Increases the length of signal state when the user presses 'a' in UI.
For every press, arrayCounter is increased by 1.
Unless already at the highest baud rate, 'cycles' is set to its next value.
*/
void increaseBaud(void)
{
    // Checks if baud rate is already the highest possible or not
    if(arrayCounter<2){
    arrayCounter++;
    cycles = baud[arrayCounter][0];
            sprintf(strTmp, "  " STR_BAUD_CONTROL "Baud rate increased to %i bps.", baud[arrayCounter][1]);
            MsgTS(strTmp);
    } else {
      MsgTS("  " STR_BAUD_CONTROL "Baud rate cannot go beyond 4800 bps.");
    }
}

/**
decreaseBaud()

Decreases the length of signal state when the user presses 'z' in UI.
For every press, arrayCounter is decreased by 1.
Unless already at the lowest baud rate, 'cycles' is set to its previous value.
*/
void decreaseBaud(void)
{
    // Checks if baud rate is already the lowest possible or not
    if(arrayCounter>0){
    arrayCounter--;
    cycles = baud[arrayCounter][0];
            sprintf(strTmp, "  " STR_BAUD_CONTROL "Baud rate decreased to %i bps.", baud[arrayCounter][1]);
            MsgTS(strTmp);
    } else {
      MsgTS("  " STR_BAUD_CONTROL "Baud rate cannot go below 1200 bps.");
    }
}

/**
baudControl()

Calls the carrier wave loop [modDelay()] 'cycles' number of times.
After each call, P2 bits are flipped to generate a 38.4 kHz square carrier wave.
Four P2 bits are flipped since the accompanying circuit requires each LED to have its own carrier wave.

baudControl() is called in by TaskDriver() before it loops again to change the P5 ASCII signals.
While baudControl() is working, all P5 signals maintain their state.
*/
void baudControl( void )        
{
    delay=cycles;
    do {
       modDelay();

       // P2 bits toggled in two steps so that the loop takes ~26 micro s (~34.8 kHz).
       // Empirically determined.
       P2OUT ^= BIT1 + BIT3;              // Toggles P2.1 and P2.3
       P2OUT ^= BIT5 + BIT7;              // Toggles P2.5 and P2.7  
   } while (delay--);
}


/**
modDelay()

Loops MOD_CYCLES number of times before letting P2 bits to be toggled in the baudControl() loop.
Ensures that the time between successive toggles is ~26 micro s.
Thereby, ensures a 38.4 khz carrier wave on the configured P2 bits.
*/
void modDelay ( void )
{
  unsigned int modCycles = MOD_CYCLES;
    do {
      NOP;
   } while (modCycles--);
}




