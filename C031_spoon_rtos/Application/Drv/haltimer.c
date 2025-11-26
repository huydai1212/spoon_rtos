/********************************************************************************
Copyright
Author:
Version: 	V0.0
Date: 		2020/10/16
History:
********************************************************************************/
#include "includes.h"

static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;

/*  */
static SOFT_TMR s_tTmr[TMR_COUNT];

/*
 *
*/
__IO uint64_t g_iRunTime = 0;
uint32_t s_RunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

/*
*********************************************************************************************************
*	
*	
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
  uint8_t i;

  /**/
  for (i = 0; i < TMR_COUNT; i++)
  {
    s_tTmr[i].Count = 0;
    s_tTmr[i].PreLoad = 0;
    s_tTmr[i].Flag = 0;
    s_tTmr[i].Mode = TMR_ONCE_MODE; /**/
  }
}

/*
*********************************************************************************************************
*****************
*/
void bsp_zInitHardTimer(void)
{
	bsp_Tick();
}
/*
*********************************************************************************************************
*****************
*/
void bsp_Tick(void)
{
	  uint8_t i;

	  if (s_uiDelayCount > 0)
	  {
	    if (--s_uiDelayCount == 0)
	    {
	      s_ucTimeOutFlag = 1;
	    }
	  }

	  for (i = 0; i < TMR_COUNT; i++)
	  {
	    bsp_SoftTimerDec(&s_tTmr[i]);
	  }

	  g_iRunTime++;
	  s_RunTime++;
	  if (g_iRunTime == 0x7FFFFFFF) /*  int32_t  0x7FFFFFFF */
	  {
	    g_iRunTime = 0;
	  }

}
/*
*********************************************************************************************************
*	SysTick_ISR
*		
*	
*********************************************************************************************************
*/
#if 0
void HAL_IncTick(void)
{

  uint8_t i;

  if (s_uiDelayCount > 0)
  {
    if (--s_uiDelayCount == 0)
    {
      s_ucTimeOutFlag = 1;
    }
  }

  for (i = 0; i < TMR_COUNT; i++)
  {
    bsp_SoftTimerDec(&s_tTmr[i]);
  }

  g_iRunTime++;
  s_RunTime++;
  if (g_iRunTime == 0x7FFFFFFF) /*  int32_t  0x7FFFFFFF */
  {
    g_iRunTime = 0;
  }
}
#endif
/*
*********************************************************************************************************
*	bsp_SoftTimerDec
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
  if (_tmr->Count > 0)
  {
    if (--_tmr->Count == 0)
    {
      _tmr->Flag = 1;
      if (_tmr->Mode == TMR_AUTO_MODE)
      {
        _tmr->Count = _tmr->PreLoad;
      }

      if (_tmr->Callback != 0)
      {
        _tmr->Callback();
      }
    }
  }
}

/*
*********************************************************************************************************
*bsp_StartTimer
**_period :
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
  if (_id >= TMR_COUNT)
  {
//    while (1);
	  return;
  }

  s_tTmr[_id].Count = (uint16_t)(_period);   /**/
  s_tTmr[_id].PreLoad = (uint16_t)(_period); /**/
  s_tTmr[_id].Flag = 0;                      /**/
  s_tTmr[_id].Mode = TMR_ONCE_MODE;          /**/
}

/*
 ************************************************************************************************
 ********************************
 */
void bsp_StartFreeRunTime(void)
{
  s_RunTime = 0;
}

uint32_t bsp_GetFreeRunTime(void)
{
  return s_RunTime;
}

/*
*********************************************************************************************************
*bsp_StartAutoTimer
*
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
  if (_id >= TMR_COUNT)
  {
//    while (1); /**/
	  return;
  }

  s_tTmr[_id].Count = (uint16_t)(_period);   /**/
  s_tTmr[_id].PreLoad = (uint16_t)(_period); /**/
  s_tTmr[_id].Flag = 0;                      /**/
  s_tTmr[_id].Mode = TMR_AUTO_MODE;          /**/
}

void bsp_StartCallBackTimer(uint8_t _id, pFun _callFun, uint32_t _period)
{
  if (_id >= TMR_COUNT)
  {
    return;
  }
  s_tTmr[_id].Count = (uint16_t)(_period);
  s_tTmr[_id].PreLoad = (uint16_t)(_period);
  s_tTmr[_id].Flag = 0;
  s_tTmr[_id].Mode = TMR_AUTO_MODE;

  s_tTmr[_id].Callback = _callFun;
}

/*
*********************************************************************************************************
*	bsp_StopTimer
*	*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
  if (_id >= TMR_COUNT)
  {
    return;
  }
  s_tTmr[_id].Count = 0;
  s_tTmr[_id].Flag = 0;
  s_tTmr[_id].Mode = TMR_ONCE_MODE;
}

/*
*********************************************************************************************************
*	bsp_CheckTimer
*	
*	_period :
*	
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
  if (_id >= TMR_COUNT)
  {
    return 0;
  }

  if (s_tTmr[_id].Flag == 1)
  {
    s_tTmr[_id].Flag = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

/*********************************************************************************************************
* Function name: bsp_DelayMS
* Function description: ms-level delay, the delay accuracy is plus or minus 1ms
* Formal parameters: n: delay length, unit 1 ms. n should be greater than 2
* Return value: None
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
  if (n == 0)
  {
     return;
  }

  s_uiDelayCount = n;
  s_ucTimeOutFlag = 0;


  while (1)
  {
   /*
		Wait for the delay time to
        Note: The compiler thinks s_ucTimeOutFlag = 0, so it may be an optimization error, so the s_ucTimeOutFlag variable must be declared as volatile
    */
    if (s_ucTimeOutFlag )
    {
            break;
    }
  }
}


/*
*********************************************************************************************************
* Function name: bsp_DelayUS
* Function description: us level delay. This function must be called after the systick timer has started.
* Formal parameters: n: delay length, unit 1 us
* Return value: None
*********************************************************************************************************
*/
void bsp_DelayUS(uint32_t n)
{
    uint32_t ticks;
    uint32_t told;
    uint32_t tnow;
    uint32_t tcnt = 0;
    uint32_t reload;
       
    reload = SysTick->LOAD;                
    ticks = n * (SystemCoreClock / 1000000);	 /* number of beats required */
    
    tcnt = 0;
    told = SysTick->VAL;             /* Counter value when just entering */

    while (1)
    {
        tnow = SysTick->VAL;    
        if (tnow != told)
        {    
            /* SYSTICK is a decrementing counter */
            if (tnow < told)
            {
                tcnt += told - tnow;    
            }
            /* reload decrement */
            else
            {
                tcnt += reload - tnow + told;    
            }        
            told = tnow;

            /* Exit if the time exceeds/equals to the time to be delayed */
            if (tcnt >= ticks)
            {
            	break;
            }
        }  
    }
} 

uint64_t bsp_GetRunTime(void)
{
  return g_iRunTime;
}


