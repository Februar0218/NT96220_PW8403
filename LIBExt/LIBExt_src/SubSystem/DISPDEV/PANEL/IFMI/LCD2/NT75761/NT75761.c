/*
    Display object object Golden Sample for driving Memory interface panel

    @file       NT75761.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Oscommon.h"
#include "display.h"
#include "dispdev_ifmi.h"
#include "dispdev_panlcomn.h"


/*
    Panel Parameters for NT75761
*/
//@{
/*Used in MI format-7 RGB666 9bits bus*/
const tPANEL_CMD tCmdModeMiFmt1[] =
{
    {MICMD_CMD,     0x11},
    {CMDDELAY_MS,   150},
    {MICMD_CMD,     0x29},
    {MICMD_CMD,     0x2C},
    {CMDDELAY_MS,   10}
};


const tPANEL_CMD *tCmdStandbyMI = NULL;

const tLCD_PARAM tModeMI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            PINMUX_LCDMODE_MI_SERIAL_BI,   //!< LCDMode
            1.164f,                        //!< fDCLK
            (0x60+96),                     //!< uiHSyncTotalPeriod
            96,                            //!< uiHSyncActivePeriod
            0x2D,                          //!< uiHSyncBackPorch
            0x24+68,                       //!< uiVSyncTotalPeriod
            68,                            //!< uiVSyncActivePeriod
            0x13,                          //!< uiVSyncBackPorchOdd
            0x13,                          //!< uiVSyncBackPorchEven
            96,                            //!< uiBufferWidth
            68,                            //!< uiBufferHeight
            96,                            //!< uiWindowWidth
            68,                            //!< uiWindowHeight
            FALSE,                         //!< bYCbCrFormat

            /* New added parameters */
            0x01,                           //!< uiHSyncSYNCwidth
            0x01                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_SERIAL_MI_SDIO,  //!< pinmux_select_lcd;
            ICST_CCIR601,                   //!< icst;
            {FALSE, FALSE},                   //!< dithering[2];
            DISPLAY_DEVICE_MI,              //!< **DONT-CARE**
            IDE_PDIR_RGB,                   //!< pdir;
            IDE_LCD_R,                      //!< odd;
            IDE_LCD_G,                      //!< even;
            TRUE,                           //!< hsinv;
            TRUE,                           //!< vsinv;
            FALSE,                          //!< hvldinv;
            FALSE,                          //!< vvldinv;
            TRUE,                           //!< clkinv;
            FALSE,                          //!< fieldinv;
            FALSE,                          //!< **DONT-CARE**
            FALSE,                          //!< interlace;
            FALSE,                          //!< **DONT-CARE**
            0x40,                           //!< ctrst;
            0x00,                           //!< brt;
            0x40,                           //!< cmults;
            FALSE,                          //!< cex;
            FALSE,                          //!< **DONT-CARE**
            TRUE,                           //!< **DONT-CARE**
            TRUE,                           //!< tv_powerdown;
            {0x00,0x00},                    //!< **DONT-CARE**

            /* New added parameters */
            FALSE,                          //!< YCex
            FALSE,                          //!< HLPF
            {FALSE,FALSE,FALSE},            //!< subpixOdd[3]
            {FALSE,FALSE,FALSE},            //!< subpixEven[3]
            {IDE_DITHER_6BITS,IDE_DITHER_6BITS,IDE_DITHER_6BITS}//!< DitherBits[3]
        },

        (tPANEL_CMD*)tCmdModeMiFmt1,                  //!< pCmdQueue
        sizeof(tCmdModeMiFmt1)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tLCD_ROT *tRotMI = NULL;

//@}

tLCD_ROT* dispdev_getLcdRotateMiCmd(UINT32 *ModeNumber)
{
    if(tRotMI != NULL)
    {
        *ModeNumber = sizeof(tRotMI)/sizeof(tLCD_ROT);
    }
    else
    {
        *ModeNumber = 0;
    }
    return (tLCD_ROT*)tRotMI;
}



tLCD_PARAM* dispdev_getConfigModeMI(UINT32 *ModeNumber)
{
    *ModeNumber = sizeof(tModeMI)/sizeof(tLCD_PARAM);
    return (tLCD_PARAM*)tModeMI;
}

tPANEL_CMD* dispdev_getStandbyCmdMI(UINT32 *CmdNumber)
{
    *CmdNumber = sizeof(tCmdStandbyMI)/sizeof(tPANEL_CMD);
    return (tPANEL_CMD*)tCmdStandbyMI;
}

#if 1
void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    if(addr & MICMD_DATA)
        mi_setCmd1(WrDATA, (value&0xff));
    else
        mi_setCmd1(WrCMD, (value&0xff));

    mi_setCmdCnt(1);
    mi_waitCmdDone();

}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
    *pValue = 0;
    debug_err(("No MI Read reg implement!\r\n"));
}
#endif

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
    BOOL    bTeSyncEn   = FALSE;
    FLOAT   MiTargetClk = 40.0; // Max is 60MHz
    UINT32  Div;

    mi_setCtrl(SERIAL_3WSPI, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, bTeSyncEn, FALSE);
    mi_setSerialCsCtrl(TRUE);
    mi_setPolCtrl(FALSE, FALSE, FALSE, FALSE, SCLMOD0);
    mi_setSerialAutoRsBitValue(TRUE);

    Div = (UINT32) (pMiConfig->fMiSrcClk/MiTargetClk);
    pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div-1));
}

void dispdev_setMiAutoCmd(void)
{
    return;
}