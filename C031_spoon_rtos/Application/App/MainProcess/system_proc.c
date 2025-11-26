#include <ringbuffer.h>
#include <string.h>
#include "cmn.h"
#include "db.h"
#include "system_event.h"
#include "system_proc.h"
#include "sem.h"
#include "fifo8.h"
#include "ringbuffer.h"
#include "halSys.h"
#include "gpio.h"
#include "str.h"
#include "crc.h"
#include "usart.h"
#include "halSys.h"
#include "tim.h"
#include "74hc595pw.h"
static uint16_t u16sysProcNumOfEvent = 0;
/* system init function */
//static void Read_Reset_Reg(void);
static status_t sys_proc_init(system_package *pkg);
static status_t sys_proc_get_data(system_package *pkg);
static status_t sys_proc_return_offset(system_package *pkg);
static status_t sys_proc_push_spoon(system_package *pkg);
static status_t sys_proc_push_chopstick(system_package *pkg);
static status_t sys_proc_add_spoon(system_package *pkg);
static status_t sys_proc_push_spoon_and_chopstick(system_package *pkg);

static sSysProcDic sysProcEventHandlerList[] = {
		{ SYSTEM_INIT, sys_proc_init },
//		{ SYS_PROC_GET_DATA, sys_proc_get_data },
		{ SYS_PROC_RETURN_OFFSET, sys_proc_return_offset },
		{ SYS_PROC_ADD_SPOON, sys_proc_add_spoon },
//		{ SYS_PROC_PUSH_SPOON, sys_proc_push_spoon },
//		{ SYS_PROC_PUSH_CHOPSTICK, sys_proc_push_chopstick },
		{ SYS_PROC_PUSH_SPOON_AND_CHOPSTICK, sys_proc_push_spoon_and_chopstick },

};

/*Local data*/
static FF8 smain_queue;
static system_package smain_queue_events[MAX_SYSTEM_QUEUE];
static sem smain_semaphore;
static status_t sys_proc_init(system_package *pkg)
{
//	Read_Reset_Reg();
//	LREP("Spoon and chopsticks devices\r\n");
	HAL_TIM_Base_Start_IT(&htim14);
	rb_Init();

	__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 50);

    pkg->e = SYS_PROC_RETURN_OFFSET;
    sys_proc_regNewEvent(pkg);

    g_sysDiagnostic.initialDone = 1;

	return RET_OK;
}


status_t sys_proc_main(void)
{
    uint8_t u8JumpIntoWhile;
    system_package pkg;
    /**/
    u16sysProcNumOfEvent = sizeof(sysProcEventHandlerList)/sizeof(sSysProcDic);

    /* init main queue and main semaphore */
    sys_proc_resetEvent();

    pkg.e = SYSTEM_INIT;
    sys_proc_regNewEvent(&pkg);
    /*main while*/
    while (1)
    {
        u8JumpIntoWhile = 0;

        while (sys_proc_countEvent() > 0 /*|| semaphore_pend(&smain_semaphore, TIMEOUT_SYS_PROC) == RET_OK*/)
        {
            static system_package pkg;

            u8JumpIntoWhile = 1;

            /*Pre-processing data*/
            if(sys_proc_preProcessing(&pkg) != RET_OK)
            {
                continue;
            }

            /*Processing data*/
            sys_proc_mainProcessing(&pkg);

            /*Pos-processing data*/
            sys_proc_afterProcessing();
        }

        if(u8JumpIntoWhile == 0)
        {
            /*Pos- processing data*/
            sys_proc_afterProcessing();

//            if(g_positionData.u8IsGPSLocationValid)
//            {
//                LREP(gps_getLastCoordString());
//            }
//            else
//            {
//                LREP(".\r\n");
//            }
        }
    }
    return RET_OK;
}

/*******************************************************************************
**                           FUNCTION DEFINITIONS
*******************************************************************************/
/************************************************************************
 * Method   : sys_proc_regNewEvent
 * FullName : sys_proc_regNewEvent
 * Access   : public
 * Note     : Reg new event to main queue
 * @param   : pkg
 * @return  : status_t
 ************************************************************************/
status_t sys_proc_regNewEvent(system_package *pkg)
{
    /*Check and add event to queue*/
    /*Lock this function*/
    if (!FF8_IsFull(&smain_queue))
    {
        if (FF8_Push(&smain_queue, *pkg) == TRUE)
        {
            /*Post Semaphore*/
            //LREP("P[%d]\r\n", pkg->e);
            semaphore_post(&smain_semaphore);
            return RET_OK;
        }
        else
        {
            //LREP("N-P[%d]\r\n", pkg->e);
        }
    }
    else
    {
        //LREP("MQ full!\r\n");
    }

    return RET_FAIL;
}

/************************************************************************
 * Method   : sys_proc_countEvent
 * FullName : sys_proc_countEvent
 * Access   : private
 * Note     : count
 * @ID     : ID1.1
 * @param   :
 * @return  : status_t
 ************************************************************************/
uint16_t sys_proc_countEvent(void)
{
    return FF8_GetCount(&smain_queue);
}

/************************************************************************
 * Method   : sys_proc_preProcessing
 * FullName : sys_proc_preProcessing
 * Access   : private
 * Note     : Pre- processing. Check and init
 * @ID     : ID1.1
 * @param   :
 * @return  : status_t
 ************************************************************************/
status_t sys_proc_preProcessing(system_package * pkg)
{
    sys_data_analysis();
    if(sys_proc_countEvent() == 0)
        return RET_FAIL;
    else
        semaphore_reset(&smain_semaphore);

    if (FF8_Pop(&smain_queue, pkg) != TRUE)
    {
//        LREP("Cannot pop data from smain_queue!\r\n");
        return RET_FAIL;
    }

//    LREP("\r\nMQ[%d][%d]\r\n", pkg->e, sys_proc_countEvent());

    return RET_OK;
}

/************************************************************************
 * Method   : sys_proc_mainProcessing
 * FullName : sys_proc_mainProcessing
 * Access   : private
 * Note     :
 * @ID      : ID1.2
 * @param   : pkg
 * @return  : status_t
 ************************************************************************/
status_t sys_proc_mainProcessing(system_package *pkg)
{
    ASSERT(pkg);
    /*Dispatch and process attached data*/
    int i = 0;
    for (; i < u16sysProcNumOfEvent; i++)
    {
        if (sysProcEventHandlerList[i].e == pkg->e && sysProcEventHandlerList[i].eventHandler != NULL)
        {
            return (*sysProcEventHandlerList[i].eventHandler)(pkg);
        }
    }

    if(i >= u16sysProcNumOfEvent)
    {
//        LREP("Can not find out handler function %02x!\r\n",pkg->e);
    }

    return RET_OK;
}
/************************************************************************
 * Method   : sys_proc_resetEvent
 * FullName : sys_proc_resetEvent
 * Access   : private
 * Note     :
 * @ID      :
 * @param   :
 * @return  : status_t
 ************************************************************************/
status_t sys_proc_resetEvent(void)
{
    /*Init queue*/
    FF8_Reset(&smain_queue, smain_queue_events, MAX_SYSTEM_QUEUE);

    /*Init sem*/
    semaphore_init(&smain_semaphore);
    return RET_OK;
}
/************************************************************************
 * Method   : sys_proc_afterProcessing
 * FullName : sys_proc_afterProcessing
 * Access   : private
 * Note     : process after main process
 * @ID      : ID1.3
 * @param   :
 * @return  : status_t
 ************************************************************************/
status_t sys_proc_afterProcessing(void)
{
    sys_data_analysis();

    if(g_sysDiagnostic.enableTest == 1){
    	proc_runTest();
    }
    return RET_OK;
}
/************************************************************************
 * Method   : sys_proc_afterProcessing
 * FullName : sys_proc_afterProcessing
 * Access   : private
 * Note     : process after main process
 * @ID      : ID1.3
 * @param   :
 * @return  : status_t
 ************************************************************************/

static status_t sys_proc_get_data(system_package *pkg){
	return RET_OK;
}
/*
 *
 * */
static status_t sys_proc_return_offset(system_package *pkg){
	beamMoveToOffset(10000);
	chopstickMoveToOffset(10000);
	spoonMoveToOffset(100000);
	return RET_OK;
}
/*
 *
 * */
static status_t sys_proc_push_spoon(system_package *pkg){
	proc_push_spoon();
	return RET_OK;
};
/*
 *
 * */
static status_t sys_proc_push_chopstick(system_package *pkg){
	proc_push_chopsticks();

	pkg->e = SYS_PROC_PUSH_SPOON;
	sys_proc_regNewEvent(pkg);
	return RET_OK;
};
/*
 *
 * */
static status_t sys_proc_add_spoon(system_package *pkg){
	proc_add_spoon();
	return RET_OK;
};
/*
 *
 * */
static status_t sys_proc_push_spoon_and_chopstick(system_package *pkg){
	proc_push_chopsticks();
	proc_push_spoon();
	g_sysDiagnostic.processStart = 0;
	return RET_OK;
}
/*
 *
 * */

/*static void Read_Reset_Reg(void) {
	if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST)) {
		LREP("-> SOW RST\r\n")
		;
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PWRRST)) {
		LREP("-> POR/PDR RST\r\n")
		;
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST)) {
		LREP("-> IWDG RST\r\n")
		;
	} else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST)) {
		LREP("-> PIN RST\r\n")
		;
	} else {
		LREP("-> ?RST\r\n")
		;
	}
	 The flags must be cleared manually after use
	__HAL_RCC_CLEAR_RESET_FLAGS();
}*/

