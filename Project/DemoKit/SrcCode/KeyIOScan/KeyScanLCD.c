/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanLCD.c
    @ingroup    mIPRJAPKey

    @brief      Detect LCD is rotated or not
                Detect LCD is rotated or not

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKey */
//@{

#include "GPIOMapping.h"
#include "display.h"
#include "disp_rotate.h"
#include "UIResource.h"
#include "UIGraphics.h"
#include "PrimaryTsk.h"

extern void LCD_Rotate(BOOL bRotate);

static BOOL bLCDRotate = FALSE;//ERIC EDIT
//static BOOL bLCDRotate = TRUE;//ERIC EDIT


/**
  Detect LCD is rotated or not

  Detect LCD is rotated or not.

  @param void
  @return void
*/
void KeyScan_DetLCDRotate(void)
{
    //#NT#2011/10/19#KS Hung -begin
    PDISP_OBJ       pDispObj=disp_getDisplayObject(DISP_1);
    DISPDEV_PARAM   DispDev;
    DISPROT_PARAM   RotParam;
    PrimaryModeID   CurrMode;
    //DISPDEV_OBJ     *pDispDevObj;
    //pDispDevObj = dispdev_getLcd1DevObj();

    if (bLCDRotate)
    {
        if (!GPIOMap_DetLCDRotate())
        {
            bLCDRotate = FALSE;
            //pDispDevObj->rotate(DISPDEV_LCD_ROTATE_NONE);
            DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_NONE;
            pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);
        }
    }
    else
    {
        if (GPIOMap_DetLCDRotate())
        {
            bLCDRotate = TRUE;
            //pDispDevObj->rotate(DISPDEV_LCD_ROTATE_NONE);
           DispDev.SEL.SET_ROTATE.Rot = DISPDEV_LCD_ROTATE_180;
            pDispObj->devCtrl(DISPDEV_SET_ROTATE, &DispDev);

        }
    }
    //#NT#2011/10/19#KS Hung -end
}

/**
  Return LCD rotate status

  Return LCD rotate status.

  @param void
  @return BOOL: TRUE -> LCD is rotated, FALSE -> LCD is not rotated
*/
BOOL KeyScan_IsLCDRotate(void)
{
    return bLCDRotate;
}

//@}
