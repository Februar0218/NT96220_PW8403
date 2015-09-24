
#include "SysCfg.h"
#include "SysFlags.h"
////#include "AF_Lib.h"
#include "Lens.h"
#include "CalLens.h"
#include "UILensObj.h"

UINT32 UILens_AFProcess_ST8005(void)
{
#if 0

    UINT32  uiAFRet, calFarIdx, calNearIdx;
    UINT32  startIdx, endIdx, tmpStartIdx, tmpEndIdx, maxVA, minVA;

    UILens_AFPrepare(TRUE);

    #if (_CALIBRATION_MODE_ == ENABLE)
    if(CalLens_IsAdjust())
    {
        calFarIdx  = CalLens_GetFarFocusStep(Lens_Zoom_GetSection(), FALSE);
        calNearIdx = CalLens_GetNearFocusStep(Lens_Zoom_GetSection(), FALSE);

        //#NT#2011/05/18#Chris Chung -begin
        if (SysGetFlag(FL_MACRO) == MACRO_ON)
        {
            switch(Lens_Zoom_GetSection())
            {
            case 1:
                startIdx = calNearIdx - 20; // 2011-07-26 Chris
                endIdx   = calNearIdx + 5;
                break;
            case 2:
                tmpStartIdx = calNearIdx;
                tmpEndIdx   = calFarIdx;
                startIdx = tmpStartIdx - 6;
                endIdx   = tmpStartIdx + (tmpEndIdx - tmpStartIdx)*0.5;
                break;
            case 3:
                tmpStartIdx = calNearIdx;
                tmpEndIdx   = calFarIdx;
                startIdx = tmpStartIdx - 6;
                endIdx   = tmpStartIdx + (tmpEndIdx - tmpStartIdx)*0.5;
                break;
            }
        }
        else
        {
            switch(Lens_Zoom_GetSection())
            {
            case 1:
                startIdx = calFarIdx - 6;
                endIdx   = calFarIdx + 6;
                break;
            case 2:
                tmpStartIdx = calNearIdx;
                endIdx   = calFarIdx + 12;
                startIdx = tmpStartIdx + (endIdx-tmpStartIdx)*0.4;
                break;
            case 3:
                tmpStartIdx = calNearIdx;
                endIdx   = calFarIdx + 18;
                startIdx = tmpStartIdx + (endIdx-tmpStartIdx)*0.4;
                break;
            }
        }

        if(endIdx > (Lens_Focus_GetMaxPosition() - 3))
            endIdx = (Lens_Focus_GetMaxPosition() - 3);

        if(startIdx < endIdx)
        {
            //uiAFRet = AF_Process_ST8005(startIdx, endIdx, TRUE);
            uiAFRet = AF_Process_ST8005(startIdx, endIdx, FALSE);

            if(uiAFRet != AF_STA_OK)
                Lens_Focus_Goto(CalLens_GetFarFocusStep(Lens_Zoom_GetSection(), FALSE));
        }
        else
        {
            Lens_Focus_Goto(CalLens_GetFarFocusStep(Lens_Zoom_GetSection(), FALSE));

            uiAFRet = AF_STA_FAIL;
        }
        //#NT#2011/05/18#Chris Chung -end
    }
    else
    #endif
    {
        //#NT#2011/05/17#Chris Chung -begin
        BOOL    bDetectShutter2 = FALSE;
        UINT32  oldSection;

        AF_ResetAFInfo();

        startIdx = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 1);
        endIdx   = Lens_Focus_GetZoomTableValue(Lens_Zoom_GetSection(), 0);

        Lens_Focus_Goto(endIdx);
        tmpStartIdx = AF_SearchFullRange7(startIdx,endIdx,5,&maxVA,&minVA,3,TRUE,TRUE);

        bDetectShutter2 = GPIOMap_DetShutter2();

        if(!bDetectShutter2)
        {
            oldSection = Lens_Zoom_GetSection();
            Lens_Retract(0);
            TimerDelayMs(500);
            Lens_Init(LENS_INIT_ZOOM_PART1);
            Lens_Init(LENS_INIT_ZOOM_PART2);
            Lens_Zoom_Goto(oldSection);

            if(Lens_Zoom_GetSection() <= 2)
            {
                Lens_Focus_Goto(tmpStartIdx+10);
                tmpEndIdx = AF_SearchFullRange7(tmpStartIdx-10,tmpStartIdx+10,5,&maxVA,&minVA,3,TRUE,TRUE);
            }
            else
            {
                Lens_Focus_Goto(tmpStartIdx+15);
                tmpEndIdx = AF_SearchFullRange7(tmpStartIdx-15,tmpStartIdx+15,5,&maxVA,&minVA,3,TRUE,TRUE);
            }

            bDetectShutter2 = GPIOMap_DetShutter2();

            if(!bDetectShutter2)
                Lens_Focus_Goto(tmpEndIdx);
        }

        if(!bDetectShutter2)
            uiAFRet = AF_STA_OK;
        else
            uiAFRet = AF_STA_FAIL;
        //#NT#2011/05/17#Chris Chung -end
    }

    UILens_AFPrepare(FALSE);

    Lens_OnOff(MOTOR_POWER_OFF, 0); // turn off motor power

    return uiAFRet;

#else

    return 0;

#endif
}

