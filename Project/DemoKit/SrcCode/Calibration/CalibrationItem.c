/**
    Calibration item

    @file       CalibrationItem.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "CalibrationInt.h"
#include "Alg_IPLMode.h"
#include "debug.h"
#include "appinitphoto.h"//ethan_20110805_cleanwarn
#include "devctrlphoto.h"//ethan_20110805_cleanwarn
UINT32 Cal_RunItem(CAL_ITEM AlgCal_item)
{
    //UINT32 uiTmp;
    UINT32 ErrorCode = CAL_ERR_OK;
    //IPL_OBJ IplObj;

    //fnCalShowString = pParam->FnShowString;
    //gCapRawAddr = pParam->MemoryAddr;
    //uiTmp = pParam->MemorySize;
    //debug_err(("gCapRawAddr: 0x%x MemorySize:0x%x \r\n", gCapRawAddr, uiTmp));

//    CFG_CalInfor();
    //ReadCalData();

    //check cur ipl status,
/*
    if(IPL_GetMode() != IPL_MODE_OFF)
    {
        debug_err(("%s: chg ipl to off(%d)\r\n",IPL_GetMode()));
        IplObj.uiCmd = IPL_CMD_CHG_MODE;
        IplObj.Mode = IPL_MODE_OFF;
        IPL_SetCmd(&IplObj);
    }
*/
    AppInit_ModePhoto();
    DevCtrl_ModePhoto();


    switch(AlgCal_item)
    {
        case CAL_ITEM_ISO:
            ErrorCode = ISO_Adjust();
            break;

        case CAL_ITEM_MSHUTTER:
            ErrorCode = Mshutter_Adjust();
            break;

        case CAL_ITEM_DEFECTPIXEL:
            ErrorCode = DP_Adjust();
            break;

        case CAL_ITEM_AWB:
            ErrorCode = AWB_Adjust();
            break;

        case CAL_ITEM_ECS:
            ErrorCode = ECS_Adjust();
            break;

        case CAL_ITEM_READSTATUS:
            ErrorCode = Cal_ShowCalRst();
            break;

        case CAL_ITEM_SAVEAWBGS:
            ErrorCode = Cal_SaveAWBGS();
            break;
        default:
            ErrorCode = CAL_ERR_NULL;
            debug_ind(("No this item\r\n"));
            break;
    }
    return ErrorCode;
}
//@}
