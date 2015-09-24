/**
    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.

    @file       Cal_Lens_FF.c
    @ingroup    mISYSLens

    @brief      Lens calibration library

    @note       Nothing.

    @version    V1.00.000
    @author     iView SA
    @date       2011/02/26
*/

#include "Type.h"
#include "SysCfg.h"
#include "CalLens.h"

#if (_CALIBRATION_MODE_ == ENABLE)

INT16 gCalLens_FocusRangeTable[CALLENS_ZOOM_SECTION_NUM][2] = {0};

UINT8 CalLens_FarFocusAdjust(void)
{
    return 0;
}

UINT8 CalLens_NearFocusAdjust(void)
{
    return 0;
}

INT32 CalLens_GetFarFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    return 0;
}

INT32 CalLens_GetNearFocusStep(UINT32 zoomSection, BOOL bCompensate)
{
    return 0;
}

#endif
