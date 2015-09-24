/*
    Display device object Golden Sample for driving RGB/YUV 8bits panel

    @file       dispdev_gsinf8bits.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IDE.h"
#include "Top.h"
#include "GPIO.h"
#include "Pll.h"
#include "SIF.h"
#include "OSCommon.h"
#include "Utility.h"
#include "display.h"
#include "dispdev_IF8bits.h"
#include "dispdev_panlcomn.h"


static void     DispDeviceInit(tLCD_PARAM *pMode);


static ER       dispdev_openIF8Bits(void);
static ER       dispdev_closeIF8Bits(void);
static void     dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue);
static UINT32   dispdev_readLcdReg(UINT32 uiAddr);
static void     dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc);
static ER       dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot);


/*
    Exporting display device object
*/
#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
static DISPDEV_OBJ     DispDev_IF8bitsObj = { dispdev_openIF8Bits, dispdev_closeIF8Bits, dispdev_setDeviceRotate, dispdev_writeLcdReg, dispdev_readLcdReg, dispdev_setIF8bitsIOCTRL, {NULL,NULL}};
static volatile DISPDEV_IOCTRL  pIF8bitsIoControl1=NULL;
#endif




static void DispDeviceInit(tLCD_PARAM *pMode)
{
    UINT32                  i;
    SIF_CTRL                SIFCtrl;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
    #endif

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF)
    {
        sif_open();

        SIFCtrl.uiClkDiv      = 13; // 80Mhz / 2 / (13+1) = 2.85Mhz (target 3MHz)
        SIFCtrl.uiMode        = USED_SIF_MODE;
        SIFCtrl.bSyncEn       = FALSE;
        SIFCtrl.uiLength      = 16;
        SIFCtrl.bMSB          = TRUE;
        SIFCtrl.uiSenDuration = 0;
        SIFCtrl.uiSenH        = 0;
        sif_setControl((SIF_CH)DevIOCtrl.SEL.GET_REG_IF.uiSifCh, &SIFCtrl);
    }

    pDispDevControl(DISPDEV_IOCTRL_GET_PANEL_ADJUST, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust != NULL)
    {
        DevIOCtrl.SEL.GET_PANEL_ADJUST.pfpAdjust();
    }
    else if(pMode->pCmdQueue != NULL)
    {
        for(i=0;i<pMode->nCmd;i++)
        {
            if(pMode->pCmdQueue[i].uiAddress == CMDDELAY_MS)
            {
                TimerDelayMs(pMode->pCmdQueue[i].uiValue);
            }
            else if (pMode->pCmdQueue[i].uiAddress == CMDDELAY_US)
            {
                TimerDelayUs(pMode->pCmdQueue[i].uiValue);
            }
            else
            {
                dispdev_writeLcdReg(pMode->pCmdQueue[i].uiAddress, pMode->pCmdQueue[i].uiValue);
            }
        }
    }

}

#if 1
/*
    Display Device API of open LCD with RGB/YUV Serial 8bits interface

    Display Device API of open LCD with RGB/YUV Serial 8bits interface.

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.

    @return
     - @b E_PAR:    Error pDevIF content.
     - @b E_NOSPT:  Driver without supporting the specified LCD Mode.
     - @b E_OK:     LCD Open done and success.
*/
static ER dispdev_openIF8Bits(void)
{
    UINT32                  i;
    tLCD_PARAM              *pMode      = NULL;
    tLCD_PARAM              *pConfigMode= NULL;
    PINMUX_LCDINIT          LcdMode;
    UINT32                  ModeNumber;
    PINMUX_FUNC_ID          PinFuncID;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
    #endif

    dispanl_debug(("^GopenIF8Bits START\r\n"));

    if(pDispDevControl == NULL)
        return E_PAR;

    //
    //  Sortig the SysInit Config Mode support
    //
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    PinFuncID = PINMUX_FUNC_ID_LCD;
    #endif
    LcdMode = pinmux_getDispMode(PinFuncID);
    pConfigMode = dispdev_getConfigMode(&ModeNumber);
    if((pConfigMode != NULL)&&(ModeNumber>0))
    {
        for(i=0;i<ModeNumber;i++)
        {
            if(pConfigMode[i].Panel.LCDMode == LcdMode)
            {
                pMode = (tLCD_PARAM*)&pConfigMode[i];
                break;
            }
        }
    }

    if(pMode==NULL)
    {
        debug_err(("LCDMode=%d not support\r\n", LcdMode));
        return E_NOSPT;
    }

    #if 1
    if(LcdMode == PINMUX_LCDMODE_CCIR656)
    {
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddStart    = 0x20C;
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiOddEnd      = 0x16;
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenStart   = 0x106;
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiEvenEnd     = 0x11C;
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldStart  = 0x04;
        DevIOCtrl.SEL.SET_CCIR656_SYNCCODE.uiFieldEnd    = 0x10A;
        pDispDevControl(DISPDEV_IOCTRL_SET_CCIR656_SYNCCODE, &DevIOCtrl);
    }
    #endif

    // Config & Enable Display physical engine
    dispdev_setDisplay(pDispDevControl, tLCD_INF_SERIAL_8BITS, pMode, PinFuncID);

    // Config & Enable display device
    DispDeviceInit(pMode);

    dispanl_debug(("^GopenIF8Bits Done\r\n"));

    #if 0
    // Test Panel Frame Rate
    {
        UINT32 i;

        Perf_Open();
        Perf_Mark();
        for(i=0;i<1000;i++)
        {
            pDevIF->waitFrmEnd();
        }
        debug_err(("Time tick = %d us\r\n",Perf_GetDuration()));
        Perf_Close();
    }
    #endif

    return E_OK;
}

/*
    Close LCD display panel.

    Call this api to close the LCD panel device.

    @param[in] pDevIF Please refer to dispdevctrl.h for details.

    @return
     - @b E_OK:     LCD Close done and success.
*/
static ER dispdev_closeIF8Bits(void)
{
    UINT32                  i,CmdNumber;
    tPANEL_CMD              *pStandby=NULL;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
    #endif

    dispanl_debug(("^GcloseIF8Bits START\r\n"));

    // Close Display Engine First
    DevIOCtrl.SEL.SET_ENABLE.bEn = FALSE;
    pDispDevControl(DISPDEV_IOCTRL_SET_ENABLE, &DevIOCtrl);
    pDispDevControl(DISPDEV_IOCTRL_WAIT_FRAMEEND, NULL);

    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);
    #endif

    pStandby = dispdev_getStandbyCmd(&CmdNumber);

    if(pStandby != NULL)
    {
        // Set display device into Standby or powerdown.
        for(i=0; i<CmdNumber; i++)
        {
            dispdev_writeLcdReg(pStandby[i].uiAddress, pStandby[i].uiValue);
        }
    }

    // Close SIF if needed.
    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF)
    {
        sif_close();
    }
    dispanl_debug(("^GcloseIF8Bits Done\r\n"));
    return E_OK;
}

/*
    Write LCD Register

    Write LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.
    @param[in] uiValue  Assigned LCD Register Value

    @return void
*/
static void dispdev_writeLcdReg(UINT32 uiAddr, UINT32 uiValue)
{
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
    #endif

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO)
    {
        dispdev_writeToLcdGpio(pDispDevControl, uiAddr, uiValue);
    }
    else if(DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_SIF)
    {
        dispdev_writeToLcdSif(pDispDevControl, uiAddr, uiValue);
    }
}

/*
    Read LCD Register

    Read LCD Register

    @param[in] pDevIF   Please refer to dispdevctrl.h for details.
    @param[in] uiAddr   LCD Register Offset.

    @return Read back LCD Register Value
*/
static UINT32  dispdev_readLcdReg(UINT32 uiAddr)
{
    UINT32                  uiValue=0x0;
    DISPDEV_IOCTRL_PARAM    DevIOCtrl;
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    DISPDEV_IOCTRL          pDispDevControl = pIF8bitsIoControl1;
    #endif

    pDispDevControl(DISPDEV_IOCTRL_GET_REG_IF, &DevIOCtrl);
    if(DevIOCtrl.SEL.GET_REG_IF.LcdCtrl == DISPDEV_LCDCTRL_GPIO)
    {
        dispdev_readFromLcdGpio(pDispDevControl, uiAddr, &uiValue);
    }
    else
    {
        debug_err(("Error dispdev_RegRead Op!\r\n"));
    }

    return uiValue;
}

/*
    Set Display Device IOCTRL function pointer
*/
static void dispdev_setIF8bitsIOCTRL(FP pIoCtrlFunc)
{
    #if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
    pIF8bitsIoControl1 = (DISPDEV_IOCTRL)pIoCtrlFunc;
    #endif
}

/*
    Set LCD Rotation
*/
static ER dispdev_setDeviceRotate(DISPDEV_LCD_ROTATE Rot)
{
    UINT32          ModeNo,index,i;
    tLCD_ROT        *pCmdRot=NULL;
    BOOL            bValid = FALSE;

    pCmdRot = dispdev_getLcdRotateCmd(&ModeNo);

    // Search Valid Rotattion Config table index
    for(index=0;index<ModeNo;index++)
    {
        if(pCmdRot[index].Rotation == Rot)
        {
            bValid = TRUE;
            break;
        }
    }

    if(bValid == FALSE)
    {
        debug_err(("DispDev NO valid rotate!\r\n\r\n"));
        return E_PAR;
    }


    if(pCmdRot[index].pCmdQueue != NULL)
    {
        for(i=0; i< pCmdRot[index].nCmd; i++)
        {
            if( pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_MS)
            {
                TimerDelayMs(pCmdRot[index].pCmdQueue[i].uiValue);
            }
            else if( pCmdRot[index].pCmdQueue[i].uiAddress == CMDDELAY_US)
            {
                TimerDelayUs(pCmdRot[index].pCmdQueue[i].uiValue);
            }
            else
            {
                dispdev_writeLcdReg(pCmdRot[index].pCmdQueue[i].uiAddress, pCmdRot[index].pCmdQueue[i].uiValue);
            }
        }
    }

    return E_OK;
}

#endif


#if (DISPLCDSEL_IF8BITS_TYPE == DISPLCDSEL_IF8BITS_LCD1)
/**
    Get LCD 1 Device Object

    Get LCD 1 Device Object. This Object is used to control the LCD Device.

    @return The pointer of LCD Display Device object.
*/
PDISPDEV_OBJ dispdev_getLcd1DevObj(void)
{
    return &DispDev_IF8bitsObj;
}
#endif

