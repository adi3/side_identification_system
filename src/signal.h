/**
* @file signal.h
*
* @brief Header file for signal.c
*
* Lists functions and variables available to all who include signal.h
*/

#ifndef __SIGNAL_H
#define __SIGNAL_H

extern void increaseBaud( void );
extern void decreaseBaud( void );
extern void baudControl( void );
extern void modDelay( void );

#define STR_BAUD_CONTROL      "BaudControl:\t"

// Empirically determined number of NOP cycles required for a 38.4 kHz frequency
#define MOD_CYCLES             5

#endif /* __SIGNAL_H */
