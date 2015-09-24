/**
    Sensor calibration item

    @file       CalSensor.c
    @ingroup    calibration
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Utility.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
#include "CalSensor.h"
//#include "Phototsk.h"

void Cal_DefectPixel(void)
{
    Cal_RunItem(CAL_ITEM_DEFECTPIXEL);
}

void Cal_ISO(void)
{
    Cal_RunItem(CAL_ITEM_ISO);
}

void Cal_Flash(void)
{
    Cal_RunItem(CAL_ITEM_FLASH);
}
void Cal_FlashAWB(void)
{
    Cal_RunItem(CAL_ITEM_FLASH_AWB);
}
void Cal_FlashRedeyeTest(void)
{
    Cal_RunItem(CAL_ITEM_FLASH_REDEYE_TEST);
}

void Cal_FlashGnoShots(void)
{
    Cal_RunItem(CAL_ITEM_FLASH_GNO_SHOTS);
}

void Cal_MShutter(void)
{
    Cal_RunItem(CAL_ITEM_MSHUTTER);
}

void Cal_MShutter_sIris(void)
{
    Cal_RunItem(CAL_ITEM_MSHUTTER_sIris);
}

void Cal_AWB(void)
{
    Cal_RunItem(CAL_ITEM_AWB);
}

void Cal_WriteAWBGS(void)
{
    Cal_RunItem(CAL_ITEM_SAVEAWBGS);
}

void Cal_ECS(void)
{
    Cal_RunItem(CAL_ITEM_ECS);
}

void Cal_Write2File(void)
{
    UINT32  result;
    Cal_RunItem(CAL_ITEM_WR2FILE);
    Cal_SetDefault();
    TimerDelayMs(500);
    result = Cal_RunItem(CAL_ITEM_READSTATUS);

    if(!result)
    {
        while(1) ;
    }
}

void Cal_ReadCalStatus(void)
{
    Cal_RunItem(CAL_ITEM_READSTATUS);
}

void Cal_VIG(void)
{
    Cal_RunItem(CAL_ITEM_VIG);
}

void Cal_SenClearStatus(void)
{
    Cal_RunItem(CAL_ITEM_ClearStatus);
}

// EOF
