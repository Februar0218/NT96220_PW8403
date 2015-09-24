#include "Type.h"
#include "UIFramework.h"
#include "WavStudioTsk.h"
#include "NVTUserCommand.h"
#define  WAVCB_MSG  0
#if (WAVCB_MSG)
#define WABCB_MSG   debug_err
#else
#define WABCB_MSG
#endif
void FlowCommon_WAVStudioCB(UINT32 uiEventID)
{
 WAVSTUDIO_DURATION  WAVDuration;

    switch(uiEventID)
    {
        case WAVSTUDIO_EVENT_RECORDING:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_RECORDING);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_RECORDING\r\n"));
            break;
        case WAVSTUDIO_EVENT_PLAYING:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_PLAYING);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_PLAYING\r\n"));
            break;
        case WAVSTUDIO_EVENT_STOPPED:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_STOPPED);
             WABCB_MSG(("CALLBACK:WAVSTUDIO_EVENT_STOPPED \r\n"));
            break;
        case WAVSTUDIO_EVENT_PAUSED :
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_PAUSED);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_PAUSED\r\n"));
            break;
        case WAVSTUDIO_EVENT_RESUMED:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_RESUMED);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_RESUMED\r\n"));
            break;
        case WAVSTUDIO_EVENT_SAVING:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_SAVING);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_SAVING\r\n"));
            break;
        case WAVSTUDIO_EVENT_DURATION:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_DURATION);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_DURATION\r\n"));
            break;
        case WAVSTUDIO_EVENT_DISKFULL:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_DISKFULL);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_DISKFULL\r\n"));
            break;
        case WAVSTUDIO_EVENT_BUFFULL:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_BUFFULL);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_BUFFULL\r\n"));
            break;
        case WAVSTUDIO_EVENT_INVALIDFILE:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_INVALIDFILE);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_INVALIDFILE\r\n"));
            break;
        case WAVSTUDIO_EVENT_RWERROR:
             Ux_PostEvent(NVTEVT_KEY_CUSTOM1, 1, WAVSTUDIO_EVENT_RWERROR);
             WABCB_MSG(("CALLBACK: WAVSTUDIO_EVENT_RWERROR\r\n"));
            break;
        default:
             WABCB_MSG(("CALLBACK EVENT = %x\r\n",uiEventID));
            break;
    }
}
