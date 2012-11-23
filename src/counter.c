/**
* @file counter.c
*
* @brief Keeps track of number of signal blocks transmitted. 
*
* @note One signal block is defined here as the entire 10-element LEDarray given in driver.c
*/

#include "counter.h"              // Good to self-reference
#include "main.h"                 // Application header

// Stores the number of signal blocks transmitted.
unsigned int count = 0; 

/**
counter()

Increases variable 'count' by one.
Called in by TaskDriver() everytime a signal block transmission is completed.
*/
void counter (void)
{
    count++; 
}


/**
returnCount()

Returns variable 'count'.
Called in by TaskUI().

@return count is an integer representing the number of signal blocks transmitted.
*/
unsigned int returnCount (void)
{
  return count;
}
