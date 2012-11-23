/**
* @file init.c
*
* @brief Initializes the hardware on processor startup.
*
* Initializes the general hardware, including the clock, timer(s) and the serial communications interfaces.
* UART is configured for 9600 bps, 8 data bits, no parity bit, one stop bit.
*
* @note Both UARTs (uart0 or uart1) have been initialized for communication.
* @note WDT_OFF and TIMERA0_RELOAD are defined in main.h
* @note Other clock definitions contained in config.h
*/

#include <msp430.h>               // Contains MSP430-specific definitions
#include <usart_uart.h>           // Req'd because we call usart_uart1_open() AND usart_uart0_open()

#include "init.h"                 // Good to self-reference
#include "main.h"                 // Application header

void Init(void) {
  unsigned int i;

  WDT_OFF;                                           // Stop WDT for initialization. This app does not use the WDT.
  __disable_interrupt();                             // Disable all interrupts during critical period of initialization.

  P1OUT = 0xFF;
  P2OUT = 0xFF;
  P3OUT = 0xFF;
  P4OUT = 0xFF;
  P5OUT = 0xFF;
  P6OUT = 0xFF;

  // Configure CSK DB or MB for COM1 out USB ...
  P6OUT |=  BIT6;                                    // U2 and U3 deselected
  P6DIR |=  BIT6;                                    // "
  P6OUT |=  BIT5+BIT4+BIT3;                          // Disable -CTS_USB, -RI_USB, -RST_USB
  P6DIR |=  BIT5+BIT4+BIT3;                          // "
  P1OUT &= ~BIT7;                                    // Enable U1
  P1DIR |=  BIT7;                                    // "

  // Configure ports for signal
  P5OUT = 0x00;                                      //Port 5 transmits signals for ASCII characters
  P5DIR = 0xFF;                                      // "
  P2DIR |= BIT1+BIT3+BIT5+BIT7;                      //Port 6 transmits 38.5 kHz carrier waves
  P2OUT |= BIT1+BIT3+BIT5+BIT7;                      // "

  // Clock initialization. Requires only 32.768kHz LFXTAL
  DCOCTL=0x60;                                       //DCO.1,DCO.0
  BCSCTL2=0x00;
  BCSCTL1=0x84;                                      //XT2OFF,RSEL.2
  
  BCSCTL1 &= ~XT2OFF;                                // Turn on XT2
  do {                                               // Wait for XT2 to stablize                                                    
    IFG1 &= ~OFIFG;                                  // Clear OSCFault flag
    for (i = 0xFFF; i > 0; i--);                     // Time for flag to set
  } while((IFG1 & OFIFG) == OFIFG);                  // Check if OSCFault flag is still set   

  BCSCTL2 |= SELM1+SELS;                             // Run at 7.3728 MHz (MCLK and SMCLK)

  //Timer A setup
  TACTL  = TASSEL0 + TACLR;                          // Stop Timer, SMCLK. Clear TAR
  CCTL0  = CCIE;                                     // CCR0 interrupt enabled
  CCR0   = TIMERA0_RELOAD;                           // Initial value
  TACTL |= MC1;                                      // Starting Timer_A in continuous mode

  usart_uart1_open(USART_UART_9600_N81_SMCLK);        // Initialize UART1 at 9600,N,8,1
  usart_uart0_open(USART_UART_9600_N81_SMCLK);        // Initialize UART0 at 9600,N,8,1

  __enable_interrupt();                             // Enable all interrupts
}
