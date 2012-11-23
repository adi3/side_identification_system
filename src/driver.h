/**
* @file driver.h
*
* @brief Header file for driver.c
*
* Lists functions and variables available to all who include driver.h
*/

#ifndef __DRIVER_H
#define __DRIVER_H

extern void TaskDriver ( void );

#define STR_TASK_DRIVER     "TaskDriver:\t"

// Only used in splash message. Does not itself alter any values within the program.
#define DEFAULT_BAUDRATE    "2400"             

#define DATA_BITS           8
#define START_BITS          1
#define STOP_BITS           1


#endif /* __DRIVER_H */
