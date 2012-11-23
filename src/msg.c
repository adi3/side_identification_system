/**
* @file msg.c
*
* @brief Defines MsgTS() to prints a tagged message for UI purposes
*
* @note Only one of the two initialized UARTs - UART1 - has been configured here for communication.
*/

#include <salvo.h>                // Req'd because we call OSGetTicks()
#include <usart_uart.h>           // Req'd because we call usart_uart1_puts()

#include "config.h"               // Req'd because we use TX1_BUFF_SIZE
#include "msg.h"                  // Good to self-reference
#include "main.h"                 // Application header

/**
MsgTS()

@brief Prints a tagged message to the USB connection

Prints a timestamped message through the USB connection.
Message format is as follows: 
          1:hh:mm:ss.tt TaskIdentifier: Message
  Where:
  hh:mm:ss.tt is the timestamp.
  TaskIdentifier is the unique identifier for the task calling MsgTS().
  Message is the string programmed to be printed.

@param  cP is a pointer to the character array that contains the string to display. 
Includes Task Identifier and Message.
*/
void MsgTS(const char * cP)
{
  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  
  OStypeTick a = 0;
  unsigned long t_ms = 0, t_s = 0;
  unsigned int t_m = 0, t_h = 0;
  unsigned short nb_ms = 0, nb_s = 0, nb_m = 0, nb_h = 0;
  
  // Format strTicks to indicate which USART is talking, and display a timestamp.
  a = OSGetTicks();
  t_ms = a*10;                  // The time since the timer started in millisecondes
        
  nb_ms = t_ms-1000*(t_ms/1000);
  t_s = (t_ms-nb_ms)/1000;
  nb_s = t_s-60*(t_s/60);
  t_m = (t_s-nb_s)/60;
  nb_m = t_m-60*(t_m/60);
  t_h = (t_m-nb_m)/60;
  nb_h = t_h-60*(t_h/60);
    
  sprintf(strTicks, "1:%02u:%02u:%02u.%03u ", nb_h, nb_m, nb_s, nb_ms);

  // Computes size of string.
  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  // If the string theoretically fits AND if there is room in the buffer, the operation is carried out.
  if (size < TX1_BUFF_SIZE) {
    while (usart_uart1_tx_free() < size);
    usart_uart1_puts(strTicks);
    usart_uart1_puts(cP);
    usart_uart1_puts(CRLF);
  } 
}


