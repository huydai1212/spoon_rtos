/*******************************************************************************
 * @file   : system_event.h
 * @date   :
 * @author :
 * @brief  : 
 * TODO    : 
 * @note   :
 *******************************************************************************/
#ifndef system_event_h__
#define system_event_h__

#include "cmn.h"

/*enum*/
typedef enum{
    
    SYSTEM_INIT = 0,
    SYSTEM_GET_TIME,
    TRANS_DEVICE_TEST,
	CMD_TEST_LED,
	CMD_RESET_DEVICE,
	CMD_WRITE_DATA_TO_FLASH,

	SYS_PROC_RETURN_OFFSET,

	SYS_PROC_ADD_SPOON,
	SYS_PROC_PUSH_SPOON,
	SYS_PROC_PUSH_CHOPSTICK,
	SYS_PROC_PUSH_SPOON_AND_CHOPSTICK,

	SYS_PROC_GET_DATA,

}system_event_t;

/*struct*/
typedef struct system_package_{
    system_event_t e;
    uint32_t u32Data;
} system_package;

#endif // system_event_h__
