/**
* @file ui.c
*
* @brief Defines UI behavior and control commands 
*
* @note Only one of the two initialized UARTs - UART0 - has been configured here for communication.
*/

#include <salvo.h>                // Req'd because we call e.g. OS_WaitSem() 
#include <usart_uart.h>           // Req'd because we call usart_uart0_getchar()
#include <ctype.h>                // Req'd because we call tolower()

#include "main.h"                 // Application header
#include "ui.h"                   // Good to self-reference
#include "msg.h"                  // Req'd because we call MsgTS()
#include "signal.h"               // Req'd because we call increaseBaud() and decreaseBaud()
#include "counter.h"              // Req'd because we call returnCount()

/**
TaskUI()

@brief User interface task running at 9600 bps

The user interface task handles commands received through the USB connection. 
The available commands are:
 - c: Counter (prints number of signal blocks transmitted)
 - a: Increase Baud Rate (via signal.c; prints new baud rate)
 - z: Decrease Baud Rate (via signal.c; prints new baud rate)
 - 1: Toggles port 2.1 (blocks ASCII signal at port 5.1)
 - 2: Toggles port 2.3 (blocks ASCII signal at port 5.0)
 - 3: Toggles port 2.5 (blocks ASCII signal at port 5.3)
 - 4: Toggles port 2.7 (blocks ASCII signal at port 5.2)
 - v: Version (prints version information)
 - r: Reset (via WDT)
 - h: Help (prints list of available commands)

A Semaphore (SEM_UI_P) is used to implement an event driven architecture. 
The semaphore is signaled in the receiving ISR, ISRRx0().
The user interface task waits for the semaphore.
*/
void TaskUI ( void )
{
  unsigned char cmd;
  // Startup message
  MsgTS(STR_TASK_UI "Starting.");

  // Main loop for TaskUI
  while (1) {
    // Wait for a new command
    OS_WaitSem(SEM_UI_CHAR_P, OSNO_TIMEOUT);
    
    if (cmd=usart_uart1_getchar()) {
      
      // Converts all characters to lower case
      switch (tolower(cmd)) {   

        // Counter
       case 'c':
          sprintf(strTmp, STR_TASK_UI "%c: Counter: %2u signal blocks have been transmitted.",cmd,returnCount());
          MsgTS(strTmp);
          break;

        // Increase baud rate
        case 'a':
          sprintf(strTmp, STR_TASK_UI "%c: Increasing baud rate...",cmd);
          MsgTS(strTmp);
          increaseBaud();
          break;

        // Decrease baud rate
        case 'z':
          sprintf(strTmp, STR_TASK_UI "%c: Decreasing baud rate...",cmd);
          MsgTS(strTmp);
          decreaseBaud();
          break;

        /*=====================================================================
         * The following four commands toggle carrier waves at for P2.x ports. 
         * Since the receiver's filter only allows 38.4 kHz modulated waves to  
         * pass through, the ASCII signals are effectively blocked when their 
         * corresponding P2.x ports no longer deliver a 38.4 kHz square wave.
        =====================================================================*/

        // Toggles port 2.1; Blocks ASCII signal at port 5.1
        case '1':
          P2DIR ^= BIT1;
          sprintf(strTmp, STR_TASK_UI "%c: P2.1: Carrier wave for 'c' toggled",cmd);
          MsgTS(strTmp);
          break;

        // Toggles port 2.3; Blocks ASCII signal at port 5.0
        case '2':
          P2DIR ^= BIT3;
          sprintf(strTmp, STR_TASK_UI "%c: P2.3: Carrier wave for 'x' toggled",cmd);
          MsgTS(strTmp);
          break;

        // Toggles port 2.5; Blocks ASCII signal at port 5.3
        case '3':
          P2DIR ^= BIT5;
          sprintf(strTmp, STR_TASK_UI "%c: P2.5: Carrier wave for 'M' toggled",cmd);
          MsgTS(strTmp);
          break;

        // Toggles port 2.7; Blocks ASCII signal at port 5.2 
        case '4':
          P2DIR ^= BIT7;
          sprintf(strTmp, STR_TASK_UI "%c: P2.7: Carrier wave for 'V' toggled",cmd);
          MsgTS(strTmp);
          break;

        // Version 
        case 'v':
          sprintf(strTmp, STR_TASK_UI "%c: Version: " VERSION_NUM STR_VERSION,cmd);
          MsgTS(strTmp);
          break;
        
        // Reset
        case 'r':
          sprintf(strTmp, STR_TASK_UI "%c: Resetting.",cmd);
          MsgTS(strTmp);
          OS_Delay(100);   // waits one second before initiating restart
          P1OUT |=  BIT7;  // 'close' USB before restarting -- makes restarts much cleaner w/respect to USB
          P1DIR &= ~BIT7;  //   "
          WDTCTL = 0xDEAD; // forced restart
          break;

        // Help 
        case 'h':
          sprintf(strTmp, STR_TASK_UI "%c: Allowed Commands: \r\n c:counter \r\n a:increase baud rate \r\n z:decrease baud rate \r\n 1:toggle signal 'c' \r\n 2:toggle signal 'x' \r\n 3:toggle signal 'M' \r\n 4:toggle signal 'V' \r\n v:version \r\n r:reset \r\n h:help",cmd);
          MsgTS(strTmp);
          break;

        // invalid command
        default:
          MsgTS("Invalid command. Type h for help.");
          break;
      
      }
    }
  }
}

