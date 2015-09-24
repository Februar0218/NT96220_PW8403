
#include "SysCfg.h"
#include "CalLens.h"
#include "UILensObj.h"

UINT32 UILens_AFProcess_DZ5311(void)
{
#if 0
    UINT32 uiAFRet;
    INT16 maxIdx, startIdx, endIdx;
    UINT32 maxVA, afTime;
    char str1[32];

    UILens_AFPrepare(TRUE);


    if(CalLens_IsFarAdjust() && CalLens_IsNearAdjust)
    {
        startIdx = CalLens_GetNearFocusStep(Lens_Zoom_GetSection(),TRUE);
        endIdx   = CalLens_GetFarFocusStep(Lens_Zoom_GetSection(), TRUE);

        if(startIdx < endIdx)
        {

            uiAFRet = AF_Process_DZ5311(startIdx,
                                        endIdx,
                                        &maxIdx, &maxVA, &afTime);
        }

        #if 0
        if(startIdx < endIdx)
        {
            snprintf(str1, 32, "%d,s=%d,e=%d,i=%d,v=%d",
                               Lens_Zoom_GetSection(),
                               startIdx,
                               endIdx,
                               maxIdx, maxVA);
        }
        else
        {
            snprintf(str1, 32, "%d,s=%d,e=%d NG!!",
                               Lens_Zoom_GetSection(),
                               startIdx,
                               endIdx,
                               maxIdx, maxVA);
        }
        Cal_AlgShowString(str1,0,0);
        #endif
    }
    else
    {
        UINT32 i;
        extern INT16 gCalLens_FocusRangeTable[CALLENS_ZOOM_SECTION_NUM][2];

        AF_ResetAFInfo();

        i = Lens_Zoom_GetSection() - 1;
        startIdx = gCalLens_FocusRangeTable[i][0];
        endIdx   = gCalLens_FocusRangeTable[i][1];

        maxIdx = AF_SearchFullRange4(startIdx, endIdx, 10, &maxVA, 80);
        maxIdx = AF_SearchFullRange4(maxIdx-8, maxIdx+8, 2, &maxVA, 80);

        Lens_Focus_Goto(maxIdx);

        uiAFRet = AF_STA_OK;
    }

    UILens_AFPrepare(FALSE);

    return uiAFRet;
#else
    return UILENS_AF_STA_FAIL;
#endif
}

