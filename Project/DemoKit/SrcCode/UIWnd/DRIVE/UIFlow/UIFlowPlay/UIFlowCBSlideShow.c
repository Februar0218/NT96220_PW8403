#include "UIFramework.h"

#include "Debug.h"
#include "NVTUserCommand.h"

void FlowPB_SliderCBFunc(void)
{
    Ux_PostEvent(NVTEVT_CB_SLIDE_KEEP, 1, NVTEVT_CB_SLIDE_KEEP);
}
