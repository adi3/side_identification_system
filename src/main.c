/**
* @file main.c
*
* @brief Contains the main function run at startup
*
* @note This app has only two tasks: One to trasnmit signals, and the other for UI operation.
*
* @author Adi
*/

#include <salvo.h>                // Req'd to access Salvo functions 
#include <usart_uart.h>           // Req'd because we call usart_uart0_puts()
#include <__cross_studio_io.h>    // Req'd because we call debug_exit()

#include "main.h"                 // Application header
#include "init.h"                 // Req'd because we call Init()
#include "msg.h"                  // Req'd because we call MsgTS()
#include "driver.h"               // Req'd because we reference TaskDriver()
#include "ui.h"                   // Req'd because we reference TaskUI()
#include "periodic.h"             // Req'd because we reference TaskPeriodic()

// General-purpose buffer for creating long strings
char strTmp[256];

/**
main()

Hardware initialized through Init() function.
RTOS initialized through OSinit() function.
Tasks and events created.
Salvos scheduler run in an infinite loop.
*/
void main(void) {

  // Initialize MSP430 built-in peripherals and pin configuration.
  Init(); 

  // Hardware initialized; Splash message via USB displayed
  usart_uart0_puts("\r\n");                         // Line break to separate splash messages of successive calls
  MsgTS("main: SSDL Team 5 LSM Project");
  MsgTS("  v" VERSION_NUM STR_VERSION);             // Note indent of two spaces

  // Initializes RTOS.
  OSInit(); 

  /*
  Creates all tasks.
  */
  OSCreateTask(TaskDriver,       OSTCBP(2),  2);
  OSCreateTask(TaskUI,       OSTCBP(3),  4);
  OSCreateTask(TaskPeriodic,       OSTCBP(4),  4);

  /*
  Creating all events.
  */
  OSCreateSem(SEM_UI_CHAR_P, 0);
  
  // Since ISRs are present, interrupts need to enabled globally.
  __enable_interrupt();

  // Run the scheduler in an infinite loop.
  while(1) {
    OSSched();
  }

  // This point is never reached if the app has no bugs.
  debug_exit(0);
}


