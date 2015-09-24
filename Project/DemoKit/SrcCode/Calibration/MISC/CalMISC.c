/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalMISC.c
    @ingroup    mIPRJAPTest

    @brief      Miscellaneous function


    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include <string.h>
#include "Nand.h"
#include "PrimaryTsk.h"
#include "SysCfg.h"
#include "Utility.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"


#if (_CALIBRATION_MODE_ == ENABLE)

static INT8 MiscStringBuf[64];
static RECT MiscStringRect = { 64, 192, 180, 24 };
extern BOOL ClearCalMain(void);
/**
  Verify Nand bad block

  Verify Nand bad block

  @param void
  @return void
*/
void Cal_CheckNand(void)
{
    Primary_ChangeStorage(PRIMARY_STGTYPE_NAND);
    sprintf((char *)MiscStringBuf,"Please Wait 1 min");
    Cal_ShowString((INT8 *)MiscStringBuf, &MiscStringRect, TRUE);
    TimerDelayMs(1000);
    /**
    nand_CheckBadBlock();
    **/
    sprintf((char *)MiscStringBuf,"Check Finish");
    Cal_ShowString((INT8 *)MiscStringBuf, &MiscStringRect, TRUE);
    while(1)
     ;;

}
void Cal_DeleteCal(void)
{
    sprintf((char *)MiscStringBuf,"Please Wait 30sec ");
    Cal_ShowString((INT8 *)MiscStringBuf, &MiscStringRect, TRUE);
    /**
    ClearCalMain();
    **/
    TimerDelayMs(1000);
    sprintf((char *)MiscStringBuf,"Delete Finish");
    Cal_ShowString((INT8 *)MiscStringBuf, &MiscStringRect, TRUE);
    while(1)
     ;;

}



#endif
//@}

