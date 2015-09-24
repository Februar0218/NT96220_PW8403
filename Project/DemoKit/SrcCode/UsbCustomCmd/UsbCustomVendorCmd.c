#include "Type.h"
#include "PhotoTsk.h"
#include "debug.h"

#define REQUEST_ZOOMIN       0xA0
#define REQUEST_ZOOMOUT      0xA1
#define REQUEST_AF           0xA2


UINT32 UsbCustomVerdorCmdCB(UINT8 bReguest, UINT16 wValue, UINT16 wIndex, UINT16 wLength, UINT8 *bBuf)
{
    /*
    // Example for vendor command processing
    UINT32 uiZoomPos;

    //debug_err(("[USB VENDOR CB]: Request = 0x%02x, Value = 0x%02x, Index = 0x%02x\n\r", bReguest, wValue, wIndex));
    switch(bReguest)
    {
    case REQUEST_ZOOMIN:
        switch(wIndex)
        {
        case 0:
            uiZoomPos = Lens_Zoom_GetSection();
            //debug_err(("[ZOOM IN]: uiZoomPos = %d\r\n", uiZoomPos));
            if (uiZoomPos < Lens_Zoom_GetMaxSection())
            {
                Lens_Zoom_Goto(uiZoomPos + 1);
                FlowCommon_ZoomControl_3X(UI_ZOOM_CTRL_STOP, FALSE);
            }
            break;

        case 1:
            FlowCommon_ZoomControl_3X(UI_ZOOM_CTRL_STOP, FALSE);
            break;
        }
        break;

    case REQUEST_ZOOMOUT:
        switch(wIndex)
        {
        case 0:
            uiZoomPos = Lens_Zoom_GetSection();
            //debug_err(("[ZOOM OUT]: uiZoomPos = %d\r\n", uiZoomPos));
            if (uiZoomPos > 1) // 1x zoom
            {
                Lens_Zoom_Goto(uiZoomPos - 1);
                FlowCommon_ZoomControl_3X(UI_ZOOM_CTRL_STOP, FALSE);
            }
            break;

        case 1:
            FlowCommon_ZoomControl_3X(UI_ZOOM_CTRL_STOP, FALSE);
            break;
        }
        break;

    case REQUEST_AF:
        SysSetFL(FL_MICRO, MICRO_ON);
        ImgCapture_SetParameter(_AFMode, _IMGAFMode_Macro);
        FlowCommon_AFProcess();
        break;
    }
    */

    return 1;
}
