/*
    TV HDMI TX module driver

    This driver opens the TV HDMI transmitter

    @file       TV_HDMI.c
    @ingroup    mIDISPHDMITx
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "OSCommon.h"
#include "TV_HDMI.h"
#include "hdmitx.h"
#include "Pll.h"
#include "Top.h"
#include "IDE.h"
#include "LCDTV.h"

#define VIDMAX 59

/* Video Format timing structure defined in CEA-861-D */
const TVHDMI_TIMING  VideoTiming[VIDMAX] ={

/*           VID                 HS       VS       HVLD      VVLD      CLK       DE  PROGRESSIVE HSYNC  HTOT      HSTART       HEND            VSYNC    VTOT       VOSTART     VOEND       VESTART       VEEND      WINX,   WINY */
/* 1*/{HDMI_640X480P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     96, (160+640),  (96+48),  (96+48+640),        2,    (480+45),   (2+33),  (2+33+480),    (2+33),    (2+33+480),   16,     12   },
/* 2*/{HDMI_720X480P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/* 3*/{HDMI_720X480P60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/* 4*/{HDMI_1280X720P60,      ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     40, (370+1280), (40+220), (40+220+1280),      5,    (720+30),   (5+20),  (5+20+720),    (5+20),    (5+20+720),   30,     20   },
/* 5*/{HDMI_1920X1080I60,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, FALSE,    44, (280+1920), (44+148), (44+148+1920),     10,   (1080+45),(10+15*2),(10+30+1080), (10+30+1),(10+30+1080+1),   45,     30   },
/* 6*/{HDMI_720X480I60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45), (6+15*2),  (6+30+480),(6+15*2+1),  (6+30+480+1),   16,     12   },
/* 7*/{HDMI_720X480I60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45), (6+15*2),  (6+30+480),(6+15*2+1),  (6+30+480+1),   16,     12   },
/* 8*/{HDMI_720X240P60,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 3,    (240+22),   (3+15),  (3+15+240),    (3+15),    (3+15+240),   16,      6   },
/* 9*/{HDMI_720X240P60_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 3,    (240+22),   (3+15),  (3+15+240),    (3+15),    (3+15+240),   16,      6   },
/*10*/{HDMI_1440X480I60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 248/2,(552+2880)/2,(248+228)/2,(248+228+2880)/2, 6,    (480+45),   (6+30),  (6+30+480),  (6+30+1),  (6+30+480+1),   32,     12   },
/*11*/{HDMI_1440X480I60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 248/2,(552+2880)/2,(248+228)/2,(248+228+2880)/2, 6,    (480+45),   (6+30),  (6+30+480),  (6+30+1),  (6+30+480+1),   32,     12   },
/*12*/{HDMI_1440X240P60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248/2,(552+2880)/2,(248+228)/2,(248+228+2880)/2, 3,    (240+22),   (3+15),  (3+15+240),    (3+15),    (3+15+240),   32,      6   },
/*13*/{HDMI_1440X240P60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248/2,(552+2880)/2,(248+228)/2,(248+228+2880)/2, 3,    (240+22),   (3+15),  (3+15+240),    (3+15),    (3+15+240),   32,      6   },
/*14*/{HDMI_1440X480P60,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    124, (276+1440), (124+120),(124+120+1440),     6,    (480+45),    (6+30),  (6+30+480),   (6+30),    (6+30+480),   32,     12   },
/*15*/{HDMI_1440X480P60_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    124, (276+1440), (124+120),(124+120+1440),     6,    (480+45),    (6+30),  (6+30+480),   (6+30),    (6+30+480),   32,     12   },
/*16*/{HDMI_1920X1080P60,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     44, (280+1920), (44+148), (44+148+1920),      5,   (1080+45),    (5+36), (5+36+1080),   (5+36),   (5+36+1080),   45,     30   },
/*17*/{HDMI_720X576P50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),   (5+39),    (5+39+576),   20,     16   },
/*18*/{HDMI_720X576P50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),   (5+39),    (5+39+576),   20,     16   },
/*19*/{HDMI_1280X720P50,      ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     40, (700+1280), (40+220), (40+220+1280),      5,    (720+30),    (5+20),  (5+20+720),   (5+20),    (5+20+720),   30,     20   },
/*20*/{HDMI_1920X1080I50,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, FALSE,    44, (720+1920), (44+148), (44+148+1920),     10,   (1080+45), (10+15*2),(10+30+1080),(10+30+1),(10+30+1080+1),   45,     30   },
/*21*/{HDMI_720X576I50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576), (6+38+1),  (6+38+576+1),   16,     16   },
/*22*/{HDMI_720X576I50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576), (6+38+1),  (6+38+576+1),   16,     16   },
      /* Warning: VID 23/24 's V-BP in CEA-861D is 18 lines but in Astro VA-1809 is 19 lines. */
/*23*/{HDMI_720X288P50,       ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 3,    (288+24),    (3+19),  (3+19+288),   (3+19),    (3+19+288),   16,      8   },
/*24*/{HDMI_720X288P50_16X9,  ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 3,    (288+24),    (3+19),  (3+19+288),   (3+19),    (3+19+288),   16,      8   },
/*25*/{HDMI_1440X576I50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 252/2,(576+2880)/2,(252+276)/2,(252+276+2880)/2, 6,    (576+49),  (6+19*2),  (6+38+576), (6+38+1),  (6+38+576+1),   32,     16   },
/*26*/{HDMI_1440X576I50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, FALSE, 252/2,(576+2880)/2,(252+276)/2,(252+276+2880)/2, 6,    (576+49),  (6+19*2),  (6+38+576), (6+38+1),  (6+38+576+1),   32,     16   },
/*27*/{HDMI_1440X288P50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  252/2,(576+2880)/2,(252+276)/2,(252+276+2880)/2, 3,    (288+24),  (3+19),    (3+19+288), (3+19+1),  (3+19+288+1),   32,      8   },
/*28*/{HDMI_1440X288P50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  252/2,(576+2880)/2,(252+276)/2,(252+276+2880)/2, 3,    (288+24),  (3+19),    (3+19+288), (3+19+1),  (3+19+288+1),   32,      8   },
/*29*/{HDMI_1440X576P50,      ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    128, (288+1440), (128+136),(128+136+1440),     5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   32,     16   },
/*30*/{HDMI_1440X576P50_16X9, ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,    128, (288+1440), (128+136),(128+136+1440),     5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   32,     16   },
/*31*/{HDMI_1920X1080P50,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     44, (720+1920), (44+148), (44+148+1920),      5,   (1080+45),    (5+36), (5+36+1080),    (5+36),  (5+36+1080),   45,     30   },
/*32*/{HDMI_1920X1080P24,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     44, (830+1920), (44+148), (44+148+1920),      5,   (1080+45),    (5+36), (5+36+1080),    (5+36),  (5+36+1080),   45,     30   },
/*33*/{HDMI_1920X1080P25,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     44, (720+1920), (44+148), (44+148+1920),      5,   (1080+45),    (5+36), (5+36+1080),    (5+36),  (5+36+1080),   45,     30   },
/*34*/{HDMI_1920X1080P30,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     44, (280+1920), (44+148), (44+148+1920),      5,   (1080+45),    (5+36), (5+36+1080),    (5+36),  (5+36+1080),   45,     30   },
      //35~38 Width is 2880  IDE Un-Supported
/*35*/{35,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248/2,(552+2880)/2,(248+240)/2,(248+240+2880)/2, 6,    (480+45),    (6+30), (6+30+480),     (6+30),   (6+30+480),   64,     12   },
/*36*/{36,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  248/2,(552+2880)/2,(248+240)/2,(248+240+2880)/2, 6,    (480+45),    (6+30), (6+30+480),     (6+30),   (6+30+480),   64,     12   },
/*37*/{37,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  256/2,(576+2880)/2,(256+272)/2,(256+272+2880)/2, 5,    (576+49),    (5+39), (5+39+576),     (5+39),   (5+39+576),   64,     16   },
/*38*/{38,                    ACT_LOW,  ACT_LOW,  ACT_HIGH, ACT_HIGH, ACT_LOW, ACT_HIGH, TRUE,  256/2,(576+2880)/2,(256+272)/2,(256+272+2880)/2, 5,    (576+49),    (5+39), (5+39+576),     (5+39),   (5+39+576),   64,     16   },
/*39*/{HDMI_1920X1080I50_VT1250,ACT_HIGH,ACT_HIGH,ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, FALSE,   168, (384+1920), (168+184),(168+184+1920),    10,  (1080+170), (10+57*2),(10+114+1080),(10+57*2+1),(10+114+1080+1),45,     30   },
/*40*/{HDMI_1920X1080I100,    ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, FALSE,    44, (720+1920), (44+148), (44+148+1920),     10,   (1080+45), (10+15*2), (10+30+1080),(10+30+1),(10+30+1080+1),  45,     30   },
/*41*/{HDMI_1280X720P100,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     40, (700+1280), (40+220), (40+220+1280),      5,    (720+30),    (5+20),   (5+20+720),   (5+20),   (5+20+720),   32,     20   },
/*42*/{HDMI_720X576P100,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   20,     16   },
/*43*/{HDMI_720X576P100_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   20,     16   },
/*44*/{HDMI_720X576I100,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576),  (6+38+1),   (6+38+576+1), 16,     16   },
/*45*/{HDMI_720X576I100_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576),  (6+38+1),   (6+38+576+1), 16,     16   },
/*46*/{HDMI_1920X1080I120,    ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, FALSE,    44, (280+1920), (44+148), (44+148+1920),     10,   (1080+45),  (10+15*2),(10+30+1080),(10+30+1),(10+30+1080+1),  45,     30   },
/*47*/{HDMI_1280X720P120,     ACT_HIGH, ACT_HIGH, ACT_LOW,  ACT_LOW,  ACT_LOW, ACT_HIGH, TRUE,     40, (370+1280), (40+220), (40+220+1280),      5,    (720+30),    (5+20),  (5+20+720),    (5+20),   (5+20+720),   30,     20   },
/*48*/{HDMI_720X480P120,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/*49*/{HDMI_720X480P120_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/*50*/{HDMI_720X480I120,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45),    (6+30),  (6+30+480), (6+30+1),  (6+30+480+1),   16,     12   },
/*51*/{HDMI_720X480I120_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45),    (6+30),  (6+30+480), (6+30+1),  (6+30+480+1),   16,     12   },
/*52*/{HDMI_720X576P200,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   20,     16   },
/*53*/{HDMI_720X576P200_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     64, (144+720),  (64+68),  (64+68+720),        5,    (576+49),    (5+39),  (5+39+576),    (5+39),   (5+39+576),   20,     16   },
/*54*/{HDMI_720X576I200,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576),  (6+38+1),   (6+38+576+1), 16,     16   },
/*55*/{HDMI_720X576I200_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 126/2,(288+1440)/2,(126+138)/2,(126+138+1440)/2, 6,    (576+49),  (6+19*2),  (6+38+576),  (6+38+1),   (6+38+576+1), 16,     16   },
/*56*/{HDMI_720X480P240,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/*57*/{HDMI_720X480P240_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, TRUE,     62, (138+720),  (62+60),  (62+60+720),        6,    (480+45),   (6+30),  (6+30+480),    (6+30),    (6+30+480),   20,     12   },
/*58*/{HDMI_720X480I240,       ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45),    (6+30),  (6+30+480), (6+30+1),  (6+30+480+1),   16,     12   },
/*59*/{HDMI_720X480I240_16X9,  ACT_LOW,  ACT_LOW,ACT_HIGH, ACT_HIGH,  ACT_LOW, ACT_HIGH, FALSE, 124/2,(276+1440)/2,(124+114)/2,(124+114+1440)/2, 6,    (480+45),    (6+30),  (6+30+480), (6+30+1),  (6+30+480+1),   16,     12   }
/*            VID                 HS       VS       HVLD      VVLD      CLK       DE  PROGRESSIVE HSYNC  HTOT      HSTART       HEND            VSYNC    VTOT       VOSTART     VOEND       VESTART       VEEND      WINX,   WINY */
};



// Function declaration
static BOOL tvhdmi_initVidFormat(HDMI_VIDEOID VID_Code);
static void tvhdmi_initDisplay(void);


// Static variable
static LCD_DEVICE_OBJ   LCD_DeviceObj;

static HDMI_MODE        gHDMI_Mode  =   HDMI_MODE_RGB444;
static HDMI_VIDEOID     gVID        =   HDMI_720X480P60;
static HDMI_AUDIOFMT    gAID        =   HDMI_AUDIO48KHZ;

static UINT32   guiLoopDiv = HDMI_594_PLL2_LOOPDIV;
static UINT32   guiPerDiv  = HDMI_594_PLL2_PREDIV;

static UINT32 guiBufWinHoriOffset=0x0;
static UINT32 guiBufWinVertOffset=0x0;

static UINT32 guiValidIndex;

/*
    Init Video ID information

    Init Video ID information, this information is used by IDE to config video format

    @param[out] pVID        Out structure of Video ID information
    @param[in]  VID_Code    Specify video ID

    @return
     - @b TRUE:  Done and Success.
     - @b FALSE: Non-Supported VID code.
*/
static BOOL tvhdmi_initVidFormat(HDMI_VIDEOID VID_Code)
{
    UINT8 i,Valid=0;

    //
    //Search the mapping Video ID index
    //
    for(i=0;i<VIDMAX;i++)
    {
        if(VideoTiming[i].VID == VID_Code)
        {
            guiValidIndex   = i;
            Valid           = 1;
            break;
        }
    }
    if(Valid == 0)
    {
        debug_err(("^RNo support Video Format!\r\n"));
        return FALSE;
    }


    //
    //  Handle the TV overscan/underscan capability.
    //
    if( ((hdmitx_getEdidInfo()&HDMITX_TV_UNDERSCAN)==0x0) && ( hdmitx_getEdidInfo() != 0x0) )
    {
        // Overscan
        ide_setConstantWindowOffset(VideoTiming[guiValidIndex].WINX_OFS, VideoTiming[guiValidIndex].WINY_OFS);

        guiBufWinHoriOffset = VideoTiming[guiValidIndex].WINX_OFS<<1;
        guiBufWinVertOffset = VideoTiming[guiValidIndex].WINY_OFS<<1;
    }
    else
    {
        // underscan or EDID Parsing fail
        ide_setConstantWindowOffset(0, 0);

        guiBufWinHoriOffset = 0x0;
        guiBufWinVertOffset = 0x0;
    }

    if(hdmitx_getEdidInfo()&HDMITX_TV_YCBCR444)
    {
        gHDMI_Mode  =   HDMI_MODE_YCBCR444;
    }
    else
    {
        gHDMI_Mode  =   HDMI_MODE_RGB444;
    }

    return TRUE;
}



/*
    Setup IDE according to specified video ID format
*/
static void tvhdmi_initDisplay(void)
{

    ide_setDithering(FALSE, FALSE);

    if(gHDMI_Mode==HDMI_MODE_YCBCR422)
    {
        /* 16bits */
        ide_setDevice(DISPLAY_DEVICE_HDMI_16BIT);
    }
    else
    {
        /* 24bits */
        ide_setDevice(DISPLAY_DEVICE_HDMI_24BIT);
    }

    #if _FPGA_EMULATION_
    hdmitx_setPg1(0, VideoTiming[guiValidIndex].H_TOTAL);
    hdmitx_setPg2(VideoTiming[guiValidIndex].H_SYNCTIME, VideoTiming[guiValidIndex].H_STARTTIME);
    hdmitx_setPg3((VideoTiming[guiValidIndex].H_ENDTIME - VideoTiming[guiValidIndex].H_STARTTIME), (VideoTiming[guiValidIndex].V_TOTAL));
    hdmitx_setPg4((VideoTiming[guiValidIndex].V_SYNCTIME), VideoTiming[guiValidIndex].V_ODD_STARTTIME);
    hdmitx_setPg5((VideoTiming[guiValidIndex].V_ODD_ENDTIME - VideoTiming[guiValidIndex].V_ODD_STARTTIME));
    hdmitx_setPgPattern(0x0A);
    #endif

    //ide_setPdir(IDE_PDIR_RGB);  /* RGB */
    //ide_setOdd(IDE_LCD_R);      /* R first */
    //ide_setEven(IDE_LCD_G);     /* G first */
    ide_setHsInv(VideoTiming[guiValidIndex].HS);       /* active high/low */
    ide_setVsInv(VideoTiming[guiValidIndex].VS);       /* active high/low */
    ide_setHvldInv(VideoTiming[guiValidIndex].HVLD);   /* active high/low */
    ide_setVvldInv(VideoTiming[guiValidIndex].VVLD);   /* active high/low */
    ide_setClkInv(VideoTiming[guiValidIndex].CLK);     /* active high/low */
    ide_setDeInv(VideoTiming[guiValidIndex].DE);       /* DE active high/low */

    ide_setFldInv(FALSE);
    ide_setRgbd(FALSE);
    ide_setThrough(FALSE);

    ide_setHorTiming(VideoTiming[guiValidIndex].H_SYNCTIME-1, VideoTiming[guiValidIndex].H_TOTAL-1, VideoTiming[guiValidIndex].H_STARTTIME, VideoTiming[guiValidIndex].H_ENDTIME-1);
    ide_setVerTiming(VideoTiming[guiValidIndex].V_SYNCTIME-1, VideoTiming[guiValidIndex].V_TOTAL-1, VideoTiming[guiValidIndex].V_ODD_STARTTIME, VideoTiming[guiValidIndex].V_ODD_ENDTIME-1, VideoTiming[guiValidIndex].V_EVEN_STARTTIME, VideoTiming[guiValidIndex].V_EVEN_ENDTIME-1);

    ide_setStartField(TRUE);

    ide_setCtrst(0x40);
    ide_setBrt(0x00);
    ide_setCmults(0x40);
    ide_setCex(FALSE);

    ide_setTvPowerDown(TRUE);

    if(gHDMI_Mode==HDMI_MODE_RGB444) /* output=RGB for 444 mode */
    {
        /* Output=RGB for 444 mode */
        ide_configIcst(TRUE, CST_YCBCR2RGB);
        ide_setOutComp(IDE_COMPONENT_R,IDE_COMPONENT_G,IDE_COMPONENT_B, FALSE, FALSE);
    }
    else if(gHDMI_Mode==HDMI_MODE_YCBCR444)
    {
        /* output=YCbCr for 444 and 422 mode */
        ide_configIcst(FALSE, CST_RGB2YCBCR);
        ide_setOutComp(IDE_COMPONENT_CR,IDE_COMPONENT_Y,IDE_COMPONENT_CB, FALSE, FALSE);
    }
    else
    {
        /* output=YCbCr for 444 and 422 mode */
        ide_configIcst(FALSE, CST_RGB2YCBCR);
        ide_setOutComp(IDE_COMPONENT_CB,IDE_COMPONENT_Y,IDE_COMPONENT_CR, FALSE, FALSE);
        ide_setCex(FALSE);
    }


    if(VideoTiming[guiValidIndex].PROGRESSIVE == FALSE)
    {
        //INTERLACE
        ide_setInterlace(TRUE);
    }
    else
    {
        //PROGRESSIVE
        ide_setInterlace(FALSE);
    }


    if(VideoTiming[guiValidIndex].VID == HDMI_640X480P60)
    {
        ide_configOutputLimit(FALSE);
    }
    else
    {
        ide_configOutputLimit(TRUE);
    }

}

/*
    Change PLL frequency as HDMI video format needed,
    Following code is CHIP version dependent, for example,
    96630 use PLL2 & 96450 use SSPLL as IDE source clock.
*/
BOOL tvhdmi_changeClkFreq(BOOL bNative)
{
    UINT8   uiClockDiv=0xFF;
    UINT32  LoopDiv,PreDiv;


    LCD_DeviceObj.bIDESrcPLL2 = LCD_SRCCLK_PLL2;

    //
    //  Configure PLL2 Clock frequency according to HDMI Video ID
    //
    pll_getPLL2(&LoopDiv, &PreDiv);

    if(bNative)
    {
        guiLoopDiv = LoopDiv;
        guiPerDiv  = PreDiv;
    }

    switch(gVID)
    {

    /* Change PLL2 to 432Mhz */
    case HDMI_1440X480P60:          /* VID14 */
    case HDMI_1440X480P60_16X9:     /* VID15 */
    case HDMI_1440X576P50:          /* VID29 */
    case HDMI_1440X576P50_16X9:     /* VID30 */
    case HDMI_720X576P100:          /* VID42 */
    case HDMI_720X576P100_16X9:     /* VID43 */
    case HDMI_720X480P120:          /* VID48 */
    case HDMI_720X480P120_16X9:     /* VID49 */
    case HDMI_720X576I200:          /* VID54 */
    case HDMI_720X576I200_16X9:     /* VID55 */
    case HDMI_720X480I240:          /* VID58 */
    case HDMI_720X480I240_16X9:     /* VID59 */

    case HDMI_720X576P200:          /* VID52 */
    case HDMI_720X576P200_16X9:     /* VID53 */
    case HDMI_720X480P240:          /* VID56 */
    case HDMI_720X480P240_16X9:     /* VID57 */

        if((LoopDiv != HDMI_432_PLL2_LOOPDIV) || (PreDiv != HDMI_432_PLL2_PREDIV) )
        {
            pll_setPLL2En(FALSE);
            pll_setPLL2(HDMI_432_PLL2_LOOPDIV, HDMI_432_PLL2_PREDIV);
            pll_setPLL2En(TRUE);
        }
        break;

    /* Change PLL2 to 504Mhz */
    case HDMI_640X480P60:           /* VID1 */
        if((LoopDiv != HDMI_504_PLL2_LOOPDIV) || (PreDiv != HDMI_504_PLL2_PREDIV) )
        {
            pll_setPLL2En(FALSE);
            pll_setPLL2(HDMI_504_PLL2_LOOPDIV, HDMI_504_PLL2_PREDIV);
            pll_setPLL2En(TRUE);
        }
        break;

    /* Change PLL2 to 594Mhz */
    case HDMI_720X480P60:           /* VID2 */
    case HDMI_720X480P60_16X9:      /* VID3 */
    case HDMI_720X480I60:           /* VID6 */
    case HDMI_720X480I60_16X9:      /* VID7 */
    case HDMI_720X240P60:           /* VID8 */
    case HDMI_720X240P60_16X9:      /* VID9 */
    case HDMI_1440X480I60:          /* VID10 */
    case HDMI_1440X480I60_16X9:     /* VID11 */
    case HDMI_1440X240P60:          /* VID12 */
    case HDMI_1440X240P60_16X9:     /* VID13 */
    case HDMI_720X576P50:           /* VID17 */
    case HDMI_720X576P50_16X9:      /* VID18 */
    case HDMI_720X576I50:           /* VID21 */
    case HDMI_720X576I50_16X9:      /* VID22 */
    case HDMI_720X288P50:           /* VID23 */
    case HDMI_720X288P50_16X9:      /* VID24 */
    case HDMI_1440X576I50:          /* VID25 */
    case HDMI_1440X576I50_16X9:     /* VID26 */
    case HDMI_1440X288P50:          /* VID27 */
    case HDMI_1440X288P50_16X9:     /* VID28 */
    case HDMI_720X576I100:          /* VID44 */
    case HDMI_720X576I100_16X9:     /* VID45 */
    case HDMI_720X480I120:          /* VID50 */
    case HDMI_720X480I120_16X9:     /* VID51 */

    case HDMI_1280X720P60:          /* VID4 */
    case HDMI_1920X1080I60:         /* VID5 */
    case HDMI_1920X1080P60:         /* VID16 */
    case HDMI_1280X720P50:          /* VID19 */
    case HDMI_1920X1080I50:         /* VID20 */
    case HDMI_1920X1080P50:         /* VID31 */
    case HDMI_1920X1080P24:         /* VID32 */
    case HDMI_1920X1080P25:         /* VID33 */
    case HDMI_1920X1080P30:         /* VID34 */
    case HDMI_1920X1080I100:        /* VID40 */
    case HDMI_1280X720P100:         /* VID41 */
    case HDMI_1920X1080I120:        /* VID46 */
    case HDMI_1280X720P120:         /* VID47 */
        if((LoopDiv != HDMI_594_PLL2_LOOPDIV) || (PreDiv != HDMI_594_PLL2_PREDIV) )
        {
            pll_setPLL2En(FALSE);
            pll_setPLL2(HDMI_594_PLL2_LOOPDIV, HDMI_594_PLL2_PREDIV);
            pll_setPLL2En(TRUE);
        }
        break;

    case HDMI_1920X1080I50_VT1250:  /* VID39 */
    default:
        debug_err(("^RNo support HDMI mode\r\n"));
        return TRUE;
    }


    //
    //  Get IDE Souce Clock frequency
    //
    if (LCD_DeviceObj.bIDESrcPLL2 == LCD_SRCCLK_PLL1)
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL1Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC,PLL_CLKSEL_IDE_CLKSRC_PLL1);
    }
    else
    {
        LCD_DeviceObj.uiIDESrcCLK = pll_getPLL2Freq()/1000000;
        pll_setClockRate(PLL_CLKSEL_IDE_CLKSRC,PLL_CLKSEL_IDE_CLKSRC_PLL2);
    }

    #if _FPGA_EMULATION_
    debug_msg("^BSource PLL Clock = %d \r\n",LCD_DeviceObj.uiIDESrcCLK);
    #endif


    //
    //  Configure IDE Clock frequency according to HDMI Video ID
    //
    switch(gVID)
    {
        /* Target frequency 13.5Mhz */
        case HDMI_720X480I60:          /* VID6 */
        case HDMI_720X480I60_16X9:     /* VID7 */
        case HDMI_720X240P60:          /* VID8 */
        case HDMI_720X240P60_16X9:     /* VID9 */
        case HDMI_720X576I50:          /* VID21 */
        case HDMI_720X576I50_16X9:     /* VID22 */
        case HDMI_720X288P50:          /* VID23 */
        case HDMI_720X288P50_16X9:     /* VID24 */
            LCD_DeviceObj.fIDEDstCLK = 13.5;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+13.49)/LCD_DeviceObj.fIDEDstCLK;
            break;

        /* Target frequency 25.2Mhz */
        case HDMI_640X480P60:           /* VID1 */
            #if _FPGA_EMULATION_
            LCD_DeviceObj.fIDEDstCLK = 27;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+26.99)/LCD_DeviceObj.fIDEDstCLK;
            #else
            LCD_DeviceObj.fIDEDstCLK = 25.2;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+25.19)/LCD_DeviceObj.fIDEDstCLK;
            #endif
            break;

        /* Target frequency 27Mhz */
        case HDMI_720X480P60:           /* VID2 */
        case HDMI_720X480P60_16X9:      /* VID3 */
        case HDMI_1440X480I60:          /* VID10 */
        case HDMI_1440X480I60_16X9:     /* VID11 */
        case HDMI_1440X240P60:          /* VID12 */
        case HDMI_1440X240P60_16X9:     /* VID13 */
        case HDMI_720X576P50:           /* VID17 */
        case HDMI_720X576P50_16X9:      /* VID18 */
        case HDMI_1440X576I50:          /* VID25 */
        case HDMI_1440X576I50_16X9:     /* VID26 */
        case HDMI_1440X288P50:          /* VID27 */
        case HDMI_1440X288P50_16X9:     /* VID28 */
        case HDMI_720X576I100:          /* VID44 */
        case HDMI_720X576I100_16X9:     /* VID45 */
        case HDMI_720X480I120:          /* VID50 */
        case HDMI_720X480I120_16X9:     /* VID51 */
            LCD_DeviceObj.fIDEDstCLK = 27;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+26.99)/LCD_DeviceObj.fIDEDstCLK;
            break;

        /* Target frequency 54Mhz */
        case HDMI_1440X480P60:          /* VID14 */
        case HDMI_1440X480P60_16X9:     /* VID15 */
        case HDMI_1440X576P50:          /* VID29 */
        case HDMI_1440X576P50_16X9:     /* VID30 */
        case HDMI_720X576P100:          /* VID42 */
        case HDMI_720X576P100_16X9:     /* VID43 */
        case HDMI_720X480P120:          /* VID48 */
        case HDMI_720X480P120_16X9:     /* VID49 */
        case HDMI_720X576I200:          /* VID54 */
        case HDMI_720X576I200_16X9:     /* VID55 */
        case HDMI_720X480I240:          /* VID58 */
        case HDMI_720X480I240_16X9:     /* VID59 */
            LCD_DeviceObj.fIDEDstCLK = 54;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+53.99)/LCD_DeviceObj.fIDEDstCLK;
            break;

        /* Target frequency 74.25Mhz */
        case HDMI_1280X720P60:          /* VID4 */
        case HDMI_1920X1080I60:         /* VID5 */
        case HDMI_1280X720P50:          /* VID19 */
        case HDMI_1920X1080I50:         /* VID20 */
        case HDMI_1920X1080P24:         /* VID32 */
        case HDMI_1920X1080P25:         /* VID33 */
        case HDMI_1920X1080P30:         /* VID34 */
            LCD_DeviceObj.fIDEDstCLK = 74.25;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+74.24)/LCD_DeviceObj.fIDEDstCLK;
            break;

        /* Target frequency 108Mhz */
        case HDMI_720X576P200:          /* VID52 */
        case HDMI_720X576P200_16X9:     /* VID53 */

        case HDMI_720X480P240:          /* VID56 */
        case HDMI_720X480P240_16X9:     /* VID57 */

            LCD_DeviceObj.fIDEDstCLK = 108;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+107.99)/LCD_DeviceObj.fIDEDstCLK;
            break;

        /* Target frequency 148.5Mhz */
        case HDMI_1920X1080P60:         /* VID16 */
        case HDMI_1920X1080P50:         /* VID31 */
        case HDMI_1920X1080I100:        /* VID40 */
        case HDMI_1280X720P100:         /* VID41 */
        case HDMI_1920X1080I120:        /* VID46 */
        case HDMI_1280X720P120:         /* VID47 */
            LCD_DeviceObj.fIDEDstCLK = 148.5;
            uiClockDiv = (((float)LCD_DeviceObj.uiIDESrcCLK/2.0)+148.49)/LCD_DeviceObj.fIDEDstCLK;
            break;

        default:
            return TRUE;
    }

    #if !_FPGA_EMULATION_
    if(((float)LCD_DeviceObj.uiIDESrcCLK/2.0) < (LCD_DeviceObj.fIDEDstCLK))
    {
        debug_err(("^RClock unreachable! Taget(%d)>Source(%d)\r\n",(UINT32)(LCD_DeviceObj.fIDEDstCLK),(UINT32)(LCD_DeviceObj.uiIDESrcCLK/2)));
        return TRUE;
    }
    #endif

    pll_setClockRate(PLL_CLKSEL_IDE_CLKDIV,PLL_IDE_CLKDIV((uiClockDiv-1)));

    #if _FPGA_EMULATION_
    debug_msg("IDE Clock Divider-1 = %d\r\n",uiClockDiv-1);
    #endif

    return FALSE;

}

#if 1



/**
    Open TV HDMI driver

    It would initilize the settings for specified type of HDMI mode and set it to specified operation mode.
\n  After the return of this api, the bIDESrcPLL2(Member of LCD_DEVICE_OBJ) would be updated to correct
    clock frequency accordding to the config of bIDESrcPLL2(Member of LCD_DEVICE_OBJ).

    @param[in,out]  lcddeviceobj    LCD device main object, refer the definition of LCD_DEVICE_OBJ.
    @param[in]      psizeDisplay    LCD display buffer and window size, refer to ide.h for details.

    @return void
*/
void OpenTVHDMI(LCD_DEVICE_OBJ *lcddeviceobj, LCDSIZE *psizeDisplay)
{
    PINMUX_LCDINIT  LcdInit;

    LCD_DeviceObj = *lcddeviceobj;

    if (ide_open() != E_OK)
        return;

    //#NT#2011/06/28#Klins Chen -begin
    //#NT#Apply new pinmux control
    LcdInit = pinmux_getDispMode(PINMUX_FUNC_ID_HDMI);
    gVID    = LcdInit&(PINMUX_HDMIMODE_OFFSET-1);
    //#NT#2011/06/28#Klins Chen -end

    if(hdmitx_isOpened()==FALSE)
    {
        if(ide_getEn()==TRUE)
        {
            // Disable IDE
            ide_set_en(FALSE);
            ide_waitFrameEnd();
        }

        // Open HDMI-Tx & Read EDID
        hdmitx_open();
        hdmitx_verifyVideoID(&gVID);

        // Check & Change IDE Source frequency
        if(tvhdmi_changeClkFreq(TRUE))
            return;

        // 220 HDMI is internal HW engine, close pins out.
        //#NT#2011/06/28#Klins Chen -begin
        //#NT#Apply new pinmux control
        if ((LcdInit&PINMUX_TV_HDMI_CFG_MASK) == PINMUX_TV_HDMI_CFG_NORMAL)
            pinmux_setPinmux(PINMUX_FUNC_ID_LCD, PINMUX_LCD_SEL_GPIO);
        //pinmux_select_lcd(PINMUX_LCD_GPIO);
        //#NT#2011/06/28#Klins Chen -end

        // Enable IDE clock
        pll_enableClock(IDE1_CLK);
        pll_enableClock(IDE2_CLK);

        hdmitx_setAV(&gVID, gAID, gHDMI_Mode);


        // Configure VID content
        if(tvhdmi_initVidFormat(gVID)==FALSE)
            return;

        // Configure IDE
        tvhdmi_initDisplay();

        psizeDisplay->uiWidth       = (VideoTiming[guiValidIndex].H_ENDTIME - VideoTiming[guiValidIndex].H_STARTTIME);//-guiBufWinHoriOffset;
        psizeDisplay->uiHeight      = (VideoTiming[guiValidIndex].V_ODD_ENDTIME - VideoTiming[guiValidIndex].V_ODD_STARTTIME);//-guiBufWinVertOffset;
        psizeDisplay->uiWinWidth    = (VideoTiming[guiValidIndex].H_ENDTIME - VideoTiming[guiValidIndex].H_STARTTIME)-guiBufWinHoriOffset;
        psizeDisplay->uiWinHeight   = (VideoTiming[guiValidIndex].V_ODD_ENDTIME - VideoTiming[guiValidIndex].V_ODD_STARTTIME)-guiBufWinVertOffset;


        ide_set_en(TRUE);
        ide_waitFrameEnd();

        hdmitx_checkVideoStable();
        hdmitx_enableAudioStream(TRUE);

    }
    else
    {
        //
        // HDMI is already opened, Just changing HDMI format
        //

        hdmitx_setAVMute(TRUE);
        ide_waitFrameEnd();

        hdmitx_enableAudioStream(FALSE);

        //ide_set_en(FALSE);
        ide_waitFrameEnd();

        // No check video format validation in this path
        hdmitx_verifyVideoID(&gVID);

        // Check & Change IDE Source frequency
        if(tvhdmi_changeClkFreq(FALSE))
            return;

        // Configure VID content
        if(tvhdmi_initVidFormat(gVID) == FALSE)
            return;

        // Configure IDE
        tvhdmi_initDisplay();

        psizeDisplay->uiWidth       = (VideoTiming[guiValidIndex].H_ENDTIME - VideoTiming[guiValidIndex].H_STARTTIME);
        psizeDisplay->uiHeight      = (VideoTiming[guiValidIndex].V_ODD_ENDTIME - VideoTiming[guiValidIndex].V_ODD_STARTTIME);
        psizeDisplay->uiWinWidth    = (VideoTiming[guiValidIndex].H_ENDTIME - VideoTiming[guiValidIndex].H_STARTTIME)-guiBufWinHoriOffset;
        psizeDisplay->uiWinHeight   = (VideoTiming[guiValidIndex].V_ODD_ENDTIME - VideoTiming[guiValidIndex].V_ODD_STARTTIME)-guiBufWinVertOffset;

        // Open HDMI-Tx & Read EDID
        hdmitx_setAV(&gVID, gAID, gHDMI_Mode);

        ide_setLoad();
        ide_waitFrameEnd();

        hdmitx_checkVideoStable();
        hdmitx_enableAudioStream(TRUE);

        // Wait TV Video Stable and then clear AV-Mute
        hdmitx_setAVMute(FALSE);

    }

}

/**
    Close TV HDMI

    Call this api to close the TV HDMI transmitter.

    @return void
*/
void CloseTVHDMI(void)
{
    UINT32 uiLoopDiv,uiPreDiv;

    // Close HDMI
    hdmitx_close();

    // Disable IDE
    ide_set_en(FALSE);
    ide_waitFrameEnd();

    ide_configOutputLimit(FALSE);
    ide_setOutComp(IDE_COMPONENT_R, IDE_COMPONENT_G, IDE_COMPONENT_B, FALSE, FALSE);

    // Close IDE clock
    pll_disableClock(IDE1_CLK);
    pll_disableClock(IDE2_CLK);

    ide_setConstantWindowOffset(0, 0);

    pll_getPLL2(&uiLoopDiv, &uiPreDiv);
    if((guiLoopDiv != uiLoopDiv) || (guiPerDiv != uiPreDiv) )
    {
        pll_setPLL2En(FALSE);
        pll_setPLL2(guiLoopDiv, guiPerDiv);
        pll_setPLL2En(TRUE);
    }

}

/**
    Init TV HDMI device operation mode.

    This API must be called before OpenTVHDMI() to setup the Video/Audio format of HDMI-Tx.
    The settings of Video/Audio formats will be taken effective in OpenTVHDMI().

    @param[in] VideoID  Video format ID of HDMI-Tx. Refer to hdmitx.h for supporting video formats.
     - @b HDMI_640X480P60
     - @b HDMI_720X480P60
     - @b HDMI_1280X720P60
     - @b HDMI_1920X1080I60
     - @b HDMI_720X480I60
     - @b HDMI_720X576P50
     - @b HDMI_1280X720P50
     - @b HDMI_1920X1080I50
     - @b HDMI_VID_NO_CHANGE
    @param[in] AudioID  Audio format ID of HDMI-Tx. The supported formats are as follows:
     - @b HDMI_32KHZ
     - @b HDMI_44_1KHZ
     - @b HDMI_48KHZ
     - @b HDMI_AID_NO_CHANGE

    @return void
*/
void InitTVHDMI(HDMI_VIDEOID VideoID, HDMI_AUDIOFMT AudioID)
{
    if(VideoID != HDMI_VID_NO_CHANGE)
    {
        pinmux_setDispMode(PINMUX_FUNC_ID_HDMI, (PINMUX_HDMIMODE_OFFSET+VideoID));
        //gVID = VideoID;
    }

    if(AudioID != HDMI_AUDIO_NO_CHANGE)
        gAID = AudioID;
}
#endif

