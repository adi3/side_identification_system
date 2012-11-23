/**
* @file main.h
*
* @brief Header file for main.c
*
* Lists functions and variables available to all who include main.h
* Used application wide.
*
* @author Adi
*/

#ifndef __MAIN_H
#define __MAIN_H

extern char strTmp[];

// Application wide constants
// Version information
#define VERSION_NUM                     "3.0"
#define STR_VERSION                     " built on " __DATE__ " at "  __TIME__"."

// TimerA count value
#define TIMERA0_RELOAD                  328

// No operation
#define NOP		                _NOP()

// Control of Watchdog timer
#define WDT_OFF				WDTCTL = WDTPW + WDTHOLD
#define WDT_ON				WDTCTL = WDT_ARST_250

// Task/Semaphore related constants 
#define SEM_UI_CHAR_P OSECBP(1)

#endif /* __MAIN_H */

