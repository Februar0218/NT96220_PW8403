#include    <string.h>
#include    <stdlib.h>
#include    <stdio.h>
#include    "kernel.h"
#include    "sensor.h"
#include    "debug.h"
#include    "utility.h"
#include    "Calibration.h"
#include    "CalibrationInt.h"
#include    "Cal_MI5130_220.h"
#include    "Syscfg.h"
#include    "Cache.h"
#include    "KeyScanTsk.h"
#include    "UIResource.h"
#include    "CalibrationTsk.h"
#include    "phototsk.h"
#if (SENSOR_LSC_ENABLE==ENABLE)
#include    "liblcmobile.h"
#endif
#include    "filesystsk.h"


ULONG gLCSRegIdx = 0;
ULONG gLCSRegData[256];
INT8 CalStringBuffer[32];

/**
  AAA_Adjust structure is flexible by project,
  so its prototype is in different folder instead
  in common file
*/
volatile Adjust_Struct  AAA_Adjust;

#define ID_LCS_CMD_INIT 0
#define ID_LCS_CMD_ADD  1
#define ID_LCS_CMD_END  2
#define ID_LCS_CMD_AE   3

static ER CheckAwbGS(void)
{
    ER ercode = 0;

    return ercode;
}

#if 0
#endif
void Cal_PassAdjustData(void)
{

}

//ethan_20110801_chg to read only specific section
ER Cal_ReadData(UINT32 uisection)
{

    return E_OK;
}

//ethan_20110801_readAllCalibrationData,for Systeminit only
ER Cal_ReadCalData(void)
{
    return E_OK;
}

ER Cal_WriteCalData(UINT32 uiSection)
{
    return  E_OK;
}
ER Cal_DeleteCalData(void)
{
    return E_OK;
}

UINT32 Mshutter_Adjust(void)
{
    return CAL_ERR_OK;
}

UINT32 AWB_Adjust(void)
{
    return CAL_ERR_OK;
}

UINT32 ISO_Adjust(void)
{

    return CAL_ERR_OK;
}

UINT32 DP_Adjust_Dark(void)
{
    return CAL_ERR_OK;
}

UINT32 DP_Adjust_White(void)
{
    return CAL_ERR_OK;
}

UINT32 DP_Adjust(void)
{
   return CAL_ERR_OK;
}


UINT32 ECS_Adjust(void)
{
    return CAL_ERR_OK;
}

//ethan_20110801_add
ER Cal_ShowCalRst(void)
{
    return E_OK;
}

void Cal_RstCalRst(void)
{
    memset((UINT8 *)&AAA_Adjust, 0, sizeof(Adjust_Struct));
}

//ethan_20110801_add
ER Cal_SaveAWBGS(void)
{
    return CAL_ERR_OK;
}
// EOF
