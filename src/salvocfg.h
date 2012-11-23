/**
* @file salvocfg.h
*
* @brief RTOS configuration file
*/
#define OSUSE_LIBRARY      TRUE
#define OSLIBRARY_TYPE     OSL
#define OSLIBRARY_CONFIG   OST

#define OSTASKS            4        // >= num(tasks) in application
#define OSEVENTS           1        // >=1 to avoid annoyances
#define OSEVENT_FLAGS      0
#define OSMESSAGE_QUEUES   0
