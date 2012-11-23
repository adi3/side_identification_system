/**
* @file periodic.c
*
* @brief Confirms program operation
*
* Emits a message saying 'Transmitting' every two seconds to confirm that the program is running.
* As long as the message keeps on being displayed, we can be sure that the program has not 
* crashed, and that the transmitter end is sending out signals.
*
* @note It is assumed that if this task is running right, then all other tasks are running correctly too.
*/

#include <msp430.h>               // Req'd because we refer to P51OUT
#include <salvo.h>                // Req'd because we call OSDelay() 

#include "periodic.h"             // Good to self-reference
#include "main.h"                 // Application header
#include "msg.h"                  // Req'd because we call MsgTS()

void TaskPeriodic ( void )     
{
  // Startup message
  MsgTS(STR_TASK_PERIODIC "Starting.");

   // Begin infinite loop that displays a message periodically.
    while (1) {
      MsgTS(STR_TASK_PERIODIC "Transmitting");
      P1OUT ^= BIT0;
      OS_Delay(200);                            //waits 2 sec before displaying next message
    }
}
