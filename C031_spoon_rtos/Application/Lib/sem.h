/*******************************************************************************
 * @file   : sem.h
 * @date   : 2018/08/02 15:25
 * @author : LieuND
 * @brief  : 
 * TODO    : 
 * @note   :
 *******************************************************************************/
#ifndef sem_h__
#define sem_h__

#include "cmn.h"

/*define*/
#define SEM_STEP_TIME (50) /* 50 ms*/
#define FOREVER_WAIT  (0xFFFFFFFF)

/*struct*/
typedef struct semaphore_ {
    uint32_t count;
}sem;

/*api*/
status_t semaphore_init(sem *s);
status_t semaphore_post(sem *s);
status_t semaphore_pend(sem *s, uint32_t timeout);
status_t semaphore_reset(sem *s);
#endif // sem_h__
