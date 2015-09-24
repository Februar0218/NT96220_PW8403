/*
    Display object object Golden Sample for driving Memory interface panel

    @file       ILI9325C.c
    @ingroup
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "Oscommon.h"
#include "display.h"
#include "dispdev_ifmi.h"
#include "dispdev_panlcomn.h"
#include "utility.h"

#define DISPLAY_ROTATE   DISABLE

/*
    Panel Parameters for TCON ILI9325C
*/
//@{
/*Used in MI format*/
const tPANEL_CMD tCmdModeMiFmt3[] =
{
//************* Start Initial Sequence **********//
    {MICMD_CMD,0xE9},
    {MICMD_DATA,0x20},
    {MICMD_CMD,0x11},                           //EXIT SLEEP
    {CMDDELAY_MS,100},
    {MICMD_CMD,0xD1},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x6B},
    {MICMD_DATA,0x17},
    {MICMD_CMD,0xD0},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x8d},
    {MICMD_CMD,0x35},//tearing on
    {MICMD_DATA,0x00},
    #if (DISPLAY_ROTATE == DISABLE)
    {MICMD_CMD,0x36},
    {MICMD_DATA,0x48},
    #else
    {MICMD_CMD,0x36},
    {MICMD_DATA,0x28},
    #endif
    {MICMD_CMD,0xC1},
    {MICMD_DATA,0x10},
    {MICMD_DATA,0x10},
    {MICMD_DATA,0x02},
    {MICMD_DATA,0x02},

    {MICMD_CMD,0xC0},
    {MICMD_DATA,0x00},//SET SS GS REV SM BGR BIT
    {MICMD_DATA,0x35},//SET NL BIT
    {MICMD_DATA,0x00},//
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x02},
    {MICMD_CMD,0xC5},
    {MICMD_DATA,0x07},//SET FRAME RATE
    {MICMD_CMD,0xD2},
    {MICMD_DATA,0x01},//POWER SETING FOR NORMAL MODE  AP0[2:0]
    {MICMD_DATA,0x11},//
    {MICMD_CMD,0xC8},
    {MICMD_DATA,0x04}, //SET GAMMA
    {MICMD_DATA,0x67},
    {MICMD_DATA,0x35},
    {MICMD_DATA,0x04},
    {MICMD_DATA,0x08},
    {MICMD_DATA,0x06},
    {MICMD_DATA,0x24},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x37},
    {MICMD_DATA,0x40},
    {MICMD_DATA,0x03},
    {MICMD_DATA,0x10},
    {MICMD_DATA,0x08},
    {MICMD_DATA,0x80},
    {MICMD_DATA,0x00},

    {MICMD_CMD,0xC9},
    {MICMD_DATA,0x0D},//SET GAMMA
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x0D},
    {MICMD_DATA,0x0D},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x04},
    {MICMD_DATA,0x0D},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x09},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x0D},
    {MICMD_DATA,0x0D},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x04},
    {MICMD_DATA,0x04},
    {MICMD_CMD,0xEA},
    {MICMD_DATA,0x80},//ENABLE 3 GAMMA
    {MICMD_CMD,0x3A},
    {MICMD_DATA,0x66},//0x55 565 control interface color format

    #if (DISPLAY_ROTATE == DISABLE)
    {MICMD_CMD,0x2A},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xef},
    {MICMD_CMD,0x2B},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x8F},
    #else
    {MICMD_CMD,0x2A},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x8f},
    {MICMD_CMD,0x2B},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xef},
    #endif
    {MICMD_CMD,0x29},       //DISPLAY ON
    {CMDDELAY_MS,100},
    {MICMD_CMD,0x2c}
};

const tPANEL_CMD tCmdStandbyMI[] =
{

//  {MICMD_CMD,0x28}, //Display off
  {CMDDELAY_MS, 20},
//  {MICMD_CMD,0x10}, //enter sleep mode
};

const tLCD_PARAM tModeMI[] =
{
    /***********       MI Serial Format 1      *************/
    {
        // tPANEL_PARAM
        {
            /* Old prototype */
            #if (DISPLAY_ROTATE == DISABLE)
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            6.8f,                          //!< fDCLK
            (0x40+240),                     //!< uiHSyncTotalPeriod
            240,                            //!< uiHSyncActivePeriod
            0x20,                           //!< uiHSyncBackPorch
            0x20+400,                        //!< uiVSyncTotalPeriod
            400,                            //!< uiVSyncActivePeriod
            0x10,                           //!< uiVSyncBackPorchOdd
            0x10,                           //!< uiVSyncBackPorchEven
            400,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            400,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            #else
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            6.8f,                          //!< fDCLK
            (0x40+400),                     //!< uiHSyncTotalPeriod
            400,                            //!< uiHSyncActivePeriod
            0x20,                           //!< uiHSyncBackPorch
            0x20+240,                        //!< uiVSyncTotalPeriod
            240,                            //!< uiVSyncActivePeriod
            0x10,                           //!< uiVSyncBackPorchOdd
            0x10,                           //!< uiVSyncBackPorchEven
            400,                            //!< uiBufferWidth
            240,                            //!< uiBufferHeight
            400,                            //!< uiWindowWidth
            240,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            #endif

            /* New added parameters */
            0x05,                           //!< uiHSyncSYNCwidth
            0x05                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_PARALLE_MI_8BITS | PINMUX_LCD_SEL_TE_ENABLE,  //!< pinmux_select_lcd;
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

        (tPANEL_CMD*)tCmdModeMiFmt3,                  //!< pCmdQueue
        sizeof(tCmdModeMiFmt3)/sizeof(tPANEL_CMD),    //!< nCmd
    }
};

const tPANEL_CMD tCmdRotate_None[] =
{
  //{MICMD_CMD,0x36}, // Memory Access Control
  //{MICMD_DATA,0x48},
  //{MICMD_CMD,0x2c}, //memory write
    {CMDDELAY_MS,1}

};
const tPANEL_CMD tCmdRotate_180[] =
{
  //{MICMD_CMD,0x36}, // Memory Access Control
  //{MICMD_DATA,0x88},
  //{MICMD_CMD,0x2c}, //memory write
  {CMDDELAY_MS,1}
};
const tLCD_ROT tRotMI[] =
{
    {DISPDEV_LCD_ROTATE_NONE,(tPANEL_CMD*)tCmdRotate_None,1},
    {DISPDEV_LCD_ROTATE_180,(tPANEL_CMD*)tCmdRotate_180,1}
};

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
    *ModeNumber = 0;
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

void dispdev_writeToLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 value)
{
    if(addr & MICMD_DATA)
        mi_setCmd1(WrDATA, (value&0xff));
    else
        mi_setCmd1(WrCMD, (value&0xff));

    mi_setCmdCnt(1);
    mi_waitCmdDone();

    //debug_err(("addr:%x value:%x\r\n",addr,value));
}


void dispdev_readFromLcdMi(DISPDEV_IOCTRL pDispDevControl, UINT32 addr, UINT32 *pValue)
{
    UINT32            High=0,Low=0;

    mi_setCmd1(WrCMD, (addr&0xff00)>>8);
    mi_setCmd2(WrCMD, addr&0x00ff);
    mi_setCmd3(RdDATA,0x0);
    mi_setCmd4(RdDATA,0x0);
    mi_setCmdCnt(4);
    mi_waitCmdDone();

    // Read ID
    High    = mi_getReadBack(3);
    Low     = mi_getReadBack(4);

    *pValue =0;
    debug_err(("%s: Addr(0x%4X) got 0x%2X   0x%2X \r\n",__func__,addr,High,Low));
}

void      dispdev_setMiConfig(tMI_CONFIG *pMiConfig)
{
    FLOAT   MiTargetClk = 50.0; // Max is 60MHz
    UINT32  Div;

    mi_setCtrl(PARALLEL_I80, MI_AUTOFMT3, pMiConfig->DataFmt, FALSE, FALSE, FALSE, FALSE);
    //mi_setSerialCsCtrl(TRUE);
    //mi_setPolCtrl(FALSE, FALSE, FALSE, FALSE, SCLMOD0);
    //mi_setSerialAutoRsBitValue(TRUE);

    Div = (UINT32) (pMiConfig->fMiSrcClk/MiTargetClk);
    pll_setClockRate(PLL_CLKSEL_MI_CLKDIV, PLL_MI_CLKDIV(Div-1));
}

void dispdev_setMiAutoCmd(void)
{
    BOOL        bTeSyncEn   = TRUE;
    MI_TYPE     Type;
    MI_AUTO_FMT AutoType;
    MI_DATAFMT  DataFMT;
    BOOL        bRGBSWAP;
    BOOL        bBITSWAP;
    BOOL        bSync;
    BOOL        bPINMUX;

    mi_getCtrl(&Type, &AutoType, &DataFMT, &bRGBSWAP, &bBITSWAP, &bSync, &bPINMUX);
    mi_setCtrl(Type, AutoType, DataFMT, bRGBSWAP, bBITSWAP, bTeSyncEn, bPINMUX);
    return;
}

void LCD_WriteComBegin(void)
{
    mi_setEn(FALSE);
    TimerDelayMs(40);
    mi_setManual(TRUE);

    debug_err(("%s\r\n",__func__));
}
void LCD_WriteComEnd(void)
{
    //dispdev_setMiAutoCmd();
    mi_setCmd1(WrCMD, 0x29);
    mi_setCmd2(WrCMD, 0x2c);
    mi_setCmdCnt(2);
    mi_waitCmdDone();

    mi_setManual(FALSE);
    mi_setEn(TRUE);
}
void LCD_WriteCom(UINT8 com)
{
    mi_setCmd1(WrCMD, com);

    mi_setCmdCnt(1);
    mi_waitCmdDone();
    debug_err(("%s %d\r\n",__func__,com));
}
void LCD_WriteDat(UINT8 dat)
{
    mi_setCmd1(WrDATA, dat);

    mi_setCmdCnt(1);
    mi_waitCmdDone();
    debug_err(("%s %d\r\n",__func__,dat));
}