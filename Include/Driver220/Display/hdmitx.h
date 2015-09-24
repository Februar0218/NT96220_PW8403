/**
    Header file for HDMI module

    Header file for HDMI module.

    @file       hdmitx.h
    @ingroup    mIDISPHDMITx
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef  _HDMITX_H
#define  _HDMITX_H

#include "Type.h"

/**
    @addtogroup mIDISPHDMITx
*/
//@{


/**
    HDMI-Tx output image data format

    This enumeration is used in hdmitx_setAV() but is useless in current chip version.
    The output format of the YUV video is determined by the EDID, but we can force output RGB format by calling
    hdmitx_forceRGB().
*/
typedef enum
{
    HDMI_MODE_RGB444=0,  ///<    HDMI Output format is RGB-444
    HDMI_MODE_YCBCR422,  ///<    HDMI Output format is YCbCr-422
    HDMI_MODE_YCBCR444,  ///<    HDMI Output format is YCbCr-444

    ENUM_DUMMY4WORD(HDMI_MODE)
}HDMI_MODE;



/**
    HDMI-Tx output Video Format ID

    This enumeration is used in hdmitx_setAV() to specify the HDMI video resolution in the IDE output stream.
*/
typedef enum
{
    HDMI_640X480P60         =1,     ///< HDMI Video format is 640x480 & Progressive 60fps
    HDMI_720X480P60         =2,     ///< HDMI Video format is 720x480 & Progressive 60fps & 4:3
    HDMI_720X480P60_16X9    =3,     ///< HDMI Video format is 720x480 & Progressive 60fps & 16:9
    HDMI_1280X720P60        =4,     ///< HDMI Video format is 1280x720 & Progressive 60fps
    HDMI_1920X1080I60       =5,     ///< HDMI Video format is 1920x1080 & Interlaced 60fps
    HDMI_720X480I60         =6,     ///< HDMI Video format is 720x480 & Interlaced 60fps
    HDMI_720X480I60_16X9    =7,     ///< HDMI Video format is 720x480 & Interlaced 60fps & 16:9
    HDMI_720X240P60         =8,     ///< HDMI Video format is 720x240 & Progressive 60fps
    HDMI_720X240P60_16X9    =9,     ///< HDMI Video format is 720x240 & Progressive 60fps & 16:9
    HDMI_1440X480I60        =10,    ///< HDMI Video format is 1440x480 & Interlaced 60fps
    HDMI_1440X480I60_16X9   =11,    ///< HDMI Video format is 1440x480 & Interlaced 60fps & 16:9
    HDMI_1440X240P60        =12,    ///< HDMI Video format is 1440x240 & Progressive 60fps
    HDMI_1440X240P60_16X9   =13,    ///< HDMI Video format is 1440x240 & Progressive 60fps & 16:9
    HDMI_1440X480P60        =14,    ///< HDMI Video format is 1440x480 & Progressive 60fps
    HDMI_1440X480P60_16X9   =15,    ///< HDMI Video format is 1440x480 & Progressive 60fps & 16:9
    HDMI_1920X1080P60       =16,    ///< HDMI Video format is 1920x1080 & Progressive 60fps
    HDMI_720X576P50         =17,    ///< HDMI Video format is 720x576 & Progressive 50fps
    HDMI_720X576P50_16X9    =18,    ///< HDMI Video format is 720x576 & Progressive 50fps & 16:9
    HDMI_1280X720P50        =19,    ///< HDMI Video format is 1280x720 & Progressive 50fps
    HDMI_1920X1080I50       =20,    ///< HDMI Video format is 1920x1080 & Interlaced 50fps
    HDMI_720X576I50         =21,    ///< HDMI Video format is 720x576 & Interlaced 50fps
    HDMI_720X576I50_16X9    =22,    ///< HDMI Video format is 720x576 & Interlaced 50fps & 16:9
    HDMI_720X288P50         =23,    ///< HDMI Video format is 720x288 & Progressive 50fps
    HDMI_720X288P50_16X9    =24,    ///< HDMI Video format is 720x288 & Progressive 50fps & 16:9
    HDMI_1440X576I50        =25,    ///< HDMI Video format is 1440x576 & Interlaced 50fps
    HDMI_1440X576I50_16X9   =26,    ///< HDMI Video format is 1440x576 & Interlaced 50fps & 16:9
    HDMI_1440X288P50        =27,    ///< HDMI Video format is 1440x288 & Progressive 50fps
    HDMI_1440X288P50_16X9   =28,    ///< HDMI Video format is 1440x288 & Progressive 50fps & 16:9
    HDMI_1440X576P50        =29,    ///< HDMI Video format is 1440x576 & Progressive 50fps
    HDMI_1440X576P50_16X9   =30,    ///< HDMI Video format is 1440x576 & Progressive 50fps & 16:9
    HDMI_1920X1080P50       =31,    ///< HDMI Video format is 1920x1080 & Progressive 50fps
    HDMI_1920X1080P24       =32,    ///< HDMI Video format is 1920x1080 & Progressive 24fps
    HDMI_1920X1080P25       =33,    ///< HDMI Video format is 1920x1080 & Progressive 25fps
    HDMI_1920X1080P30       =34,    ///< HDMI Video format is 1920x1080 & Progressive 30fps
    HDMI_1920X1080I50_VT1250=39,    ///< HDMI Video format is 1920x1080 & Interlaced  50fps & V-total is 1250 lines
    HDMI_1920X1080I100      =40,    ///< HDMI Video format is 1920x1080 & Interlaced  100fps
    HDMI_1280X720P100       =41,    ///< HDMI Video format is 1280X720  & Progressive 100fps
    HDMI_720X576P100        =42,    ///< HDMI Video format is 720X576   & Progressive 100fps
    HDMI_720X576P100_16X9   =43,    ///< HDMI Video format is 720X576   & Progressive 100fps & 16:9
    HDMI_720X576I100        =44,    ///< HDMI Video format is 720X576  & Interlaced 100fps
    HDMI_720X576I100_16X9   =45,    ///< HDMI Video format is 720X576  & Interlaced 100fps & 16:9
    HDMI_1920X1080I120      =46,    ///< HDMI Video format is 1920X1080 & Interlaced 120fps
    HDMI_1280X720P120       =47,    ///< HDMI Video format is 1280X720  & Progressive 120fps
    HDMI_720X480P120        =48,    ///< HDMI Video format is 720X480   & Progressive 120fps
    HDMI_720X480P120_16X9   =49,    ///< HDMI Video format is 720X480   & Progressive 120fps & 16:9
    HDMI_720X480I120        =50,    ///< HDMI Video format is 720X480  & Interlaced 120fps
    HDMI_720X480I120_16X9   =51,    ///< HDMI Video format is 720X480  & Interlaced 120fps & 16:9
    HDMI_720X576P200        =52,    ///< HDMI Video format is 720X576  & Progressive 200fps
    HDMI_720X576P200_16X9   =53,    ///< HDMI Video format is 720X576  & Progressive 200fps & 16:9
    HDMI_720X576I200        =54,    ///< HDMI Video format is 720X576  & Interlaced 200fps
    HDMI_720X576I200_16X9   =55,    ///< HDMI Video format is 720X576  & Interlaced 200fps & 16:9
    HDMI_720X480P240        =56,    ///< HDMI Video format is 720X480  & Progressive 240fps
    HDMI_720X480P240_16X9   =57,    ///< HDMI Video format is 720X480  & Progressive 240fps & 16:9
    HDMI_720X480I240        =58,    ///< HDMI Video format is 720X480  & Interlaced 240fps
    HDMI_720X480I240_16X9   =59,    ///< HDMI Video format is 720X480  & Interlaced 240fps & 16:9

    HDMI_VID_NO_CHANGE      =0xFF,  ///< HDMI Video format use current setting
    ENUM_DUMMY4WORD(HDMI_VIDEOID)
}HDMI_VIDEOID;

/**
    HDMI-Tx input audio format

    This structure is used in hdmitx_setAV() to specify the Audio Sampling we will use in HDMI audio stream.
*/
typedef enum
{
    HDMI_AUDIO32KHZ,                ///< HDMI input Audio is 32KHz
    HDMI_AUDIO44_1KHZ,              ///< HDMI input Audio is 44.1KHz
    HDMI_AUDIO48KHZ,                ///< HDMI input Audio is 48KHz

    HDMI_AUDIO_NO_CHANGE  = 0xFF,   ///< HDMI Audio format use current setting
    ENUM_DUMMY4WORD(HDMI_AUDIOFMT)
}HDMI_AUDIOFMT;

/**
    HDMI EDID flags

    Each bit represents TV device support this feature or not.
    These flags are used for hdmitx_getEdidInfo() to determine the TV supported audio/video formats.
    All the information below is from TV's EDID.

    @name   HDMI EDID parsing results flags
*/
//@{
#define HDMITX_DTD1080P60       0x0001  ///< Video as 1080P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720P60        0x0002  ///< Video as  720P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD640X480P60    0x0004  ///< Video as  480P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720X480P60    0x0008  ///< Video as  480P60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080I60       0x0010  ///< Video as 1080I60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720X480I60    0x0020  ///< Video as  480I60 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080P50       0x0100  ///< Video as 1080P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD720P50        0x0200  ///< Video as  720P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD576P50        0x0400  ///< Video as  576P50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD1080I50       0x1000  ///< Video as 1080I50 from EDID's Detailed Timing Descriptor.
#define HDMITX_DTD576I50        0x2000  ///< Video as  576I50 from EDID's Detailed Timing Descriptor.

#define HDMITX_TV_YCBCR444     0x10000  ///< TV Support Video format: YCbCr-444.
#define HDMITX_TV_YCBCR422     0x20000  ///< TV Support Video format: YCbCr-422.
#define HDMITX_TV_UNDERSCAN    0x40000  ///< TV Support UnderScan.
#define HDMITX_TV_HDMI         0x80000  ///< TV Support HDMI. If not, it is DVI device.
#define HDMITX_ADO_32KHZ      0x100000  ///< TV Support Audio Sampling Rate 32Khz.
#define HDMITX_ADO_44KHZ      0x200000  ///< TV Support Audio Sampling Rate 44.1Khz.
#define HDMITX_ADO_48KHZ      0x400000  ///< TV Support Audio Sampling Rate 48Khz.
#define HDMITX_ADO_MORE       0x800000  ///< TV Support Advanced Audio Format.
//@}



//
//  HDMI Extern API Definitions
//

extern ER       hdmitx_open(void);
extern ER       hdmitx_close(void);
extern void     hdmitx_setAV(HDMI_VIDEOID *VID_Code,HDMI_AUDIOFMT ADOFMT,HDMI_MODE ModeFMT);
extern UINT32   hdmitx_getEdidInfo(void);
extern void     hdmitx_setAudMute(BOOL bAMute);
extern void     hdmitx_enableAudioStream(BOOL bEn);
extern void     hdmitx_setAVMute(BOOL bEn);
extern void     hdmitx_forceRGB(BOOL bRGB);
extern BOOL     hdmitx_isOpened(void);
extern BOOL     hdmitx_checkHotplug(void);
extern void     hdmitx_dumpInfo(void);
extern void     hdmitx_verifyVideoID(HDMI_VIDEOID *VID_Code);

extern void     hdmitx_setHwReset(UINT32 uiGpio);
extern void     hdmitx_checkVideoStable(void);


#if _FPGA_EMULATION_
// APIs for FPGA Verification Usage Only.
extern void hdmitx_setPgEnable(BOOL bEn);
extern void hdmitx_setPgPattern(UINT32 uiPatID);
extern void hdmitx_setPg1(UINT32 Gray, UINT32 HTOT);
extern void hdmitx_setPg2(UINT32 HSW, UINT32 HAB);
extern void hdmitx_setPg3(UINT32 HAW, UINT32 VTOT);
extern void hdmitx_setPg4(UINT32 VSW, UINT32 VAB);
extern void hdmitx_setPg5(UINT32 VAW);
extern void hdmitx_resetSil9002(void);
#endif
//@}

#endif
