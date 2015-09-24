
#include "SysCfg.h"
#include "CalLens.h"
#include "UILensObj.h"

UINT32 UILens_AFProcess_ST8003(void)
{
#if 0
    AF_Process();

    AFTsk_WaitIdle();

    if(gAlgMsg.AF_Status == 0)
        return UILENS_AF_STA_OK;
    else
        return UILENS_AF_STA_FAIL;
#else
    return UILENS_AF_STA_FAIL;
#endif
}

