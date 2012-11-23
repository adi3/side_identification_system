/**
* @file isr.c
*
* @brief Defines Interrupt service routines
*
* ISR defined for TimerA. It calls OSTimer().
* ISR defined for UART0 and UART1 for both input and output characters.
*
* @note Either one of the two UARTs (uart0 or uart1) could be used for communication.
*/

#include <salvo.h>                // Req'd because we call OSTimer() 
#include <usart_uart.h>           // Req'd because we call usart_uart0|1_in|outchar()

#include "isr.h"                  // Good to self-reference
#include "main.h"                 // Application header

/**
Timer_A()

@brief Timer A Interrupt Service Routine

This ISR is activated when Timer A reaches is defined value. 
The timer is reset and the Salvos function OSTimer() is called.
The timer is set to run at about 100Hz.
*/
void Timer_A (void) __interrupt[TIMERA0_VECTOR] {
  CCR0 += TIMERA0_RELOAD;
  OSTimer(); 
}                    


/**
ISRTx0()

@brief UART0 Transmitting Interrupt Service Routine

Pumpkin usart libraries are used to transmit characters in the buffer to the serial interface.
*/
void ISRTx0 (void) __interrupt[UART0TX_VECTOR] {
  usart_uart0_outchar();
}


/**
ISRRx0()

@brief UART1 Receiving Interrupt Service Routine

Pumpkin usart libraries are used to put characters received at the serial interface into a buffer.
The semaphore SEM_UI_CHAR_P is signaled. 
TaskUI() is waiting for that semaphore and processes the command in the buffer.
*/
void ISRRx0 (void) __interrupt[UART0RX_VECTOR] {
  usart_uart0_inchar(RXBUF0);
  OSSignalSem(SEM_UI_CHAR_P); 
}


/**
ISRTx1()

@brief  UART1 Transmitting Interrupt Service Routin

Pumpkin usart libraries are used to transmit characters in the buffer to the serial interface.
*/
void ISRTx1 (void) __interrupt[UART1TX_VECTOR] {
  usart_uart1_outchar();
} 


/**
ISRRx1()

@brief UART1 Receiving Interrupt Service Routine

Pumpkin usart libraries are used to put characters received at the serial interface into a buffer.
The semaphore SEM_UI_CHAR_P is signaled. 
TaskUI() is waiting for that semaphore and processes the command in the buffer.
*/
void ISRRx1 (void) __interrupt[UART1RX_VECTOR] {
  usart_uart1_inchar(RXBUF1);
  OSSignalSem(SEM_UI_CHAR_P); 
}


