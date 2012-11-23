/**
* @file driver.c
*
* @brief Controls signals of ASCII characters transmitted.
*
* Binary signals transmitted to P5, from LSB to MSB; including start, stop and redundant bits.
* P5.0 --> 0,0,0,0,1,1,1,1,0,1
* P5.1 --> 0,1,1,0,0,0,1,1,0,1
* P5.2 --> 0,0,1,1,0,1,0,1,0,1
* P5.3 --> 0,1,0,1,1,0,0,1,0,1
* P5.4 --> 0,1,0,1,0,1,0,1,0,1
* P5.5 --> 0,0,0,0,0,0,0,0,0,1
* P5.6 --> 1,1,1,1,1,1,1,1,1,1
* P5.7 --> 0,0,0,0,0,0,0,0,0,1
*
* @note ASCII chosen such that there are at least three bits different between the binay forms of any two characters.
*       'x' 0111 1000
*       'c' 0110 0011
*       'V' 0101 0110
*       'M' 0100 1101
*/

#include <msp430.h>               // Req'd because we refer to P5OUT
#include <salvo.h>                // Req'd because we call OSDelay() 

#include "driver.h"               // Good to self-reference
#include "main.h"                 // Application header
#include "msg.h"                  // Req'd because we call MsgTS()
#include "signal.h"               // Req'd because we call baudControl()
#include "counter.h"              // Req'd because we call counter()

//10 elements made up of start, stop and 8 bits.
char LEDarray[START_BITS+DATA_BITS+STOP_BITS] = {   
0xFF,
0x40,
0x5F,
0x43,
0x55,
0x49,
0x5C,
0x46,
0x5A,
0x40
};


/**
TaskDriver()

@brief Signal driving task

Loops through the LEDarray to emit every hex. All ASCII signals sent to P5.
baudControl() is used to alter the baud rate of ASCII signal transmission.
While baudControl() is working, all P5 signals maintain their state.

This application allows ASCII transmission at 1200, 2400 and 4800 bps.

counter() called after every loop to increment number of transmitted signal blocks by 1.
OS_Delay() is called after every loop to separate signal blocks.
During OS_Delay(), neither the ASCII signals nor the carrier waves are being transmitted.
This avoids the receiver getting confused with overlapping signals.

If application works correctly, then receiving end should receive:
  P5.0: 'x'
  P5.1: 'c'
  P5.2: 'V'
  P5.3: 'M'

@note This task works only for an 8 bit port.
@note Interrupts disabled during the period that ASCII characters are being transmitted
*/
void TaskDriver ( void )     
{
  static unsigned int i = 0;
  static unsigned s;

  // Startup message
  MsgTS(STR_TASK_DRIVER "Starting.");
  MsgTS("  Communicating at " DEFAULT_BAUDRATE " bps.");       // Note indent of two spaces

  // This loop runs infinitely, and alters between a transmitting state and an idle state.
  while(1) {
    i = START_BITS+DATA_BITS+STOP_BITS-1;
    s = __disable_interrupt();

      // Signal loop, transmitting state
      do {
        P5OUT = LEDarray[i];
        baudControl();
      } while (i--);
    counter();

    __set_interrupt(s);
    OS_Delay(1);
   }
 }


