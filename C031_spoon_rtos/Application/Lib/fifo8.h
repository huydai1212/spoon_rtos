/*******************************************************************************
 * @file   : fifo8.h
 * @date   : 2018/08/02 15:25
 * @author : LieuND
 * @brief  : 
 * TODO    : 
 * @note   :
 *******************************************************************************/
#ifndef _FF8_H_
#define _FF8_H_

#include "cmn.h"
#include "system_event.h"

#define FF_TYPE   system_package

typedef struct _FF8 {
  FF_TYPE   *pArrBuff;
  int   nMaxSize;
  int   nSize;
  int   nHead;
  int   nTail;
  // Protection Mode (usefully for packet handling)
  BOOL  bEnProtect;
  int   nProtectPtr;
} FF8;

 void   FF8_AsignElement  (FF_TYPE *pEFrom, FF_TYPE *pETo);
 void   FF8_Reset         (FF8 *pFF, FF_TYPE *pBuff, int nSize);
 BOOL   FF8_Push          (FF8 *pFF, FF_TYPE b);
 BOOL   FF8_Pop           (FF8 *pFF, FF_TYPE *b);
 void   FF8_EnableProtect (FF8 *pFF);
 void   FF8_DisableProtect(FF8 *pFF);
 void   FF8_RewindHead    (FF8 *pFF);
 BOOL   FF8_IsEnablePush  (FF8 *pFF);
 BOOL   FF8_IsEnablePop   (FF8 *pFF);
 BOOL   FF8_IsEmpty       (FF8 *pFF);
 BOOL   FF8_IsFull        (FF8 *pFF);
 int    FF8_GetCount      (FF8 *pFF);
 int    FF8_GetAvaiLen    (FF8 *pFF);
 int    FF8_GetMaxSize    (FF8 *pFF);
 BOOL   FF8_PopArray      (FF8 *pFF, FF_TYPE *b, int nSize);
 BOOL   FF8_GetData		  (FF8 *pFF, FF_TYPE *b, int nSize);

#endif  //_FF8_H_
