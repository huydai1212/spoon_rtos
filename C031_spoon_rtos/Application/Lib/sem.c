/*******************************************************************************
 * @file   : sem.c
 * @date   : 2018/08/02 15:26
 * @author : LieuND
 * @brief  : 
 * TODO    : 
 * @note   :
 *******************************************************************************/
#include "sem.h"
#include "system_proc.h"

/*API*/
/************************************************************************
 * Method   : semaphore_init
 * FullName : semaphore_init
 * Access   : public 
 * Note     : intialize semaphore
 * @param   : s
 * @return  : status_t
 ************************************************************************/
status_t semaphore_init(sem *s)
{
    ASSERT(s);

    s->count = 0;

    return RET_OK;
}

/************************************************************************
 * Method   : semaphore_post
 * FullName : semaphore_post
 * Access   : public 
 * Note     : post signal to semaphore
 * @param   : s
 * @return  : status_t
 ************************************************************************/
status_t semaphore_post(sem *s)
{
    ASSERT(s);
    if (s->count < FOREVER_WAIT)
    {
        s->count += 1;
        return RET_OK;
    }
    else
    {
        //LREP("~");
    }
    
    return RET_FAIL;
}

/************************************************************************
 * Method   : semaphore_pend
 * FullName : semaphore_pend
 * Access   : public 
 * Note     : pending a signal from the semaphore
 * @param   : s
 * @param   : timeout
 * @return  : status_t
 ************************************************************************/
status_t semaphore_pend(sem *s, uint32_t timeout)
{
    ASSERT(s);

    volatile uint32_t t = 0;

    while (t < timeout && s->count == 0)
    {
        sys_data_analysis();
        
        HAL_Delay(SEM_STEP_TIME);
        t += SEM_STEP_TIME;
    }

    if (t >= timeout)
    {
        return RET_FAIL;
    }
    else
    {
        if (s->count > 0)
        {
            s->count--;
        }
        return RET_OK;
    }
}

/************************************************************************
 * Method   : semaphore_reset
 * FullName : semaphore_reset
 * Access   : public 
 * Note     : reset semaphore
 * @param   : s
 * @return  : status_t
 ************************************************************************/
status_t semaphore_reset(sem *s)
{
    ASSERT(s);
    s->count = 0;
	
	return RET_OK;
}

/*EOF*/
