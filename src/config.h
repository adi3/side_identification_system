/**
* @file config.h
*
* @brief Hardware configuration file
*/

#ifndef __CONFIG_H
#define __CONFIG_H

#include <msp430.h>

// Clocks
#define XT2CLK                            7372800L    // 7.3728MHz XT2 xtal (if fitted)
#define DCOCLK                            4975000L    // Actual value of non-FLL DCO (measured)
#define LFXT1CLK                          32768       // 32.768kHz LFXT1 xtal

#define ACLK                              LFXT1CLK    // ACLK = LFXT1CLK
#define MCLK                              XT2CLK      // MCLK = DCOCLK
#define SMCLK                             XT2CLK      // SMCLK = DCOCLK

// putchar() and getchar() configurability
#define GETCHAR_PUTCHAR_RETURN_ZERO       1           // For this project, these functions return 0
                                                      //  on error

// size of buffers in use
#define RX0_BUFF_SIZE                     256         // Not used
#define TX0_BUFF_SIZE                     256         // Not used
#define RX1_BUFF_SIZE                     256         // 
#define TX1_BUFF_SIZE                     256         // 
#define RX4_BUFF_SIZE                     0           // Not used
#define TX4_BUFF_SIZE                     0           // Not used


#endif /* __CONFIG_H */

