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
#include "Utility.h"

#define DISPLAY_ROTATE      DISABLE

/*
    Panel Parameters for TCON ILI9325C
*/
//@{
/*Used in MI format*/
const tPANEL_CMD tCmdModeMiFmt3[] =
{
    #if 1
    {CMDDELAY_MS,55},
    //************* Start Initial Sequence **********//
    //LCD_CtrlWrite_ILI9225G(0x00D0, 0x0003);
    {MICMD_CMD,0x00},{MICMD_CMD,0xD0},{MICMD_DATA,0x00},{MICMD_DATA,0x03},
    //LCD_CtrlWrite_ILI9225G(0x00EB, 0x0B00);
    {MICMD_CMD,0x00},{MICMD_CMD,0xEB},{MICMD_DATA,0x0B},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x00EC, 0x000F);
    {MICMD_CMD,0x00},{MICMD_CMD,0xEC},{MICMD_DATA,0x00},{MICMD_DATA,0x4F},
    //LCD_CtrlWrite_ILI9225G(0x00C7, 0x030F);
    {MICMD_CMD,0x00},{MICMD_CMD,0xC7},{MICMD_DATA,0x03},{MICMD_DATA,0x0F},
    //LCD_CtrlWrite_ILI9225G(0x0001, 0x011C);
    {MICMD_CMD,0x00},{MICMD_CMD,0x01},{MICMD_DATA,0x01},{MICMD_DATA,0x1C},
    //LCD_CtrlWrite_ILI9225G(0x0002, 0x0100);
    {MICMD_CMD,0x00},{MICMD_CMD,0x02},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0003, 0x1030);
    {MICMD_CMD,0x00},{MICMD_CMD,0x03},{MICMD_DATA,0x12},{MICMD_DATA,0x30},
    //LCD_CtrlWrite_ILI9225G(0x0007, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x07},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //{CMDDELAY_MS,55},
    //LCD_CtrlWrite_ILI9225G(0x0008, 0x0808);
    {MICMD_CMD,0x00},{MICMD_CMD,0x08},{MICMD_DATA,0x08},{MICMD_DATA,0x08},
    //LCD_CtrlWrite_ILI9225G(0x000F, 0x0901);
    {MICMD_CMD,0x00},{MICMD_CMD,0x0F},{MICMD_DATA,0x0E},{MICMD_DATA,0x01},

    //LCD_CtrlWrite_ILI9225G(0x0010, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x10},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0011, 0x1B41); //The register setting is suitable for VCI=2.8V
    {MICMD_CMD,0x00},{MICMD_CMD,0x11},{MICMD_DATA,0x1B},{MICMD_DATA,0x41},
    //LCD_CtrlWrite_ILI9225G(0x0012, 0x200E);//The register setting is suitable for VCI=2.8V
    {CMDDELAY_MS,55},
    {MICMD_CMD,0x00},{MICMD_CMD,0x12},{MICMD_DATA,0x20},{MICMD_DATA,0x0E},
    //LCD_CtrlWrite_ILI9225G(0x0013, 0x0052); //The register setting is suitable for VCI=2.8V
    {MICMD_CMD,0x00},{MICMD_CMD,0x13},{MICMD_DATA,0x00},{MICMD_DATA,0x52},
    //LCD_CtrlWrite_ILI9225G(0x0014, 0x4C5C); //The register setting is suitable for VCI=2.8V
    {MICMD_CMD,0x00},{MICMD_CMD,0x14},{MICMD_DATA,0x4A},{MICMD_DATA,0x63},
     //LCD_CtrlWrite_ILI9225G(0x0030, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x30},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0031, 0x00DB);
    {MICMD_CMD,0x00},{MICMD_CMD,0x31},{MICMD_DATA,0x00},{MICMD_DATA,0xDB},
    //LCD_CtrlWrite_ILI9225G(0x0032, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x32},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0033, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x33},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0034, 0x00DB);
    {MICMD_CMD,0x00},{MICMD_CMD,0x34},{MICMD_DATA,0x00},{MICMD_DATA,0xDB},
    //LCD_CtrlWrite_ILI9225G(0x0035, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x35},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0036, 0x00AF);
    {MICMD_CMD,0x00},{MICMD_CMD,0x36},{MICMD_DATA,0x00},{MICMD_DATA,0xAF},
    //LCD_CtrlWrite_ILI9225G(0x0037, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x37},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0038, 0x00DB);
    {MICMD_CMD,0x00},{MICMD_CMD,0x38},{MICMD_DATA,0x00},{MICMD_DATA,0xDB},
    //LCD_CtrlWrite_ILI9225G(0x0039, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x39},{MICMD_DATA,0x00},{MICMD_DATA,0x00},

    //LCD_CtrlWrite_ILI9225G(0x0020, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x20},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0021, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x21},{MICMD_DATA,0x00},{MICMD_DATA,0x00},

    //LCD_CtrlWrite_ILI9225G(0x0050, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x50},{MICMD_DATA,0x05},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0051, 0x0506);
    {MICMD_CMD,0x00},{MICMD_CMD,0x51},{MICMD_DATA,0x02},{MICMD_DATA,0x03},
    //LCD_CtrlWrite_ILI9225G(0x0052, 0x0C06);
    {MICMD_CMD,0x00},{MICMD_CMD,0x52},{MICMD_DATA,0x0A},{MICMD_DATA,0x02},
    //LCD_CtrlWrite_ILI9225G(0x0053, 0x0401);
    {MICMD_CMD,0x00},{MICMD_CMD,0x53},{MICMD_DATA,0x04},{MICMD_DATA,0x01},
    //LCD_CtrlWrite_ILI9225G(0x0054, 0x060C);
    {MICMD_CMD,0x00},{MICMD_CMD,0x54},{MICMD_DATA,0x02},{MICMD_DATA,0x0A},
    //LCD_CtrlWrite_ILI9225G(0x0055, 0x0605);
    {MICMD_CMD,0x00},{MICMD_CMD,0x55},{MICMD_DATA,0x03},{MICMD_DATA,0x02},
    //LCD_CtrlWrite_ILI9225G(0x0056, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x56},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    //LCD_CtrlWrite_ILI9225G(0x0057, 0x0104);
    {MICMD_CMD,0x00},{MICMD_CMD,0x57},{MICMD_DATA,0x01},{MICMD_DATA,0x04},
    //LCD_CtrlWrite_ILI9225G(0x0058, 0x0E02);
    {MICMD_CMD,0x00},{MICMD_CMD,0x58},{MICMD_DATA,0x01},{MICMD_DATA,0x06},
    //LCD_CtrlWrite_ILI9225G(0x0059, 0x020E);;
    {MICMD_CMD,0x00},{MICMD_CMD,0x59},{MICMD_DATA,0x06},{MICMD_DATA,0x01},

    //LCD_CtrlWrite_ILI9225G(0x0010, 0x0000);
    {MICMD_CMD,0x00},{MICMD_CMD,0x10},{MICMD_DATA,0x00},{MICMD_DATA,0x00},
    {CMDDELAY_MS,50},
    //LCD_CtrlWrite_ILI9225G(0x0007, 0x1017);
    {MICMD_CMD,0x00},{MICMD_CMD,0x07},{MICMD_DATA,0x10},{MICMD_DATA,0x17},
    //Write_Comm(0x0022)
    {MICMD_CMD,0x00},{MICMD_CMD,0x22},
    #else
    //ILI9225C_wr_command(0x0001, 0x011C);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x01},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x1C},

    //ILI9225C_wr_command(0x0002, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x02},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

  #if 1//ILI9225C_NOROTATE
    //ILI9225C_wr_command(0x0003, 0x1230);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x03},
    {MICMD_DATA,0x12},
    {MICMD_DATA,0x30},

    //ILI9225C_wr_command(0x0020, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x20},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0021, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x21},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
  #else
    //ILI9225C_wr_command(0x0003, 0x1228);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x03},
    {MICMD_DATA,0x12},
    {MICMD_DATA,0x28},

    //ILI9225C_wr_command(0x0020, 0x00AF);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x20},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xAF},

    //ILI9225C_wr_command(0x0021, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x21},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},
  #endif

    //ILI9225C_wr_command(0x0008, 0x0808);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x08},
    {MICMD_DATA,0x08},
    {MICMD_DATA,0x08},

    //ILI9225C_wr_command(0x000C, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x0C},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x000F, 0x0C01);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x0F},
    {MICMD_DATA,0x0C},
    {MICMD_DATA,0x01},

    //ILI9225C_DelayMs(50);
    {CMDDELAY_MS,50},

    //ILI9225C_wr_command(0x0010, 0x0A00);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x10},
    {MICMD_DATA,0x0A},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0011, 0x1038);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x11},
    {MICMD_DATA,0x10},
    {MICMD_DATA,0x38},

    //ILI9225C_DelayMs(50);
    {CMDDELAY_MS,50},

    //ILI9225C_wr_command(0x0012, 0x6121);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x12},
    {MICMD_DATA,0x61},
    {MICMD_DATA,0x21},

    //ILI9225C_wr_command(0x0013, 0x006F);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x13},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x6F},

    //ILI9225C_wr_command(0x0014, 0x436F);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x14},
    {MICMD_DATA,0x43},
    {MICMD_DATA,0x6F},

    //ILI9225C_wr_command(0x0030, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x30},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0031, 0x00DB);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x31},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xDB},

    //ILI9225C_wr_command(0x0032, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x32},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0033, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x33},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0034, 0x00DB);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x34},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xDB},

    //ILI9225C_wr_command(0x0035, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x35},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0036, 0x00AF);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x36},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xAF},

    //ILI9225C_wr_command(0x0037, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x37},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0038, 0x00DB);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x38},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0xDB},

    //ILI9225C_wr_command(0x0039, 0x0000);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x39},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0050, 0x0400);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x50},
    {MICMD_DATA,0x04},
    {MICMD_DATA,0x00},

    //ILI9225C_wr_command(0x0051, 0x060B);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x51},
    {MICMD_DATA,0x06},
    {MICMD_DATA,0x0B},

    //ILI9225C_wr_command(0x0052, 0x0E0B);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x52},
    {MICMD_DATA,0x0E},
    {MICMD_DATA,0x0B},

    //ILI9225C_wr_command(0x0053, 0x0103);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x53},
    {MICMD_DATA,0x01},
    {MICMD_DATA,0x03},

    //ILI9225C_wr_command(0x0054, 0x0B0E);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x54},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x0E},

    //ILI9225C_wr_command(0x0055, 0x0B06);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x55},
    {MICMD_DATA,0x0B},
    {MICMD_DATA,0x06},

    //ILI9225C_wr_command(0x0056, 0x0004);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x56},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x04},

    //ILI9225C_wr_command(0x0057, 0x0301);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x57},
    {MICMD_DATA,0x03},
    {MICMD_DATA,0x01},

    //ILI9225C_wr_command(0x0058, 0x000F);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x58},
    {MICMD_DATA,0x00},
    {MICMD_DATA,0x0F},

    //ILI9225C_wr_command(0x0059, 0x0F00);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x59},
    {MICMD_DATA,0x0F},
    {MICMD_DATA,0x00},

    //ILI9225C_DelayMs(50);
    {CMDDELAY_MS,50},

    //ILI9225C_wr_command(0x0007, 0x1017);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x07},
    {MICMD_DATA,0x10},
    {MICMD_DATA,0x17},

    //mi_setCmd1(WrCMD, 0x00);
    //mi_setCmd2(WrCMD, 0x22);
    {MICMD_CMD, 0x00},
    {MICMD_CMD, 0x22},
    #endif
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
            #if 1//(DISPLAY_ROTATE == DISABLE)
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            3.4f,                          //!< fDCLK
            (0x40+176),                     //!< uiHSyncTotalPeriod
            176,                            //!< uiHSyncActivePeriod
            0x20,                           //!< uiHSyncBackPorch
            0x20+220,                        //!< uiVSyncTotalPeriod
            220,                            //!< uiVSyncActivePeriod
            0x10,                           //!< uiVSyncBackPorchOdd
            0x10,                           //!< uiVSyncBackPorchEven
            176,                            //!< uiBufferWidth
            220,                            //!< uiBufferHeight
            176,                            //!< uiWindowWidth
            220,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            #else
            PINMUX_LCDMODE_MI_FMT3,         //!< LCDMode
            3.4f,                          //!< fDCLK
            (0x40+176),                     //!< uiHSyncTotalPeriod
            176,                            //!< uiHSyncActivePeriod
            0x20,                           //!< uiHSyncBackPorch
            0x20+220,                        //!< uiVSyncTotalPeriod
            220,                            //!< uiVSyncActivePeriod
            0x10,                           //!< uiVSyncBackPorchOdd
            0x10,                           //!< uiVSyncBackPorchEven
            224,                            //!< uiBufferWidth
            176,                            //!< uiBufferHeight
            220,                            //!< uiWindowWidth
            176,                            //!< uiWindowHeight
            FALSE,                          //!< bYCbCrFormat
            #endif

            /* New added parameters */
            0x05,                           //!< uiHSyncSYNCwidth
            0x05                            //!< uiVSyncSYNCwidth
        },

        // tIDE_PARAM
        {
            /* Old prototype */
            PINMUX_LCD_SEL_PARALLE_MI_8BITS,//|PINMUX_LCD_SEL_TE_ENABLE,  //!< pinmux_select_lcd;
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

    debug_err(("addr:%x value:%x\r\n",addr,value));
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
    FLOAT   MiTargetClk = 30.0; // Max is 60MHz
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
    BOOL        bTeSyncEn   = FALSE;
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
    mi_setCmd1(WrCMD, 0);
    mi_setCmd2(WrCMD, 0x22);
    mi_setCmdCnt(2);
    mi_waitCmdDone();

    mi_setManual(FALSE);
    mi_setEn(TRUE);
    debug_err(("%s\r\n",__func__));
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