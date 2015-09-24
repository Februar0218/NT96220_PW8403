
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ImageAlgInfor.h"
#include "Utility.h"
#include "SysCfg.h"
#include "Lens.h"
#include "CalLens.h"
#include "UILensObj.h"

UINT32 UILens_GetVariationCB(void)
{
#if 0
    return (aaa_GetVariation(27)+aaa_GetVariation(28)+aaa_GetVariation(35)+aaa_GetVariation(36))/4;
#else
    return 0;
#endif
}

void UILens_AFPrepare(BOOL bBegin)
{
#if 0
    BOOL bFace = FALSE;

    if(bBegin)
    {
        if(Get_FDIndex() == FD_ON && gAlgMsg.FaceDetectNum > 0)
        {
            AF_RegGetVariationCB(AF_GetVariation_OBJ);
            AF_GetVariation_OBJInit();
            bFace = TRUE;
        }
        else
        {
            AF_RegGetVariationCB(AF_GetVariation_3X3);
        }

        AFdrv_VAPrepare(bFace, TRUE, 1);
        AFdrv_VAPrepare(bFace, TRUE, 2);
    }
    else
    {
        AFdrv_VAPrepare(FALSE, FALSE, 0);
    }
#endif
}

UINT32 UILens_AFProcess(void)
{
#if (_LENSLIB_ == _LENSLIB_A300_)
    return UILens_AFProcess_A300();
#elif (_LENSLIB_ == _LENSLIB_ST8003_)
    return UILens_AFProcess_ST8003();
#elif (_LENSLIB_ == _LENSLIB_ST8005_)
    return UILens_AFProcess_ST8005();
#elif (_LENSLIB_ == _LENSLIB_DZ5311_)
    return UILens_AFProcess_DZ5311();
#else
    return UILENS_AF_STA_FAIL;
#endif
}

void UILens_FocusToFar(UINT32 section)
{
    #if (_LENSLIB_ == _LENSLIB_ST8005_)
    return;
    #endif

    if(CalLens_IsFarAdjust())
    {
        Lens_Focus_Goto(CalLens_GetFarFocusStep(section, FALSE));
    }
    else
    {
        Lens_Focus_Goto(Lens_Focus_GetZoomTableValue(section, 0));
    }

    Lens_OnOff(MOTOR_POWER_OFF, 0);
}

void UILens_WaitZoomFinish(void)
{
#if 0
    UINT32 count = 0;

    while(gpio_getPin(GPIO_LENS_MOT_EXT2) == 1)
    {
        TimerDelayMs(100);
        count++;

        if(count > 50)
        {
            debug_err(("%s: timeout error!!\r\n"));
            break;
        }
    }
#endif
}

void UILens_WaitAeConverge(void)
{

}

INT32 UILens_SearchFullRange(INT32 startIdx, INT32 endIdx, UINT32 jumpStep)
{
    INT32   idx, maxIdx=0;
    UINT32  tempVA, maxVA=0, lastVA=0;
    UINT32  contrastCnt, dropCnt=0;

    for(idx=startIdx; idx <= (endIdx+jumpStep-1); idx += jumpStep)
    {
        Lens_Focus_Goto(idx);
        sie_waitVD(2);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = UILens_GetVariationCB();

        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if(tempVA < lastVA)
                dropCnt++;
            else
                dropCnt=0;

            if(maxVA > 1000 && dropCnt >= 3)
            {
                break;
            }
        }

        lastVA = tempVA;
    }

    return maxIdx;
}

INT32 UILens_SearchFullRange_Bwd(INT32 startIdx, INT32 endIdx, UINT32 jumpStep)
{
    INT32   idx, maxIdx=0;
    UINT32  tempVA, maxVA=0, lastVA=0;
    UINT32  contrastCnt, dropCnt=0;

    for(idx=endIdx; idx >= (startIdx-jumpStep+1); idx -= jumpStep)
    {
        Lens_Focus_Goto(idx);
        sie_waitVD(2);

        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            TimerDelayMs(1);

        tempVA = UILens_GetVariationCB();

        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if(tempVA < lastVA)
                dropCnt++;
            else
                dropCnt=0;

            if(maxVA > 1000 && dropCnt >= 3)
            {
                break;
            }
        }

        lastVA = tempVA;
    }

    return maxIdx;
}

