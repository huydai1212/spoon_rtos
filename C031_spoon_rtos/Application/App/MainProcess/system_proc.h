#ifndef system_proc_h__
#define system_proc_h__


#include "cmn.h"
#include "db.h"
#include "system_defines.h"
#include "system_event.h"

/*define*/
/*******************************************************************************
**                               FUNCTION POINTER
*******************************************************************************/
typedef status_t(*psysProcHandler)(system_package *pkg);

/*******************************************************************************
**                               DATA STRUCT DEFINE
*******************************************************************************/
typedef struct sysDic_ {
    system_event_t  e;
    psysProcHandler eventHandler;
} sSysProcDic;


/*API*/

status_t sys_proc_regNewEvent(system_package *pkg);
uint16_t sys_proc_countEvent(void);
status_t sys_proc_resetEvent(void);

status_t sys_proc_main(void);
status_t sys_proc_preProcessing(system_package * pkg);
status_t sys_proc_mainProcessing(system_package *pkg);
status_t sys_proc_afterProcessing(void);

void sys_data_analysis(void);
void sys_data_analysis_withOutAtRespond(void);
status_t sys_dataSend(char* headerStr, uint16_t cmd, uint16_t pcbId, char* dataSend);


#endif // system_proc_h__
