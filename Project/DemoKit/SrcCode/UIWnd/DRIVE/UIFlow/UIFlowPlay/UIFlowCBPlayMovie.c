#include "Debug.h"
#include "UIFramework.h"
#include "UIGraphics.h"

#include "NVTUserCommand.h"
#include "AviPlayApi.h"

void UIFlowMoviePlayCBFunc(UINT32 event_id)
{

    if(event_id == AVIPLAY_EVENT_FINISH)
    {
        Ux_PostEvent(NVTEVT_CB_MOVIE_FINISH, 0);
    }
    else if(event_id == AVIPLAY_EVENT_ONE_SECOND)
    {
        Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_SEC, 0);
    }
    else if(event_id == AVIPLAY_EVENT_START)
    {
        Ux_PostEvent(NVTEVT_CB_MOVIE_START, 0 );
    }
    //else if(event_id == AVIPLAY_EVENT_ONE_VIDEOFRAME)
    //{
    //    Ux_PostEvent(NVTEVT_CB_MOVIE_ONE_VIDEOFRAME, 0);
    //}

}

