
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "OSCommon.h"
#include "LCDTV.h"
#include "anx7150.h"
#include "i2c.h"
#include "gpio.h"
#include "Utility.h"
#include "hdmitx.h"

static  volatile BOOL   bhdmiTxOpen     = FALSE;
static  UINT32          guiHdmiEdidInfo = 0x00;
static  UINT8           SVD_ID[HDMITX_EDIDNUM];

static  UINT8           ANX7150_video_timing_id = HDMI_1280X720P60;
static  HDMI_AUDIOFMT   ANX7150_audio_id    = HDMI_AUDIO48KHZ;

static  volatile UINT32 HwResetPin = D_GPIO_7;// default

/*
    SELECT INPUT_INTERFACE is DDR or SDR:

    ANX7150_YCMux422_SepSync_Mode1      :  CCIR601-8bits SDR
    ANX7150_RGB_YCrCb444_DDR_SepSync    :  CCIR601-8bits DDR
*/
static  UINT8   ANX7150_video_format_config = ANX7150_RGB_YCrCb444_DDR_SepSync;





//
//  Analogix Driver Local Variables
//

static  UINT8   ANX7150_ddr_edge = ANX7150_IDCK_EDGE_DDR;
audio_config_struct s_ANX7150_audio_config;
config_packets s_ANX7150_packet_config;
UINT8   ANX7150_avi_data[19];
UINT32  ANX7150_edid_length;
UINT8   ANX7150_edid_dtd[18];
UINT8   ANX7150_ext_block_num;
UINT8   ANX7150_stdaddr,ANX7150_stdreg,ANX7150_sau_length,ANX7150_svd_length;
UINT8   ANX7150_RGBorYCbCr;//INPUT_COLORSPACE
BOOL    ANX7150_emb_sync_mode,ANX7150_de_gen_en,ANX7150_demux_yc_en,ANX7150_ddr_bus_mode;
BOOL    ANX7150_ycmux_BIT_sel;
UINT8   ANX7150_tx_pix_rpt,ANX7150_in_pix_rpt=0x0;
UINT8   ANX7150_in_pix_rpt_bkp,ANX7150_tx_pix_rpt_bkp;
UINT8   ANX7150_audio_clock_edge;
UINT8   FREQ_MCLK;//0X72:0X50 bit2:0//000b:Fm = 128*Fs//001b:Fm = 256*Fs//010b:Fm = 384*Fs//011b:Fm = 512*Fs


UINT8 ANX7150_video_timing_parameter[18];

struct ANX7150_video_timingtype ANX7150_video_timingtype_table = {
     //640x480p-60hz
    {0x20/*H_RES_LOW*/, 0x03/*H_RES_HIGH*/,0x80 /*ACT_PIX_LOW*/,0x02 /*ACT_PIX_HIGH*/,
    0x60/*HSYNC_WIDTH_LOW*/,0x00 /*HSYNC_WIDTH_HIGH*/,0x30 /*H_BP_LOW*/,0x00 /*H_BP_HIGH*/,
    0xe0/*ACT_LINE_LOW*/, 0x01/*ACT_LINE_HIGH*/,0x02 /*VSYNC_WIDTH*/, 0x21/*V_BP_LINE*/,
    0x0a/*V_FP_LINE*/,0x10 /*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Progressive, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},
    //720x480p-60hz
    {0x5a/*H_RES_LOW*/,0x03 /*H_RES_HIGH*/,0xd0/*ACT_PIX_LOW*/, 0x02/*ACT_PIX_HIGH*/,
    0x3e/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x3c/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
    0xe0/*ACT_LINE_LOW*/, 0x01/*ACT_LINE_HIGH*/, 0x06/*VSYNC_WIDTH*/, 0x1e/*V_BP_LINE*/,
    0x09/*V_FP_LINE*/, 0x10/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Progressive, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},

     //720p-60hz
    {0x72/*H_RES_LOW*/, 0x06/*H_RES_HIGH*/, 0x00/*ACT_PIX_LOW*/, 0x05/*ACT_PIX_HIGH*/,
     0x28/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0xdc/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
     0xd0/*ACT_LINE_LOW*/, 0x02/*ACT_LINE_HIGH*/, 0x05/*VSYNC_WIDTH*/, 0x14/*V_BP_LINE*/,
     0x05/*V_FP_LINE*/, 0x6e/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
     ANX7150_Progressive, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},

     //1080i-60hz
    {0x98/*H_RES_LOW*/, 0x08/*H_RES_HIGH*/, 0x80/*ACT_PIX_LOW*/, 0x07/*ACT_PIX_HIGH*/,
    0x2c/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x94/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
    0x38/*ACT_LINE_LOW*/, 0x04/*ACT_LINE_HIGH*/, 0x05/*VSYNC_WIDTH*/, 0x0f/*V_BP_LINE*/,
    0x02/*V_FP_LINE*/, 0x58/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Interlace, ANX7150_Pos_Hsync_pol, ANX7150_Pos_Vsync_pol},
    //720x480i-60hz
    {0x5a/*H_RES_LOW*/,0x03 /*H_RES_HIGH*/,0xd0/*ACT_PIX_LOW*/, 0x02/*ACT_PIX_HIGH*/,
    0x3e/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x39/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
    0xe0/*ACT_LINE_LOW*/, 0x01/*ACT_LINE_HIGH*/, 0x03/*VSYNC_WIDTH*/, 0x0f/*V_BP_LINE*/,
    0x04/*V_FP_LINE*/, 0x13/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Interlace, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},                                           //update
    //576p-50hz
    {0x60/*H_RES_LOW*/,0x03 /*H_RES_HIGH*/,0xd0 /*ACT_PIX_LOW*/, 0x02/*ACT_PIX_HIGH*/,
    0x40/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x44/*H_BP_LOW*/,0x00 /*H_BP_HIGH*/,
    0x40/*ACT_LINE_LOW*/, 0x02/*ACT_LINE_HIGH*/, 0x05/*VSYNC_WIDTH*/, 0x27/*V_BP_LINE*/,
    0x05/*V_FP_LINE*/, 0x0c/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Progressive, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},
    //720p-50hz
    {0xbc/*H_RES_LOW*/, 0x07/*H_RES_HIGH*/, 0x00/*ACT_PIX_LOW*/, 0x05/*ACT_PIX_HIGH*/,
    0x28/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0xdc/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
    0xd0/*ACT_LINE_LOW*/, 0x02/*ACT_LINE_HIGH*/, 0x05/*VSYNC_WIDTH*/, 0x14/*V_BP_LINE*/,
    0x05/*V_FP_LINE*/, 0xb8/*H_FP_LOW*/, 0x01/*H_FP_HIGH*/,
    ANX7150_Progressive, ANX7150_Pos_Hsync_pol, ANX7150_Pos_Vsync_pol},
    //1080i-50hz
    {0x50/*H_RES_LOW*/, 0x0a/*H_RES_HIGH*/, 0x80/*ACT_PIX_LOW*/, 0x07/*ACT_PIX_HIGH*/,
    0x2c/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x94/*H_BP_LOW*/, 0x00/*H_BP_HIGH*/,
    0x38/*ACT_LINE_LOW*/, 0x04/*ACT_LINE_HIGH*/, 0x05/*VSYNC_WIDTH*/, 0x0f/*V_BP_LINE*/,
    0x02/*V_FP_LINE*/, 0x10/*H_FP_LOW*/, 0x02/*H_FP_HIGH*/,
    ANX7150_Interlace, ANX7150_Pos_Hsync_pol, ANX7150_Pos_Vsync_pol},
     //576i-50hz
    {0x60/*H_RES_LOW*/,0x03 /*H_RES_HIGH*/,0xd0 /*ACT_PIX_LOW*/, 0x02/*ACT_PIX_HIGH*/,
    0x3f/*HSYNC_WIDTH_LOW*/, 0x00/*HSYNC_WIDTH_HIGH*/, 0x45/*H_BP_LOW*/,0x00 /*H_BP_HIGH*/,
    0x40/*ACT_LINE_LOW*/,0x02 /*ACT_LINE_HIGH*/, 0x03/*VSYNC_WIDTH*/, 0x13/*V_BP_LINE*/,
    0x02/*V_FP_LINE*/, 0x0c/*H_FP_LOW*/, 0x00/*H_FP_HIGH*/,
    ANX7150_Interlace, ANX7150_Neg_Hsync_pol, ANX7150_Neg_Vsync_pol},
};





UINT32  anx7150_allocate(void);
BOOL    anx7150_checkHotplug(void);

ER      anx7150_open(void);
ER      anx7150_close(void);
void    anx7150_openVideo(void);

ER      anx7150_readReg(UINT32 DevAddr, UINT32 uiRegAddr, UINT32 *puiData);
ER      anx7150_writeReg(UINT32 DevAddr, UINT32 uiRegAddr, UINT32 uiData);

UINT32  ANX7150_i2c_read_p0_reg(UINT32 offset);
UINT32  ANX7150_i2c_read_p1_reg(UINT32 offset);
void    ANX7150_i2c_write_p0_reg(UINT32 offset, UINT32 Data);
void    ANX7150_i2c_write_p1_reg(UINT32 offset, UINT32 Data);
void    ANX7150_set_p0_bit(UINT32 offset, UINT32 bit);
void    ANX7150_set_p1_bit(UINT32 offset, UINT32 bit);
void    ANX7150_clear_p0_bit(UINT32 offset, UINT32 bit);
void    ANX7150_clear_p1_bit(UINT32 offset, UINT32 bit);

void    ANX7150_API_Video_Config(UINT32 video_id,UINT32 input_pixel_rpt_time);
void    ANX7150_API_Packets_Config(UINT32 pkt_sel);
void    ANX7150_API_AVI_Config(UINT8 pb1,UINT8 pb2,UINT8 pb3,UINT8 pb4,UINT8 pb5,
    UINT8 pb6,UINT8 pb7,UINT8 pb8,UINT8 pb9,UINT8 pb10,UINT8 pb11,UINT8 pb12,UINT8 pb13);
void    ANX7150_API_AUD_INFO_Config(UINT8 pb1,UINT8 pb2,UINT8 pb3,UINT8 pb4,UINT8 pb5,
    UINT8 pb6,UINT8 pb7,UINT8 pb8,UINT8 pb9,UINT8 pb10);
void    ANX7150_API_AUD_CHStatus_Config(UINT8 MODE,UINT8 PCM_MODE,UINT8 SW_CPRGT,UINT8 NON_PCM,
    UINT8 PROF_APP,UINT8 CAT_CODE,UINT8 CH_NUM,UINT8 SOURCE_NUM,UINT8 CLK_ACCUR,UINT8 Fs);
void    ANX7150_API_System_Config(void);


void    ANX7150_initHardware(void);
void    ANX7150_Set_AVMute(void);
void    ANX7150_Clear_AVMute(void);
void    ANX7150_RST_DDCChannel(void);
void    ANX7150_Clean_HDCP(void);
void    ANX7150_HotplugChange(BOOL Plug);
void    ANX7150_Reset_AVI(void);
void    ANX7150_Variable_Initial(void);

BOOL    ANX7150_Parse_EDID(void);

void    ANX7150_Config_Video(void);
UINT32  ANX7150_Config_Audio(void);
UINT32  ANX7150_Config_Packet(void);
void    ANX7150_HDCP_Process(void);
void    ANX7150_PLAYBACK_Process(void);


void    ANX7150_TimerSlot1(void);
void    ANX7150_TimerSlot2(void);
void    ANX7150_TimerSlot3(void);


#if 1

/**
    Open HDMI-Tx Module

    Open HDMI-Tx Module

    @return void
     - @b E_OK: Done and success
*/
ER hdmitx_open(void)
{
    if(bhdmiTxOpen == TRUE)
        return E_OK;

    if(anx7150_open() != E_OK)
    {
        return E_SYS;
    }

    bhdmiTxOpen=TRUE;

    return E_OK;
}

/**
    Close the HDMI-Tx module

    Close the HDMI-Tx module, this api would power down the hdmi-tx module

    @return void
     - @b E_OK: Done and success
*/
ER  hdmitx_close(void)
{
    if(bhdmiTxOpen==FALSE)
        return E_OK;

    anx7150_close();

    bhdmiTxOpen=FALSE;
    return E_OK;
}


/**
    Set HDMI-Tx Audio & Video Attributes

    Set HDMI-Tx Audio & Video Attributes, such as Video-ID, Audio format,
    and HDMI-Tx output image data format.

    @param[in,out]  VID_Code    Video-ID defined in CEA-861D, please refer to hdmitx.h for details.
    @param[in]      ADOFMT      HDMI audio input format, please refer to hdmitx.h for details.
    @param[in]      ModeFMT     HDMI output image data format, please refer to hdmitx.h for details.

    @return void
*/
void hdmitx_setAV(HDMI_VIDEOID *VID_Code, HDMI_AUDIOFMT ADOFMT, HDMI_MODE ModeFMT)
{
    BOOL    Valid;
    UINT32  i,Reg;


    //
    //  Select Video Format
    //
    if(*VID_Code != HDMI_VID_NO_CHANGE)
    {
        Valid=0;
        for(i=0;i<HDMITX_EDIDNUM;i++)
        {
            if(SVD_ID[i] == *VID_Code)
            {
                Valid = 1;
            }
        }
        if(*VID_Code == HDMI_1280X720P60)
        {
            if(guiHdmiEdidInfo & HDMITX_DTD720P60)
                Valid = 1;
        }

        if(Valid == 0)
        {
            debug_err(("Desired Video Format Device not support!! Output Basic format HDMI_720X480P60!!! \r\n"));
            // Basic Format
            *VID_Code = HDMI_720X480P60;
        }

        ANX7150_video_timing_id = *VID_Code;
    }


    //
    //  Select Audio Sampling Rate
    //
    if(ADOFMT != HDMI_AUDIO_NO_CHANGE)
    {
        UINT32 temp;

        if(ADOFMT == HDMI_AUDIO32KHZ)
            temp = ANX7150_I2SCH_STATUS4_32Khz;
        else if(ADOFMT == HDMI_AUDIO44_1KHZ)
            temp = ANX7150_I2SCH_STATUS4_44Khz;
        else
            temp = ANX7150_I2SCH_STATUS4_48Khz;

        Reg = ANX7150_i2c_read_p0_reg(ANX7150_I2SCH_STATUS4_REG);
        Reg &= ~ 0x0F;
        Reg |= temp;
        ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS4_REG, Reg);

        ANX7150_audio_id = ADOFMT;
    }


    //
    //  Open AV
    //
    anx7150_openVideo();

}

/**
    Check the HDMI-Tx driver is opened or not

    Check the HDMI-Tx driver is opened or not

    @return
     - @b TRUE:  Already opened.
     - @b FALSE: Do not opened yet.
*/
BOOL hdmitx_isOpened(void)
{
    return bhdmiTxOpen;
}

/**
    Get HDMI EDID information

    Get HDMI EDID Parsing results flags.

    @return Flags
*/
UINT32 hdmitx_getEdidInfo(void)
{
    return guiHdmiEdidInfo;
}

/**
    Set HDMI Audio Mute Enable/Disable

    @param[in]  bAMute
     - @b TRUE:  Mute HDMI Audio
     - @b FALSE: Un-Mute HDMI Audio

    @return void
*/
void hdmitx_setAudMute(BOOL bAMute)
{
    if(bAMute)
        ANX7150_clear_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);
    else
        ANX7150_set_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);
}

/**
    Force HDMI output format as RGB

    Force HDMI output format as RGB

    @param[in] bRGB
     -@b TRUE:  Enable
     -@b FALSE: DISABLE
*/
void hdmitx_forceRGB(BOOL bRGB)
{
    debug_err(("Force RGB not implement yet!! \r\n"));
}

/**
    Check hotplug or not

    Check hotplug or not

    @return
     - @b TRUE:  Hotplug attached.
     - @b FALSE: Hotplug detached.
*/
BOOL hdmitx_checkHotplug(void)
{
    return anx7150_checkHotplug();
}

/**
    Assert Hardware Reset

    Assert Hardware Reset

    @param[in]  uiGpio  Hardware reset pin GPIO ID, this pin depends on HW layout.

    @return void
*/
void hdmitx_setHwReset(UINT32 uiGpio)
{
    gpio_setDir(uiGpio, GPIO_DIR_OUTPUT);

    gpio_clearPin(uiGpio);
    TimerDelayMs(20);
    gpio_setPin(uiGpio);
    TimerDelayMs(10);

    HwResetPin = uiGpio;
}



/**
    Check Video Stable

    Check Video Stable

    @return void
*/
void hdmitx_checkVideoStable(void)
{
    UINT32 Reg;

    while(1)
    {
        Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_STATUS_REG);
        if(!(Reg & ANX7150_VID_STATUS_VID_STABLE))
        {
            anx7150_msg(("Video not stable!!!!!!!!!!!!!!!!!\n\r"));
        }
        else
            break;
    }
    anx7150_msg(("Video is stable!!!!!!!!!!!!!!!!!\n\r"));
}


#endif


#if 1

UINT32 anx7150_allocate(void)
{
    //
    //  Allocate Device ANX7150 Existed
    //
    if (ANX7150_i2c_read_p0_reg(ANX7150_DEV_IDL_REG) != 0x50)
    {
        debug_err(("ANX7150 Device NOT detected! \r\n"));
        return 1;
    }
    if (ANX7150_i2c_read_p0_reg(ANX7150_DEV_IDH_REG) != 0x71)
    {
        debug_err(("ANX7150 Device NOT detected! \r\n"));
        return 1;
    }

    anx7150_msg(("ANX7150 Device detected! \r\n"));
    return 0;
}


BOOL anx7150_checkHotplug(void)
{
    UINT32 Reg;
    #if (ANX7150_DEBUG && ANX7150_FORCE_PLUG)
    // debug usage
    return TRUE;
    #else
    if(anx7150_allocate())
        return FALSE;

    // Power On DDC
    if((ANX7150_i2c_read_p0_reg(ANX7150_SYS_CTRL3_REG) & ANX7150_SYS_CTRL3_I2C_PWON) == 0x0)
    {
        ANX7150_set_p0_bit(ANX7150_SYS_CTRL3_REG, ANX7150_SYS_CTRL3_I2C_PWON);
        ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, 0x00);
    }


    Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_STATE_REG);
    if (Reg & ANX7150_SYS_STATE_HP)
    {
        anx7150_msg(("Hotplug attached!! \r\n"));
        return TRUE;
    }
    else
    {
        anx7150_msg(("Hotplug detached!! \r\n"));
        return FALSE;
    }
    #endif
}


#endif



#if 1

/*
    Open ANX7150 HDMI-Tx Module

    Open ANX7150 HDMI-Tx Module

    @return
     - @b E_OK:  Open success.
     - Others:   Error Occured.
*/
ER anx7150_open(void)
{
    // Allocate Chip Existence
    if(anx7150_allocate())
        return E_SYS;

    // Initialize hdmi-tx
    ANX7150_Variable_Initial();
    ANX7150_initHardware();

    // Set AV-Mute
    ANX7150_Set_AVMute();

    // Clean HDCP
    ANX7150_Clean_HDCP();

    // Go to State Machine
    ANX7150_HotplugChange(TRUE);//Plug

    // Read & Parsing EDID
    ANX7150_TimerSlot1();
    ANX7150_API_System_Config();

    debug_err(("HDMI EDID Parsing results: 0X%08X \r\n",guiHdmiEdidInfo));

    return E_OK;
}

/*
    Close ANX7150 HDMI-Tx Module

    Close ANX7150 HDMI-Tx Module

    @return
     - @b E_OK:  Close success.
     - Others:   Error Occured.
*/
ER anx7150_close(void)
{
    //Power-down chip
    ANX7150_clear_p0_bit(ANX7150_SYS_CTRL1_REG, ANX7150_SYS_CTRL1_PWDN_CTRL);
    ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, 0x00);

    return E_OK;
}

void anx7150_openVideo(void)
{
    ANX7150_TimerSlot2();
    ANX7150_TimerSlot3();
}
#endif




#if 1

ER anx7150_readReg(UINT32 DevAddr, UINT32 uiRegAddr, UINT32 *puiData)
{
    if(i2c_lock() != E_OK)
    {
        debug_err(("ANX7150: readReg I2C Lock Fail\r\n"));
        return E_SYS;
    }

    if(i2c_open() != E_OK)
    {
        debug_err(("ANX7150: readReg I2C open Fail\r\n"));
        i2c_unlock();
        return E_SYS;
    }


    if( i2c_initMaster(ANX7150_I2C_DIV, ANX7150_I2C_TSR, ANX7150_I2C_GSR) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: readReg Error init I2C! \r\n"));
        i2c_unlock();
        return E_SYS;
    }
    i2c_enable();



    if(i2c_masterTransmit((DevAddr|ANX7150_WRITE), 1, 0) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: readReg I2C Tx error 1! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }
    if(i2c_masterTransmit(uiRegAddr, 0, 0) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: readReg I2C Tx error 2! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }

    if(i2c_masterTransmit((DevAddr|ANX7150_READ), 1, 0) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: readReg I2C Tx error 3! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }
    if(i2c_masterReceive(puiData, 1, 1) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: readReg I2C Rx error 4! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }


    if( i2c_unlock() != E_OK )
    {
        debug_err(("ANX7150: readReg I2C UnLock Fail\r\n"));
        return E_SYS;
    }


    //anx7150_msg(("ReadReg 0x%02X Got Data = 0x%X \r\n",uiRegAddr,*puiData));

    return E_OK;
}

ER anx7150_writeReg(UINT32 DevAddr, UINT32 uiRegAddr, UINT32 uiData)
{
    if(i2c_lock() != E_OK)
    {
        debug_err(("ANX7150: writeReg I2C Lock Fail\r\n"));
        i2c_unlock();
        return E_SYS;
    }

    if(i2c_open() != E_OK)
    {
        debug_err(("ANX7150: writeReg I2C open Fail\r\n"));
        i2c_unlock();
        return E_SYS;
    }


    if( i2c_initMaster(ANX7150_I2C_DIV, ANX7150_I2C_TSR, ANX7150_I2C_GSR) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: writeReg Error init I2C! \r\n"));
        i2c_unlock();
        return E_SYS;
    }
    i2c_enable();



    if(i2c_masterTransmit((DevAddr|ANX7150_WRITE), 1, 0) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: writeReg Tx error 1! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }
    if(i2c_masterTransmit(uiRegAddr, 0, 0) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: writeReg Tx error 2! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }

    if(i2c_masterTransmit(uiData, 0, 1) != I2C_SUCCESS )
    {
        debug_err(("ANX7150: writeReg Tx error 3! \r\n"));
        i2c_unlock();
        return E_TMOUT;
    }


    if( i2c_unlock() != E_OK )
    {
        debug_err(("ANX7150: writeReg UnLock I2C Fail\r\n"));
        i2c_unlock();
        return E_SYS;
    }


    #if ANX7150_DEBUG
    {
        UINT32 ReadBack;

        anx7150_readReg(DevAddr, uiRegAddr, &ReadBack);
        if(ReadBack != uiData)
        {
            anx7150_msg(("ANX7150 I2C Register Write/Read Not Match! Reg=0x%02X Correct=0x%02X  But Got=0x%02X \r\n",uiRegAddr,uiData,ReadBack));
        }
        else
        {
            ;//anx7150_msg(("ANX7150 I2C Register Write/Read Match! Reg=0x%02X \r\n",uiRegAddr));
        }
    }
    #endif


    return E_OK;
}



UINT32 ANX7150_i2c_read_p0_reg(UINT32 offset)
{
    UINT32 Data;

    if (anx7150_readReg(ANX7150_REGBANK0, offset, &Data) != E_OK)
    {
        debug_err(("ANX7150_i2c_read_p0_reg Failed! \r\n"));
    }

    #if ANX7150_DUMP_REGACCESS
    anx7150_msg(("ANX7150 Read p0 reg(0x%02X) = 0x%02X \r\n", offset, Data));
    #endif

    return Data;
}

UINT32 ANX7150_i2c_read_p1_reg(UINT32 offset)
{
    UINT32 Data;

    if (anx7150_readReg(ANX7150_REGBANK1, offset, &Data) != E_OK)
    {
        debug_err(("ANX7150_i2c_read_p0_reg Failed! \r\n"));
    }

    #if ANX7150_DUMP_REGACCESS
    anx7150_msg(("ANX7150 Read p1 reg(0x%02X) = 0x%02X \r\n", offset, Data));
    #endif

    return Data;
}


void ANX7150_i2c_write_p0_reg(UINT32 offset, UINT32 Data)
{
    if (anx7150_writeReg(ANX7150_REGBANK0, offset, Data) != E_OK)
    {
        debug_err(("ANX7150_i2c_write_p0_reg Failed! \r\n"));
    }

    #if ANX7150_DUMP_REGACCESS
    anx7150_msg(("ANX7150 Write p0 reg(0x%02X) = 0x%02X \r\n", offset, Data));
    #endif
}

void ANX7150_i2c_write_p1_reg(UINT32 offset, UINT32 Data)
{
    if (anx7150_writeReg(ANX7150_REGBANK1, offset, Data) != E_OK)
    {
        debug_err(("ANX7150_i2c_write_p1_reg Failed! \r\n"));
    }
    #if ANX7150_DUMP_REGACCESS
    anx7150_msg(("ANX7150 Write p1 reg(0x%02X) = 0x%02X \r\n", offset, Data));
    #endif
}

void ANX7150_set_p0_bit(UINT32 offset, UINT32 bit)
{
    UINT32 Reg;

    Reg= ANX7150_i2c_read_p0_reg(offset);
    #if !ANX7150_I2CSPEEDUP
    ANX7150_i2c_write_p0_reg(offset, (Reg | bit));
    #else
    if(!(Reg&bit))
        ANX7150_i2c_write_p0_reg(offset, (Reg | bit));
    #endif

}

void ANX7150_set_p1_bit(UINT32 offset, UINT32 bit)
{
    UINT32 Reg;

    Reg= ANX7150_i2c_read_p1_reg(offset);
    #if !ANX7150_I2CSPEEDUP
    ANX7150_i2c_write_p1_reg(offset, (Reg | bit));
    #else
    if(!(Reg&bit))
        ANX7150_i2c_write_p1_reg(offset, (Reg | bit));
    #endif
}

void ANX7150_clear_p0_bit(UINT32 offset, UINT32 bit)
{
    UINT32 Reg;

    Reg= ANX7150_i2c_read_p0_reg(offset);
    #if !ANX7150_I2CSPEEDUP
    Reg &= ~bit;
    ANX7150_i2c_write_p0_reg(offset, Reg);
    #else
    if(Reg&bit)
    {
        Reg &= ~bit;
        ANX7150_i2c_write_p0_reg(offset, Reg);
    }
    #endif
}

void ANX7150_clear_p1_bit(UINT32 offset, UINT32 bit)
{
    UINT32 Reg;

    Reg= ANX7150_i2c_read_p1_reg(offset);
    #if !ANX7150_I2CSPEEDUP
    Reg &= ~bit;
    ANX7150_i2c_write_p1_reg(offset, Reg);
    #else
    if(Reg&bit)
    {
        Reg &= ~bit;
        ANX7150_i2c_write_p1_reg(offset, Reg);
    }
    #endif
}

#endif
void ANX7150_API_Video_Config(UINT32 video_id,UINT32 input_pixel_rpt_time)
{
    ANX7150_video_timing_id = video_id;
    ANX7150_in_pix_rpt = input_pixel_rpt_time;
}

void ANX7150_API_Packets_Config(UINT32 pkt_sel)
{
    s_ANX7150_packet_config.packets_need_config = pkt_sel;
}

void ANX7150_API_AVI_Config(UINT8 pb1,UINT8 pb2,UINT8 pb3,UINT8 pb4,UINT8 pb5,
    UINT8 pb6,UINT8 pb7,UINT8 pb8,UINT8 pb9,UINT8 pb10,UINT8 pb11,UINT8 pb12,UINT8 pb13)
{
    s_ANX7150_packet_config.avi_info.pb_byte[1] = pb1;
    s_ANX7150_packet_config.avi_info.pb_byte[2] = pb2;
    s_ANX7150_packet_config.avi_info.pb_byte[3] = pb3;
    s_ANX7150_packet_config.avi_info.pb_byte[4] = pb4;
    s_ANX7150_packet_config.avi_info.pb_byte[5] = pb5;
    s_ANX7150_packet_config.avi_info.pb_byte[6] = pb6;
    s_ANX7150_packet_config.avi_info.pb_byte[7] = pb7;
    s_ANX7150_packet_config.avi_info.pb_byte[8] = pb8;
    s_ANX7150_packet_config.avi_info.pb_byte[9] = pb9;
    s_ANX7150_packet_config.avi_info.pb_byte[10] = pb10;
    s_ANX7150_packet_config.avi_info.pb_byte[11] = pb11;
    s_ANX7150_packet_config.avi_info.pb_byte[12] = pb12;
    s_ANX7150_packet_config.avi_info.pb_byte[13] = pb13;
}

void ANX7150_API_AUD_INFO_Config(UINT8 pb1,UINT8 pb2,UINT8 pb3,UINT8 pb4,UINT8 pb5,
    UINT8 pb6,UINT8 pb7,UINT8 pb8,UINT8 pb9,UINT8 pb10)
{
    s_ANX7150_packet_config.audio_info.pb_byte[1] = pb1;
    s_ANX7150_packet_config.audio_info.pb_byte[2] = pb2;
    s_ANX7150_packet_config.audio_info.pb_byte[3] = pb3;
    s_ANX7150_packet_config.audio_info.pb_byte[4] = pb4;
    s_ANX7150_packet_config.audio_info.pb_byte[5] = pb5;
    s_ANX7150_packet_config.audio_info.pb_byte[6] = pb6;
    s_ANX7150_packet_config.audio_info.pb_byte[7] = pb7;
    s_ANX7150_packet_config.audio_info.pb_byte[8] = pb8;
    s_ANX7150_packet_config.audio_info.pb_byte[9] = pb9;
    s_ANX7150_packet_config.audio_info.pb_byte[10] = pb10;
}


void ANX7150_API_AUD_CHStatus_Config(UINT8 MODE,UINT8 PCM_MODE,UINT8 SW_CPRGT,UINT8 NON_PCM,
    UINT8 PROF_APP,UINT8 CAT_CODE,UINT8 CH_NUM,UINT8 SOURCE_NUM,UINT8 CLK_ACCUR,UINT8 Fs)
{
    //MODE: 0x00 = PCM Audio
    //PCM_MODE: 0x00 = 2 audio channels without pre-emphasis;
                                //0x01 = 2 audio channels with 50/15 usec pre-emphasis;
    //SW_CPRGT: 0x00 = copyright is asserted;
                                // 0x01 = copyright is not asserted;
    //NON_PCM: 0x00 = Represents linear PCM
                            //0x01 = For other purposes
    //PROF_APP: 0x00 = consumer applications;
                              // 0x01 = professional applications;

    //CAT_CODE: Category code
    //CH_NUM: 0x00 = Do not take into account
                           // 0x01 = left channel for stereo channel format
                           // 0x02 = right channel for stereo channel format
    //SOURCE_NUM: source number
                                   // 0x00 = Do not take into account
                                  // 0x01 = 1; 0x02 = 2; 0x03 = 3
    //CLK_ACCUR: 0x00 = level II
                                   // 0x01 = level I
                                   // 0x02 = level III
                                   // else reserved;

    s_ANX7150_audio_config.i2s_config.Channel_status1 = (MODE << 7) | (PCM_MODE << 5) |
        (SW_CPRGT << 2) | (NON_PCM << 1) | PROF_APP;
    s_ANX7150_audio_config.i2s_config.Channel_status2 = CAT_CODE;
    s_ANX7150_audio_config.i2s_config.Channel_status3 = (CH_NUM << 7) | SOURCE_NUM;
    s_ANX7150_audio_config.i2s_config.Channel_status4 = (CLK_ACCUR << 5) | Fs;
}


void ANX7150_API_System_Config(void)
{

    ANX7150_API_Video_Config(HDMI_1280X720P60, input_pixel_clk_1x_repeatition);
    ANX7150_API_Packets_Config(ANX7150_avi_sel | ANX7150_audio_sel);
    if(s_ANX7150_packet_config.packets_need_config & ANX7150_avi_sel)
        ANX7150_API_AVI_Config(YCbCr422,source_ratio,null,null,null,null,null,null,null,null,null,null,null);
    if(s_ANX7150_packet_config.packets_need_config & ANX7150_audio_sel)
        ANX7150_API_AUD_INFO_Config(null,null,null,null,null,null,null,null,null,null);
    ANX7150_API_AUD_CHStatus_Config(null,null,null,null,null,null,null,null,null,null);

}


void ANX7150_initHardware(void)
{
    UINT32 Reg;

    hdmitx_setHwReset(HwResetPin);

    // Power On DDC
    ANX7150_set_p0_bit(ANX7150_SYS_CTRL3_REG, ANX7150_SYS_CTRL3_I2C_PWON);
    ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, 0x00);
    ANX7150_i2c_write_p0_reg(ANX7150_SRST_REG,      0x00);

    //clear HDCP_HPD_RST
    ANX7150_clear_p0_bit(ANX7150_SYS_CTRL1_REG,ANX7150_SYS_CTRL1_HDCPHPD_RST);

    //Power on Audio capture and Video capture module clock
    ANX7150_set_p0_bit(ANX7150_SYS_PD_REG, ANX7150_SYS_PD_IDCK|ANX7150_SYS_PD_AUD);//0x06

    //Initial Interrupt
    //Disable video/audio CLK,Format change and before config video. 060713 xy
    ANX7150_i2c_write_p0_reg(ANX7150_INTR1_MASK_REG, 0x0c);//3
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_INTR1_MASK_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_INTR2_MASK_REG, 0x9f);
    ANX7150_i2c_write_p0_reg(ANX7150_INTR3_MASK_REG, 0x1a);

    //Enable auto set clock range for video PLL
    ANX7150_clear_p0_bit(ANX7150_CHIP_CTRL_REG, ANX7150_CHIP_CTRL_PLL_MAN);

    //Set registers value of Blue Screen when HDCP authentication failed--RGB mode,green field
    ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN0_REG, 0x10);
    ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN1_REG, 0xeb);
    ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN2_REG, 0x10);


    ANX7150_set_p0_bit(ANX7150_TMDS_CLKCH_CONFIG_REG, ANX7150_TMDS_CLKCH_MUTE);
    //ANX7150_i2c_read_p0_reg(ANX7150_PLL_CTRL0_REG, &c);
    ANX7150_i2c_write_p0_reg(ANX7150_PLL_CTRL0_REG, 0xA8);//(c & 0xf3));//wen change to 0xa8
    ANX7150_set_p0_bit(ANX7150_PLL_TX_AMP, 0x01);

    ANX7150_clear_p0_bit(ANX7150_I2S_CTRL_REG, SPDIF_IN_SEL);

    /* Go to Wait Hotplug State. */

}



void ANX7150_Set_AVMute(void)
{

    ANX7150_i2c_write_p1_reg(ANX7150_GNRL_CTRL_PKT_REG, ANX7150_GNRL_CTRL_SET_AVMUTE);//wen

    ANX7150_set_p1_bit(ANX7150_INFO_PKTCTRL1_REG, ANX7150_INFO_PKTCTRL1_GCP_RPT|ANX7150_INFO_PKTCTRL1_GCP_EN);//0x0C
}

void ANX7150_Clear_AVMute(void)
{
    ANX7150_i2c_write_p1_reg(ANX7150_GNRL_CTRL_PKT_REG, ANX7150_GNRL_CTRL_CLR_AVMUTE);

    ANX7150_set_p1_bit(ANX7150_INFO_PKTCTRL1_REG, ANX7150_INFO_PKTCTRL1_GCP_RPT|ANX7150_INFO_PKTCTRL1_GCP_EN);//0x0C
}

void ANX7150_RST_DDCChannel(void)
{
    //Reset the DDC channel
    ANX7150_set_p0_bit(ANX7150_SYS_CTRL2_REG, ANX7150_SYS_CTRL2_DDC_RST);
    ANX7150_clear_p0_bit(ANX7150_SYS_CTRL2_REG, ANX7150_SYS_CTRL2_DDC_RST);
    //ANX7150_i2c_read_p0_reg(ANX7150_SYS_CTRL2_REG, &c);
    //ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, (c | ANX7150_SYS_CTRL2_DDC_RST));
    //ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, (c & (~ANX7150_SYS_CTRL2_DDC_RST)));

    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACC_CMD_REG, 0x00);    //abort current operation
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACC_CMD_REG, 0x06);    //reset I2C command

    //Clear FIFO
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACC_CMD_REG, 0x05);
}

void ANX7150_Clean_HDCP(void)
{
    //mute TMDS link
    ANX7150_clear_p0_bit(ANX7150_TMDS_CLKCH_CONFIG_REG, ANX7150_TMDS_CLKCH_MUTE);
    //ANX7150_i2c_read_p0_reg(ANX7150_TMDS_CLKCH_CONFIG_REG, &c);
    //ANX7150_i2c_write_p0_reg(ANX7150_TMDS_CLKCH_CONFIG_REG, c & (~ANX7150_TMDS_CLKCH_MUTE));

    //Disable hardware HDCP
    ANX7150_clear_p0_bit(ANX7150_HDCP_CTRL0_REG, ANX7150_HDCP_CTRL0_HW_AUTHEN);
    //ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL0_REG, &c);
    //ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, (c & (~ANX7150_HDCP_CTRL0_HW_AUTHEN)));

    //Reset HDCP logic
    ANX7150_set_p0_bit(ANX7150_SRST_REG, ANX7150_SRST_HDCP_RST);
    ANX7150_clear_p0_bit(ANX7150_SRST_REG, ANX7150_SRST_HDCP_RST);
    //ANX7150_i2c_read_p0_reg(ANX7150_SRST_REG, &c);
    //ANX7150_i2c_write_p0_reg(ANX7150_SRST_REG, (c | ANX7150_SRST_HDCP_RST) );
    //ANX7150_i2c_write_p0_reg(ANX7150_SRST_REG, (c & (~ANX7150_SRST_HDCP_RST)) );

    //Set ReAuth
    ANX7150_set_p0_bit(ANX7150_HDCP_CTRL0_REG, ANX7150_HDCP_CTRL0_RE_AUTH);
    ANX7150_clear_p0_bit(ANX7150_HDCP_CTRL0_REG, ANX7150_HDCP_CTRL0_RE_AUTH);
    //ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL0_REG, &c);
    //ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, c |ANX7150_HDCP_CTRL0_RE_AUTH);
    //ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, c & (~ANX7150_HDCP_CTRL0_RE_AUTH));

    //ANX7150_RST_DDCChannel();
}


void ANX7150_HotplugChange(BOOL Plug)
{
    UINT32 Reg;

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_STATE_REG);

    if (Reg & ANX7150_SYS_STATE_HP)
    {
        anx7150_msg(("Detect Hotplug in ANX7150! \r\n\r\n"));
    }
    else
    {
        debug_err(("Hotplug detach in ANX7150_open ERR!! \r\n"));
    }


    if(Plug)
    {
        ANX7150_clear_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);
        ANX7150_clear_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_IN_EN);
        ANX7150_clear_p0_bit(ANX7150_TMDS_CLKCH_CONFIG_REG, ANX7150_TMDS_CLKCH_MUTE);
        ANX7150_clear_p0_bit(ANX7150_HDCP_CTRL0_REG, ANX7150_HDCP_CTRL0_HW_AUTHEN);

        /*End Init State*/

        /*Go to Parse EDID State*/

        //
        //Power on chip and select DVI mode
        //
        ANX7150_set_p0_bit(ANX7150_SYS_CTRL1_REG, ANX7150_SYS_CTRL1_PWDN_CTRL);
        ANX7150_clear_p0_bit(ANX7150_SYS_CTRL1_REG, ANX7150_SYS_CTRL1_HDMI);
    }
    else
    {
        //Power down chip
        ANX7150_clear_p0_bit(ANX7150_SYS_CTRL1_REG, ANX7150_SYS_CTRL1_PWDN_CTRL);
        ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, 0x00);
    }


}

void ANX7150_Reset_AVI(void)
{
    ANX7150_avi_data[0] = 0x82; //type, HDMI defined
    ANX7150_avi_data[1] = 0x02; //Version
    ANX7150_avi_data[2] = 0x0d; //AVI data byte length is 13
    ANX7150_avi_data[3] = 0x00; //checksum
    ANX7150_avi_data[4] = 0x00; //data byte 1
    ANX7150_avi_data[5] = 0x08; //data byte 2
    ANX7150_avi_data[6] = 0x00; //data byte 3
    ANX7150_avi_data[7] = 0x01; //data byte 4
    ANX7150_avi_data[8] = 0x00; //data byte 5
    ANX7150_avi_data[9] = 0x00; //data byte 6
    ANX7150_avi_data[10] = 0x00; //data byte 7
    ANX7150_avi_data[11] = 0x00; //data byte 8
    ANX7150_avi_data[12] = 0x00; //data byte 9
    ANX7150_avi_data[13] = 0x00; //data byte 10
    ANX7150_avi_data[14] = 0x00; //data byte 11
    ANX7150_avi_data[15] = 0x00; //data byte 12
    ANX7150_avi_data[16] = 0x00; //data byte 13
    ANX7150_avi_data[17] = 0x00; //for firmware use
    ANX7150_avi_data[18] = 0x00; //for firmware use
}

void ANX7150_Variable_Initial(void)
{

    //
    //  Novatek Added
    //
    memset(SVD_ID, 0x00, 34);
    guiHdmiEdidInfo = 0x00;



    //
    //  Analogix HDMI Driver Parameters Initialize
    //

    /*
    ANX7150_Set_System_State(ANX7150_INITIAL);
    ANX7150_hdcp_auth_en = 0;
    ANX7150_ksv_srm_pass =0;
    ANX7150_srm_checked = 0;
    ANX7150_hdcp_auth_pass = 0;
    ANX7150_avmute_enable = 1;
    ANX7150_hdcp_auth_fail_counter =0;
    ANX7150_hdcp_encryption = 0;
    ANX7150_send_blue_screen = 0;
    ANX7150_hdcp_init_done = 0;
    ANX7150_hdcp_wait_100ms_needed = 1;
    ANX7150_auth_fully_pass = 0;
    */
    //********************for video config**************
    ANX7150_video_timing_id = 0;
    ANX7150_in_pix_rpt = 0;
    ANX7150_tx_pix_rpt = 0;
    /*ANX7150_new_csc = 0;
    ANX7150_new_vid_id = 0;
    ANX7150_new_HW_interface = 0;*/
    //********************end of video config*********

    //********************for edid parse***********
    /*
    ANX7150_edid_result.is_HDMI = 0;
    ANX7150_edid_result.ycbcr422_supported = 0;
    ANX7150_edid_result.ycbcr444_supported = 0;
    ANX7150_edid_result.supported_720p_60Hz = 0;
    ANX7150_edid_result.supported_720p_50Hz = 0;
    ANX7150_edid_result.supported_576p_50Hz = 0;
    ANX7150_edid_result.supported_576i_50Hz = 0;
    ANX7150_edid_result.supported_1080i_60Hz = 0;
    ANX7150_edid_result.supported_1080i_50Hz = 0;
    ANX7150_edid_result.supported_640x480p_60Hz = 0;
    ANX7150_edid_result.supported_720x480p_60Hz = 0;
    ANX7150_edid_result.supported_720x480i_60Hz = 0;
    ANX7150_edid_result.edid_errcode = 0;
    ANX7150_edid_result.SpeakerFormat = 0;
    for(i = 0; i < 8; i ++)
    {
        ANX7150_edid_result.AudioChannel[i] = 0;
        ANX7150_edid_result.AudioFormat[i] = 0;
        ANX7150_edid_result.AudioFs[i] = 0;
        ANX7150_edid_result.AudioLength[i] = 0;
    }
    */
    //********************end of edid**************

    s_ANX7150_packet_config.packets_need_config  = 0x03;   //new avi infoframe
    s_ANX7150_packet_config.avi_info.type        = 0x82;
    s_ANX7150_packet_config.avi_info.version     = 0x02;
    s_ANX7150_packet_config.avi_info.length      = 0x0d;
    s_ANX7150_packet_config.avi_info.pb_byte[1]  = 0x21;//YCbCr422
    s_ANX7150_packet_config.avi_info.pb_byte[2]  = 0x08;
    s_ANX7150_packet_config.avi_info.pb_byte[3]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[4]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[5]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[6]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[7]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[8]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[9]  = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[10] = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[11] = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[12] = 0x00;
    s_ANX7150_packet_config.avi_info.pb_byte[13] = 0x00;

    // audio infoframe
    s_ANX7150_packet_config.audio_info.type        = 0x84;
    s_ANX7150_packet_config.audio_info.version     = 0x01;
    s_ANX7150_packet_config.audio_info.length      = 0x0a;
    s_ANX7150_packet_config.audio_info.pb_byte[1]  = 0x00;  //zy 061123 for ATC
    s_ANX7150_packet_config.audio_info.pb_byte[2]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[3]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[4]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[5]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[6]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[7]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[8]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[9]  = 0x00;
    s_ANX7150_packet_config.audio_info.pb_byte[10] = 0x00;

    ANX7150_Reset_AVI();

    s_ANX7150_audio_config.audio_type = ANX7150_AUD_HW_INTERFACE;     // input I2S
    s_ANX7150_audio_config.down_sample = 0x00;
    s_ANX7150_audio_config.i2s_config.audio_channel = ((ANX7150_I2S_CH0_ENABLE << 2) | (ANX7150_I2S_CH1_ENABLE << 3) |
        (ANX7150_I2S_CH2_ENABLE << 4) | (ANX7150_I2S_CH3_ENABLE << 5));//0x04;
    s_ANX7150_audio_config.i2s_config.Channel_status1 =0x00;
    s_ANX7150_audio_config.i2s_config.Channel_status1 = 0x00;
    s_ANX7150_audio_config.i2s_config.Channel_status2 = 0x00;
    s_ANX7150_audio_config.i2s_config.Channel_status3 = 0x00;
    s_ANX7150_audio_config.i2s_config.Channel_status4 = 0x00;//0x02;//48k
    s_ANX7150_audio_config.i2s_config.Channel_status5 = ANX7150_I2S_WORD_LENGTH;//0x0b;
    s_ANX7150_audio_config.i2s_config.i2s_format = ((ANX7150_I2S_SHIFT_CTRL << 3) | (ANX7150_I2S_DIR_CTRL << 2)  |
          (ANX7150_I2S_WS_POL << 1) | ANX7150_I2S_JUST_CTRL);//0x00;

    FREQ_MCLK = ANX7150_MCLK_Fs_RELATION;//set the relation of MCLK and WS
    ANX7150_audio_clock_edge = ANX7150_AUD_CLK_EDGE;

}



#if 1

void ANX7150_InitDDC_Read(UINT32 devaddr, UINT32 segmentpointer, UINT32 offset, UINT32  access_num_Low,UINT32 access_num_high)
{
    //Write slave device address
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_SLV_ADDR_REG, devaddr);

    // Write segment address
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_SLV_SEGADDR_REG, segmentpointer);

    //Write offset
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_SLV_OFFADDR_REG, offset);

    //Write number for access
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACCNUM0_REG, access_num_Low);
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACCNUM1_REG, access_num_high);

    //Clear FIFO
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACC_CMD_REG, 0x05);

    //EDDC sequential Read
    ANX7150_i2c_write_p0_reg(ANX7150_DDC_ACC_CMD_REG, 0x04);
}


UINT32 ANX7150_Read_EDID_BYTE(UINT32 segmentpointer,UINT32 offset)
{
    ANX7150_InitDDC_Read(0xA0, segmentpointer, offset, 0x01, 0x00);
    TimerDelayMs(3);

    return ANX7150_i2c_read_p0_reg(ANX7150_DDC_FIFO_ACC_REG);
}


void ANX7150_GetEDIDLength()
{
    UINT32 edid_data_length;

    ANX7150_RST_DDCChannel();

    edid_data_length = ANX7150_Read_EDID_BYTE(0x00, 0x7e);
    //debug_puts("Finish reading EDID block number.\n");

    ANX7150_edid_length = edid_data_length * 128 + 128;
}

void ANX7150_DDC_Mass_Read(UINT32 length)
{
    UINT32  i, j;
    UINT32  c, c1, ddc_empty_cnt;
    BOOL    ANX7150_ddc_fifo_full, ANX7150_ddc_progress;

    i = length;

    while (i > 0)
    {
        //check DDC FIFO statue
        c = ANX7150_i2c_read_p0_reg(ANX7150_DDC_CHSTATUS_REG);
        if(c & ANX7150_DDC_CHSTATUS_DDC_OCCUPY)
        {
            break;
        }

        if(c & ANX7150_DDC_CHSTATUS_FIFO_FULL)
            ANX7150_ddc_fifo_full = 1;
        else
            ANX7150_ddc_fifo_full = 0;

        if(c & ANX7150_DDC_CHSTATUS_INPRO)
            ANX7150_ddc_progress = 1;
        else
            ANX7150_ddc_progress = 0;


        if(ANX7150_ddc_fifo_full)
        {
            c = ANX7150_i2c_read_p0_reg(ANX7150_DDC_FIFOCNT_REG);

            i = i - c;
            for(j=0; j<c; j++)
            {
                c1 = ANX7150_i2c_read_p0_reg(ANX7150_DDC_FIFO_ACC_REG);
                ANX7150_ddc_fifo_full = 0;
            }
        }
        else if(!ANX7150_ddc_progress)
        {
            c = ANX7150_i2c_read_p0_reg(ANX7150_DDC_FIFOCNT_REG);
            if(!c)
            {
                i =0;
                break;
            }
            i = i - c;
            for(j=0; j<c; j++)
            {
                c1 = ANX7150_i2c_read_p0_reg(ANX7150_DDC_FIFO_ACC_REG);
            }
        }
        else
        {
            ddc_empty_cnt = 0x00;
            for(c1=0; c1<0x0a; c1++)
            {
                c = ANX7150_i2c_read_p0_reg(ANX7150_DDC_CHSTATUS_REG);

                if(c & ANX7150_DDC_CHSTATUS_FIFO_EMPT)
                    ddc_empty_cnt++;

                TimerDelayMs(5);
            }
            if(ddc_empty_cnt >= 0x0A)
                i = 0;
        }
    }
}


void ANX7150_Read_EDID(void)
{
    UINT32 edid_segment,segmentpointer,k;

    ANX7150_RST_DDCChannel();

    edid_segment = ANX7150_edid_length / 256;

    if(edid_segment==0)                                                                         //update
        segmentpointer =0;
    else
        segmentpointer = edid_segment - 1;


    for(k = 0; k <= segmentpointer; k ++)
    {
        ANX7150_InitDDC_Read(0xA0, k, 0x00, 0x80, 0x00);
        ANX7150_DDC_Mass_Read(128);
        ANX7150_InitDDC_Read(0xA0, k, 0x80, 0x80, 0x00);
        ANX7150_DDC_Mass_Read(128);
    }

    if((ANX7150_edid_length - 256 * edid_segment) == 0)
    {
        anx7150_msg(("Finish reading EDID\r\n"));
    }
    else
    {
        anx7150_msg(("Read one more block(128 bytes).........\r\n"));

        ANX7150_InitDDC_Read(0xA0, segmentpointer + 1, 0x00, 0x80, 0x00);
        ANX7150_DDC_Mass_Read(128);

        anx7150_msg(("Finish reading EDID\r\n"));
    }
}


BOOL ANX7150_Parse_EDIDHeader(void)
{
    UINT32 i,temp;

    temp = 0;
    // the EDID header should begin with 0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00
    if((ANX7150_Read_EDID_BYTE(0, 0) == 0x00) && (ANX7150_Read_EDID_BYTE(0, 7) == 0x00))
    {
        for(i = 1; i < 7; i++)
        {
            if(ANX7150_Read_EDID_BYTE(0, i) != 0xff)
            {
                temp = 0x01;
                break;
            }
        }
    }
    else
    {
        temp = 0x01;
    }

    if(temp == 0x01)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

BOOL ANX7150_Parse_EDIDVersion(void)
{

    if(!((ANX7150_Read_EDID_BYTE(0, 0x12) == 1) && (ANX7150_Read_EDID_BYTE(0, 0x13) >= 3) ))
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

BOOL ANX7150_EDID_Checksum(UINT32 block_number)
{
    UINT8    i, real_checksum;
    UINT8    edid_block_checksum;

    edid_block_checksum = 0;

    for(i = 0; i < 127; i ++)
    {
        if((block_number / 2) * 2 == block_number)
            edid_block_checksum = edid_block_checksum + ANX7150_Read_EDID_BYTE(block_number/2, i);
        else
            edid_block_checksum = edid_block_checksum + ANX7150_Read_EDID_BYTE(block_number/2, i + 0x80);
    }

    edid_block_checksum = (~edid_block_checksum) + 1;

    if((block_number / 2) * 2 == block_number)
        real_checksum = ANX7150_Read_EDID_BYTE(block_number/2, 0x7f);
    else
        real_checksum = ANX7150_Read_EDID_BYTE(block_number/2, 0xff);

    if(real_checksum == edid_block_checksum)
        return 1;
    else
        return 0;

}

void ANX7150_Parse_DTD()
{
    UINT32 temp;
    UINT32 temp1,temp2;
    UINT32 Hresolution,Vresolution,Hblanking,Vblanking;
    UINT32 PixelCLK,Vtotal,H_image_size,V_image_size;
    UINT32 Hz;


    temp = ANX7150_edid_dtd[1];
    temp = temp << 8;
    PixelCLK = temp + ANX7150_edid_dtd[0];
    anx7150_msg(("Pixel clock is 10000 * %u\n\r",  temp));

    temp = ANX7150_edid_dtd[4];
    temp = (temp << 4) & 0x0f00;
    Hresolution = temp + ANX7150_edid_dtd[2];
    anx7150_msg(("Horizontal Active is  %u\n\r",  Hresolution));

    temp = ANX7150_edid_dtd[4];
    temp = (temp << 8) & 0x0f00;
    Hblanking = temp + ANX7150_edid_dtd[3];
    anx7150_msg(("Horizontal Blanking is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[7];
    temp = (temp << 4) & 0x0f00;
    Vresolution = temp + ANX7150_edid_dtd[5];
    anx7150_msg(("Vertical Active is  %u\n\r",  Vresolution));

    temp = ANX7150_edid_dtd[7];
    temp = (temp << 8) & 0x0f00;
    Vblanking = temp + ANX7150_edid_dtd[6];
    anx7150_msg(("Vertical Blanking is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[11];
    temp = (temp << 2) & 0x0300;
    temp = temp + ANX7150_edid_dtd[8];
    anx7150_msg(("Horizontal Sync Offset is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[11];
    temp = (temp << 4) & 0x0300;
    temp = temp + ANX7150_edid_dtd[9];
    anx7150_msg(("Horizontal Sync Pulse is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[11];
    temp = (temp << 2) & 0x0030;
    temp = temp + (ANX7150_edid_dtd[10] >> 4);
    anx7150_msg(("Vertical Sync Offset is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[11];
    temp = (temp << 4) & 0x0030;
    temp = temp + (ANX7150_edid_dtd[8] & 0x0f);
    anx7150_msg(("Vertical Sync Pulse is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[14];
    temp = (temp << 4) & 0x0f00;
    H_image_size = temp + ANX7150_edid_dtd[12];
    anx7150_msg(("Horizontal Image size is  %u\n\r",  temp));

    temp = ANX7150_edid_dtd[14];
    temp = (temp << 8) & 0x0f00;
    V_image_size = temp + ANX7150_edid_dtd[13];
    anx7150_msg(("Vertical Image size is  %u\n\r",  temp));

    anx7150_msg(("Horizontal Border is  %u\n\r",  ANX7150_edid_dtd[15]));

    anx7150_msg(("Vertical Border is  %u\n\r",  ANX7150_edid_dtd[16]));


    if(ANX7150_edid_dtd[17] & 0x80)
        anx7150_msg((" Interlaced\n"));
    else
        anx7150_msg(("Non-interlaced\n\r"));

    if(ANX7150_edid_dtd[17] & 0x60)
        anx7150_msg(("Table 3.17 for defenition\n\r"));
    else
        anx7150_msg(("Normal Display\n\r"));

    if(ANX7150_edid_dtd[17] & 0x10)
        anx7150_msg((" Digital\n\r"));
    else
        anx7150_msg((" Analog\n\r"));


    temp1 = Hresolution + Hblanking;
    Vtotal = Vresolution + Vblanking;
    temp1 = temp1 * Vtotal;
    temp2 = PixelCLK;
    temp2 = temp2 * 10000;

    if (temp1 == 0)                                                                                                             //update
        Hz = 0;
    else
        Hz = temp2 / temp1;

    //Hz = temp2 / temp1;
    if((Hz == 59) || (Hz == 60))
    {
        Hz = 60;
        anx7150_msg(("_______________Vertical Active is  %u\n\r",  Vresolution));
        if(Vresolution == 540)
        {
            guiHdmiEdidInfo |= HDMITX_DTD1080I60;
            anx7150_msg(("supported_1080i_60Hz \r\n"));
        }
        if(Vresolution == 1080)
        {
            guiHdmiEdidInfo |= HDMITX_DTD1080P60;
            anx7150_msg(("supported_1080p_60Hz \r\n"));
        }
        if(Vresolution == 720)
        {
            guiHdmiEdidInfo |= HDMITX_DTD720P60;
            anx7150_msg(("supported_720p_60Hz \r\n"));
        }
        if((Hresolution == 640) && (Vresolution == 480))
        {
            guiHdmiEdidInfo |= HDMITX_DTD640X480P60;
            anx7150_msg(("supported_640x480p_60Hz \r\n"));
        }
        if((Hresolution == 720) && (Vresolution == 480))
        {
            guiHdmiEdidInfo |= HDMITX_DTD720X480P60;
            anx7150_msg(("supported_720x480p_60Hz \r\n"));
        }
        if((Hresolution == 720) && (Vresolution == 240))
        {
            guiHdmiEdidInfo |= HDMITX_DTD720X480I60;
            anx7150_msg(("supported_720x480i_60Hz \r\n"));
        }
    }
    if(Hz == 50)
    {
        anx7150_msg(("+++++++++++++++Vertical Active is  %u\n\r",  Vresolution));
        if(Vresolution == 540)
        {
            guiHdmiEdidInfo |= HDMITX_DTD1080I50;
            anx7150_msg(("supported_1080i_50Hz \r\n"));
        }
        if(Vresolution == 1080)
        {
            guiHdmiEdidInfo |= HDMITX_DTD1080P50;
            anx7150_msg(("supported_1080p_50Hz \r\n"));
        }
        if(Vresolution == 720)
        {
            guiHdmiEdidInfo |= HDMITX_DTD720P50;
            anx7150_msg(("supported_720p_50Hz \r\n"));
        }
        if(Vresolution == 576)
        {
            guiHdmiEdidInfo |= HDMITX_DTD576P50;
            anx7150_msg(("supported_576p_50Hz \r\n"));
        }
        if(Vresolution == 288)
        {
            guiHdmiEdidInfo |= HDMITX_DTD576I50;
            anx7150_msg(("supported_576i_50Hz \r\n"));
        }
    }
    anx7150_msg(("Fresh rate :%u Hz\n\r", Hz));

}

void ANX7150_Parse_DTDinBlockONE(void)
{
    UINT32 i;

    for(i = 0; i < 18; i++)
    {
        ANX7150_edid_dtd[i] = ANX7150_Read_EDID_BYTE(0, (i + 0x36));
    }
    anx7150_msg(("Parse the first DTD in Block one:\n\r"));
    ANX7150_Parse_DTD();

    if((ANX7150_Read_EDID_BYTE(0, 0x48) == 0)
    && (ANX7150_Read_EDID_BYTE(0, 0x49) == 0)
    && (ANX7150_Read_EDID_BYTE(0, 0x4A) == 0))
    {
        anx7150_msg(("the second DTD in Block one is not used to descript video timing.\n\r"));
    }
    else
    {
        for(i = 0; i < 18; i++)
        {
            ANX7150_edid_dtd[i] = ANX7150_Read_EDID_BYTE(0, (i + 0x48));
        }
        ANX7150_Parse_DTD();
    }

    if((ANX7150_Read_EDID_BYTE(0,0x5A) == 0)
    && (ANX7150_Read_EDID_BYTE(0,0x5B) == 0)
    && (ANX7150_Read_EDID_BYTE(0,0x5C) == 0))
    {
        anx7150_msg(("the third DTD in Block one is not used to descript video timing.\n\r"));
    }
    else
    {
        for(i = 0; i < 18; i++)
        {
            ANX7150_edid_dtd[i] = ANX7150_Read_EDID_BYTE(0, (i + 0x5A));
        }
        ANX7150_Parse_DTD();
    }

    if((ANX7150_Read_EDID_BYTE(0,0x6C) == 0)
    && (ANX7150_Read_EDID_BYTE(0,0x6D) == 0)
    && (ANX7150_Read_EDID_BYTE(0,0x6E) == 0))
    {
        anx7150_msg(("the fourth DTD in Block one is not used to descript video timing.\n\r"));
    }
    else
    {
        for(i = 0; i < 18; i++)
        {
            ANX7150_edid_dtd[i] = ANX7150_Read_EDID_BYTE(0,(i + 0x6C));
        }

        ANX7150_Parse_DTD();
    }
}

void ANX7150_Parse_NativeFormat(void)
{
    UINT32 temp;

    temp = ANX7150_Read_EDID_BYTE(0,0x83) & 0xF0;

    if(temp & 0x80)
    {
        #if ANX7150_UNDERSCAN_HANDLE
        guiHdmiEdidInfo |= HDMITX_TV_UNDERSCAN;
        #endif
        anx7150_msg(("DTV supports underscan.\n\r"));
    }

    if(temp & 0x40)
    {
        anx7150_msg(("DTV supports BasicAudio.\n\r"));
    }

    if(temp & 0x20)
    {
        anx7150_msg(("DTV supports YCbCr 4:4:4.\n\r"));
        guiHdmiEdidInfo |= HDMITX_TV_YCBCR444;
    }

    if(temp & 0x10)
    {
        anx7150_msg(("DTV supports YCbCr 4:2:2.\n\r"));
        guiHdmiEdidInfo |= HDMITX_TV_YCBCR422;
    }

}


void ANX7150_Parse_DTDinExtBlock(void)
{
    UINT8 i,DTDbeginAddr;

    DTDbeginAddr = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2, 0x82) + 0x80;

    while(DTDbeginAddr < (0x6C + 0x80))
    {
        if((ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,DTDbeginAddr) == 0)
        && (ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(DTDbeginAddr + 1)) == 0)
        && (ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(DTDbeginAddr + 2)) == 0))
        {
            anx7150_msg(("this DTD in Extension Block is not used to descript video timing.\n\r"));
        }
        else
        {
            for(i = 0; i < 18; i++)
            {
                ANX7150_edid_dtd[i] = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(i + DTDbeginAddr));
            }
            anx7150_msg(("Parse the DTD in Extension Block :\n\r"));
            ANX7150_Parse_DTD();
        }
        DTDbeginAddr = DTDbeginAddr + 18;
    }
}


void ANX7150_Parse_AudioSTD(void)
{
    UINT32 Reg;
    UINT32 i,AudioFormat,STDReg_tmp,STDAddr_tmp;


    STDReg_tmp = ANX7150_stdreg & 0x1f;
    STDAddr_tmp = ANX7150_stdaddr + 1;
    i = 0;

    while(i < STDReg_tmp)
    {
        AudioFormat = (ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,STDAddr_tmp ) & 0xF8) >> 3;

        Reg = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,STDAddr_tmp+1);

        if ( Reg & 0x01)
        {
            anx7150_msg(("TV Audio Support 32Khz sampling rate! \r\n"));
            guiHdmiEdidInfo |= HDMITX_ADO_32KHZ;
        }
        if ( Reg & 0x02)
        {
            anx7150_msg(("TV Audio Support 44.1Khz sampling rate! \r\n"));
            guiHdmiEdidInfo |= HDMITX_ADO_44KHZ;
        }
        if ( Reg & 0x04)
        {
            anx7150_msg(("TV Audio Support 48Khz sampling rate! \r\n"));
            guiHdmiEdidInfo |= HDMITX_ADO_48KHZ;
        }
        if ( Reg & 0x78)
        {
            anx7150_msg(("TV Audio Support sampling rate over 48Khz! \r\n"));
            guiHdmiEdidInfo |= HDMITX_ADO_MORE;
        }

        if(AudioFormat == 1)
        {

            Reg = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2, STDAddr_tmp+2);
            if(Reg & 0x01)
                anx7150_msg(("TV Audio support 16bits \r\n"));
            if(Reg & 0x02)
                anx7150_msg(("TV Audio support 20bits \r\n"));
            if(Reg & 0x04)
                anx7150_msg(("TV Audio support 24bits \r\n"));
        }
        else
            debug_err(("Un-Known TV Audio Format Code! \r\n"));
            //ANX7150_edid_result.AudioLength[i/3] = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(STDAddr_tmp + 2)) << 3;

        i = i + 3;
        STDAddr_tmp = STDAddr_tmp + 3;
    }

}

void ANX7150_Parse_VideoSTD(void)
{
    UINT8    i, STDReg_tmp, STDAddr_tmp;


    STDReg_tmp  = ANX7150_stdreg & 0x1F;
    STDAddr_tmp = ANX7150_stdaddr + 1;
    i = 0;

    while(i < STDReg_tmp)
    {
        SVD_ID[i] = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2, STDAddr_tmp) & 0x7F;

        /*
        if(SVD_ID[i] == 1)
            ANX7150_edid_result.supported_640x480p_60Hz = 1;
        else if(SVD_ID[i] == 4)
            ANX7150_edid_result.supported_720p_60Hz = 1;
        else if(SVD_ID[i] == 19)
            ANX7150_edid_result.supported_720p_50Hz = 1;
        else if(SVD_ID[i] == 16)
            ANX7150_edid_result.supported_1080p_60Hz = 1;
        else if(SVD_ID[i] == 31)
            ANX7150_edid_result.supported_1080p_50Hz = 1;
        else if(SVD_ID[i] == 5)
            ANX7150_edid_result.supported_1080i_60Hz = 1;
        else if(SVD_ID[i] == 20)
            ANX7150_edid_result.supported_1080i_50Hz = 1;
        else if((SVD_ID[i] == 2) ||(SVD_ID[i] == 3))
            ANX7150_edid_result.supported_720x480p_60Hz = 1;
        else if((SVD_ID[i] == 6) ||(SVD_ID[i] == 7))
            ANX7150_edid_result.supported_720x480i_60Hz = 1;
        else if((SVD_ID[i] == 17) ||(SVD_ID[i] == 18))
            ANX7150_edid_result.supported_576p_50Hz = 1;
        else if((SVD_ID[i] == 21) ||(SVD_ID[i] == 22))
            ANX7150_edid_result.supported_576i_50Hz = 1;
        */

        i = i + 1;
        STDAddr_tmp = STDAddr_tmp + 1;
    }
}

void ANX7150_Parse_VendorSTD(void)
{
    //BYTE c;
    if((ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(ANX7150_stdaddr + 1)) == 0x03)
    && (ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(ANX7150_stdaddr + 2)) == 0x0c)
    && (ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2,(ANX7150_stdaddr + 3)) == 0x00))
    {
        anx7150_msg(("TV is HDMI Device! \r\n"));
        guiHdmiEdidInfo |= HDMITX_TV_HDMI;
    }
    else
    {
        //ANX7150_edid_result.is_HDMI = 0;
        anx7150_msg(("TV is DVI Device! \r\n"));
    }
}


void ANX7150_Parse_STD(void)
{
    UINT8    DTDbeginAddr;
    ANX7150_stdaddr = 0x84;

    DTDbeginAddr = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2, 0x82) + 0x80;

    while(ANX7150_stdaddr < DTDbeginAddr)
    {
        ANX7150_stdreg = ANX7150_Read_EDID_BYTE(ANX7150_ext_block_num/2, ANX7150_stdaddr);
        switch(ANX7150_stdreg & 0xE0)
        {
            case 0x20:
                ANX7150_Parse_AudioSTD();
                ANX7150_sau_length = ANX7150_stdreg & 0x1F;
                break;
            case 0x40:
               ANX7150_Parse_VideoSTD();
                ANX7150_svd_length = ANX7150_stdreg & 0x1F;
                break;
            //case 0x80:
            //    ANX7150_Parse_SpeakerSTD();
            //   break;
            case 0x60:
                ANX7150_Parse_VendorSTD();
                break;
            default:
                break;
        }

        ANX7150_stdaddr = ANX7150_stdaddr + (ANX7150_stdreg & 0x1F) + 0x01;
    }
}


UINT32 ANX7150_Parse_ExtBlock(void)
{
    UINT8 i,c;

    for(i = 0; i < ANX7150_Read_EDID_BYTE(0, 0x7E); i++)
    {
        c = ANX7150_Read_EDID_BYTE(i/2, 0x80);
        if( c == 0x02)
        {
            ANX7150_ext_block_num = i + 1;
            ANX7150_Parse_DTDinExtBlock();
            ANX7150_Parse_STD();
            if(!(ANX7150_EDID_Checksum(ANX7150_ext_block_num)))
            {
                anx7150_msg(("ExBlk CheckSum Error! (%d)\r\n",ANX7150_ext_block_num));
                return 1;
            }
        }
        else
        {
            anx7150_msg(("ANX7150_EDID_ExtBlock_NotFor_861B \r\n"));
            return 1;
        }
    }

    return 0;
}


BOOL ANX7150_Parse_EDID(void)
{
    ANX7150_GetEDIDLength();

    anx7150_msg(("EDIDLength is %.u \n\r",  ANX7150_edid_length));

    ANX7150_Read_EDID();

    if(!(ANX7150_Parse_EDIDHeader()))
    {
        anx7150_msg(("BAD EDID Header, Stop parsing \n\r"));
        return TRUE;
    }

    if(!(ANX7150_Parse_EDIDVersion()))
    {
        anx7150_msg(("EDID does not support 861B, Stop parsing\n\r"));
        return TRUE;
    }

    if(ANX7150_EDID_Checksum(0) == 0)
    {
        anx7150_msg(("EDID Block one check sum error, Stop parsing\n\r"));
        return TRUE;
    }

    ANX7150_Parse_DTDinBlockONE();

    if(ANX7150_Read_EDID_BYTE(0, 0x7E) == 0)
    {
        anx7150_msg(("No EDID extension blocks.\r\n"));
        return TRUE;
    }

    ANX7150_Parse_NativeFormat();
    ANX7150_Parse_ExtBlock();

    anx7150_msg(("EDID parsing finished!\n\r"));

    return FALSE;
}

#endif

#if 1
void ANX7150_Get_Video_Timing(void)
{
    UINT8  i;
    //#ifdef ITU656
    for(i = 0; i < 18; i++)
    {
        switch(ANX7150_video_timing_id)
        {
            case HDMI_640X480P60:
                //debug_puts("640x480p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_640x480p_60Hz[i];
                break;
            case HDMI_720X480P60:
            case HDMI_720X480P60_16X9:
                //debug_puts("720x480p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_720x480p_60Hz[i];
                break;
            case HDMI_1280X720P60:
                //debug_puts("1280x720p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_1280x720p_60Hz[i];
                break;
            case HDMI_1920X1080I60:
                //debug_puts("1920x1080i_60Hz!\n");
                ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_1920x1080i_60Hz[i];
                break;
            //case HDMI_720x480I60:
            //case HDMI_720x480i_60Hz_16x9:
                //debug_puts("720x480i_60Hz!\n");
            //    ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_720x480i_60Hz[i];
            //    break;
            case HDMI_720X576P50:
            case HDMI_720X576P50_16X9:
                //debug_puts("720x576p_50Hz!\n");
                ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_720x576p_50Hz[i];
                break;
            //case HDMI_1280x720p_50Hz:
                //debug_puts("1280x720p_50Hz!\n");
            //    ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_1280x720p_50Hz[i];
            //    break;
            //case HDMI_1920x1080i_50Hz:
                //debug_puts("1920x1080i_50Hz!\n");
            //    ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_1920x1080i_50Hz[i];
            //    break;
            //case HDMI_720x576i_50Hz_4x3:
            //case HDMI_720x576i_50Hz_16x9:
                //debug_puts("720x576i_50Hz!\n");
            //    ANX7150_video_timing_parameter[i] = ANX7150_video_timingtype_table.ANX7150_720x576i_50Hz[i];
            //    break;

            default:
                break;
        }
        //debug_printf("Video_Timing_Parameter[%.2x]=%.2x\n", (WORD)i, (WORD) ANX7150_video_timing_parameter[i]);
    }
    /*#else
    for(i = 0; i < 18; i++)
    {
        switch(ANX7150_video_timing_id)
        {
            case ANX7150_V640x480p_60Hz:
                //debug_puts("640x480p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, i);
                delay_ms(3);
                break;
            case ANX7150_V720x480p_60Hz_4x3:
            case ANX7150_V720x480p_60Hz_16x9:
                //debug_puts("720x480p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 18 + i);
                delay_ms(3);
                break;
            case ANX7150_V1280x720p_60Hz:
                //debug_puts("1280x720p_60Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 36 + i);
                delay_ms(3);
                break;
            case ANX7150_V1920x1080i_60Hz:
                //debug_puts("1920x1080i_60Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 54 + i);
                delay_ms(3);
                break;
            case ANX7150_V720x480i_60Hz_4x3:
            case ANX7150_V720x480i_60Hz_16x9:
                //debug_puts("720x480i_60Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 72 + i);
                delay_ms(3);
                break;
            case ANX7150_V720x576p_50Hz_4x3:
            case ANX7150_V720x576p_50Hz_16x9:
                //debug_puts("720x576p_50Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 90 + i);
                delay_ms(3);
                break;
            case ANX7150_V1280x720p_50Hz:
                //debug_puts("1280x720p_50Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 108 + i);
                delay_ms(3);
                break;
            case ANX7150_V1920x1080i_50Hz:
                //debug_puts("1920x1080i_50Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 126 + i);
                delay_ms(3);
                break;
            case ANX7150_V720x576i_50Hz_4x3:
            case ANX7150_V720x576i_50Hz_16x9:
                //debug_puts("720x576i_50Hz!\n");
                ANX7150_video_timing_parameter[i] = Load_from_EEPROM(0, 144 + i);
                delay_ms(3);
                break;

            default:
                break;
        }
        //debug_printf("Video_Timing_Parameter[%.2x]=%.2x\n", (WORD)i, (WORD) ANX7150_video_timing_parameter[i]);
    }
    #endif*/
}



void ANX7150_DE_Generator(void)
{
    UINT8 video_type,hsync_pol,vsync_pol,v_fp,v_bp,vsync_width;
    UINT8 hsync_width_low,hsync_width_high,v_active_low,v_active_high;
    UINT8 h_active_low,h_active_high,h_res_low,h_res_high,h_bp_low,h_bp_high;
    UINT32  hsync_width,h_active,h_res,h_bp;
    UINT32  Reg;

    video_type = ANX7150_video_timing_parameter[15];
    hsync_pol = ANX7150_video_timing_parameter[16];
    vsync_pol = ANX7150_video_timing_parameter[17];
    v_fp = ANX7150_video_timing_parameter[12];
    v_bp = ANX7150_video_timing_parameter[11];
    vsync_width = ANX7150_video_timing_parameter[10];
    hsync_width = ANX7150_video_timing_parameter[5];
    hsync_width = (hsync_width << 8) + ANX7150_video_timing_parameter[4];
    v_active_high = ANX7150_video_timing_parameter[9];
    v_active_low = ANX7150_video_timing_parameter[8];
    h_active = ANX7150_video_timing_parameter[3];
    h_active = (h_active << 8) + ANX7150_video_timing_parameter[2];
    h_res = ANX7150_video_timing_parameter[1];
    h_res = (h_res << 8) + ANX7150_video_timing_parameter[0];
    h_bp = ANX7150_video_timing_parameter[7];
    h_bp = (h_bp << 8) + ANX7150_video_timing_parameter[6];
    if(ANX7150_demux_yc_en)
    {
        hsync_width = 2* hsync_width;
        h_active = 2 * h_active;
        h_res = 2 * h_res;
        h_bp = 2 * h_bp;
    }
    hsync_width_low = hsync_width & 0xff;
    hsync_width_high = (hsync_width >> 8) & 0xff;
    h_active_low = h_active & 0xff;
    h_active_high = (h_active >> 8) & 0xff;
    h_res_low = h_res & 0xff;
    h_res_high = (h_res >> 8) & 0xff;
    h_bp_low = h_bp & 0xff;
    h_bp_high = (h_bp >> 8) & 0xff;

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_CAPCTRL1_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_CAPCTRL1_REG, Reg & 0xf7 |video_type);//set video type

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_CAPCTRL1_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_CAPCTRL1_REG, Reg  & 0xdf |hsync_pol);//set HSYNC polarity

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_CAPCTRL1_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_CAPCTRL1_REG, Reg & 0xbf |vsync_pol);//set VSYNC polarity

    ANX7150_i2c_write_p0_reg(ANX7150_ACT_LINEL_REG, v_active_low);
    ANX7150_i2c_write_p0_reg(ANX7150_ACT_LINEH_REG, v_active_high);
    ANX7150_i2c_write_p0_reg(ANX7150_VSYNC_WID_REG, vsync_width);
    ANX7150_i2c_write_p0_reg(ANX7150_VSYNC_TAIL2VIDLINE_REG, v_bp);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_PIXL_REG, h_active_low);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_PIXH_REG, h_active_high);
    ANX7150_i2c_write_p0_reg(ANX7150_H_RESL_REG, h_res_low);
    ANX7150_i2c_write_p0_reg(ANX7150_H_RESH_REG, h_res_high);
    ANX7150_i2c_write_p0_reg(ANX7150_HSYNC_ACT_WIDTHL_REG, hsync_width_low);
    ANX7150_i2c_write_p0_reg(ANX7150_HSYNC_ACT_WIDTHH_REG, hsync_width_high);
    ANX7150_i2c_write_p0_reg(ANX7150_H_BACKPORCHL_REG, h_bp_low);
    ANX7150_i2c_write_p0_reg(ANX7150_H_BACKPORCHH_REG, h_bp_high);

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_CAPCTRL0_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_CAPCTRL0_REG, Reg | ANX7150_VID_CAPCTRL0_DEGEN_EN);
}



void ANX7150_Blue_Screen_Format_Config(void)
{
    // TODO:Add ITU 601 format.(Now only ITU 709 format added)
    switch (ANX7150_RGBorYCbCr)
    {
        case ANX7150_RGB: //select RGB mode
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN0_REG, 0x10);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN1_REG, 0xeb);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN2_REG, 0x10);
            break;
        case ANX7150_YCbCr422: //select YCbCr4:2:2 mode
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN0_REG, 0x00);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN1_REG, 0xad);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN2_REG, 0x2a);
            break;
        case ANX7150_YCbCr444: //select YCbCr4:4:4 mode
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN0_REG, 0x1a);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN1_REG, 0xad);
            ANX7150_i2c_write_p0_reg(ANX7150_HDCP_BLUESCREEN2_REG, 0x2a);
            break;
        default:
            break;
    }
}


void ANX7150_Parse_Video_Format(void)
{
    switch(ANX7150_video_format_config)
    {
        case ANX7150_RGB_YCrCb444_SepSync:
            ANX7150_emb_sync_mode = 0;
            ANX7150_demux_yc_en = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            anx7150_msg(("RGB_YCrCb444_SepSync mode!\n\r"));
            break;
        case ANX7150_YCrCb422_SepSync:
            ANX7150_emb_sync_mode = 0;
            ANX7150_demux_yc_en = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            anx7150_msg(("YCrCb422_SepSync mode!\n\r"));
            break;
        /*case ANX7150_YCrCb422_EmbSync:
            anx7150_msg(("YCrCb422_EmbSync mode!\n\r"));
            ANX7150_demux_yc_en = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_emb_sync_mode = 1;
            ANX7150_Get_Video_Timing();
            break;*/
        case ANX7150_YCMux422_SepSync_Mode1:
            anx7150_msg(("YCMux422_SepSync_Mode1 mode!\n\r"));
            ANX7150_emb_sync_mode = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_ycmux_BIT_sel = 0;
            ANX7150_demux_yc_en = 1;
            break;
        case ANX7150_YCMux422_SepSync_Mode2:
            anx7150_msg(("YCMux422_SepSync_Mode2 mode!\n\r"));
            ANX7150_emb_sync_mode = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_ycmux_BIT_sel = 1;
            ANX7150_demux_yc_en = 1;
            break;
        /*case ANX7150_YCMux422_EmbSync_Mode1:
            anx7150_msg(("YCMux422_EmbSync_Mode1 mode!\n\r"));
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_emb_sync_mode = 1;
            ANX7150_ycmux_BIT_sel = 0;
            ANX7150_demux_yc_en = 1;
            ANX7150_Get_Video_Timing();
            break;
        case ANX7150_YCMux422_EmbSync_Mode2:
            anx7150_msg(("YCMux422_EmbSync_Mode2 mode!\n\r"));
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_emb_sync_mode = 1;
            ANX7150_ycmux_BIT_sel = 1;
            ANX7150_demux_yc_en = 1;
            ANX7150_Get_Video_Timing();
            break;*/
        case ANX7150_RGB_YCrCb444_DDR_SepSync:
            anx7150_msg(("RGB_YCrCb444_DDR_SepSync mode!\n\r"));
            ANX7150_emb_sync_mode = 0;
            ANX7150_demux_yc_en = 0;
            ANX7150_de_gen_en = 1;//2010/01/29
            ANX7150_ddr_bus_mode = 1;
            ANX7150_Get_Video_Timing();
            break;
        /*case ANX7150_RGB_YCrCb444_DDR_EmbSync:
            anx7150_msg(("RGB_YCrCb444_DDR_EmbSync mode!\n\r"));
            ANX7150_demux_yc_en = 0;
            ANX7150_de_gen_en = 0;
            ANX7150_emb_sync_mode = 1;
            ANX7150_ddr_bus_mode = 1;
            ANX7150_Get_Video_Timing();
            break;
        case ANX7150_RGB_YCrCb444_SepSync_No_DE:
            anx7150_msg(("RGB_YCrCb444_SepSync_No_DE mode!\n\r"));
            ANX7150_emb_sync_mode = 0;
            ANX7150_demux_yc_en = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 1;
            ANX7150_Get_Video_Timing();
            break;
        case ANX7150_YCrCb422_SepSync_No_DE:
            anx7150_msg(("YCrCb422_SepSync_No_DE mode!\n\r"));
            ANX7150_emb_sync_mode = 0;
            ANX7150_demux_yc_en = 0;
            ANX7150_ddr_bus_mode = 0;
            ANX7150_de_gen_en = 1;
            ANX7150_Get_Video_Timing();
            break;*/
        default:
            debug_err(("Video Interface NO SUPPORT!! \r\n"));
            break;
    }
}


void ANX7150_Config_Video(void)
{
    UINT32   Reg,TX_is_HDMI;
    BOOL    cspace_y2r, y2r_sel, up_sample,range_y2r;

    cspace_y2r  = 0;
    y2r_sel     = 0;
    up_sample   = 0;
    range_y2r   = 0;

    ANX7150_RGBorYCbCr = ANX7150_INPUT_COLORSPACE;//update
    ANX7150_i2c_write_p0_reg(ANX7150_VID_MODE_REG, 0x00);//update

    ANX7150_clear_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_BITCTRL_EN);

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_STATE_REG);
    if(!(Reg & 0x02))
    {
        anx7150_msg(("No clock detected !!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r"));

        //#NT# No return, we just open hdmi first and open IDE later.
        //return;
    }

    ANX7150_Clean_HDCP();

    //
    //color space issue
    //
    switch(ANX7150_video_timing_id)
    {
        case HDMI_1280X720P50:
        case HDMI_1280X720P60:
        case HDMI_1920X1080I60:
        case HDMI_1920X1080I50:
        case HDMI_1920X1080P60:
        case HDMI_1920X1080P50:
           y2r_sel = ANX7150_CSC_BT709;
           break;
        default:
           y2r_sel = ANX7150_CSC_BT601;
           break;
    }

    //rang[0~255]/[16~235] select
    if(ANX7150_video_timing_id == HDMI_640X480P60)
        range_y2r = 1;//rang[0~255]
    else
        range_y2r = 0;//rang[16~235]

    if((ANX7150_RGBorYCbCr == ANX7150_YCbCr422) && (!(guiHdmiEdidInfo&HDMITX_TV_YCBCR422)))
    {
        up_sample = 1;

        if(guiHdmiEdidInfo&HDMITX_TV_YCBCR444)
            cspace_y2r = 0;
        else
            cspace_y2r = 1;
    }

    if((ANX7150_RGBorYCbCr == ANX7150_YCbCr444) && (!(guiHdmiEdidInfo&HDMITX_TV_YCBCR444)))
    {
        cspace_y2r = 1;
    }

    //Config the embeded blue screen format according to output video format.
    ANX7150_Blue_Screen_Format_Config();

    ANX7150_Parse_Video_Format();

    if(ANX7150_de_gen_en)
    {
        debug_err(("ANX7150_de_gen_en!\n"));
        ANX7150_DE_Generator();
    }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DEGEN_EN);
    }


    if(ANX7150_emb_sync_mode)
    {
        debug_err(("ANX7150_Embed_Sync_Decode! NO SUPPORT!!!!\r\n"));
    }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_EMSYNC_EN);
    }


    if(ANX7150_demux_yc_en)
    {
        anx7150_msg(("ANX7150_demux_yc_en!\r\n"));

        ANX7150_set_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DEMUX_EN);

        if(ANX7150_ycmux_BIT_sel)
        {
            ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_YCBIT_SEL);

            //jack wen, bit mapping for yc mux, D13-8,1-0 -->D11-4
            anx7150_msg(("ANX7150_demux_yc_en!####D11-4 \r\n"));

            ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG,ANX7150_VID_CTRL_BITCTRL_EN);

            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL11,    0x0d);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL10,    0x0c);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL9,     0x0b);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL8,     0x0a);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL7,     0x09);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL6,     0x08);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL5,     0x01);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL4,     0x00);

         }
         else
         {
            ANX7150_clear_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_YCBIT_SEL);

            //jack wen, bit mapping for yc mux, D13-8,1-0 -->D15-8,
            anx7150_msg(("ANX7150_demux_yc_en!####D15-8\r\n"));

            ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_BITCTRL_EN);

            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL15,    0x0d);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL14,    0x0c);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL13,    0x0b);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL12,    0x0a);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL11,    0x09);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL10,    0x08);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL9,     0x01);
            ANX7150_i2c_write_p0_reg(VID_BIT_CTRL8,     0x00);

         }
     }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DEMUX_EN);
    }


     if(ANX7150_ddr_bus_mode)
     {
         anx7150_msg(("ANX7150_ddr_bus_mode!\n\r"));
         ANX7150_set_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DV_BUSMODE);

         ANX7150_set_p0_bit(ANX7150_SYS_CTRL4_REG, 0x02);
         ANX7150_clear_p0_bit(ANX7150_SYS_CTRL4_REG, 0x01);

         ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_YCBIT_SEL);


         if(ANX7150_ddr_edge)
         {
            ANX7150_set_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DDR_EDGE);
         }
         else
         {
            ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DDR_EDGE);
         }


         if (ANX7150_video_format_config == 0x07)//jack wen, DDR yc422, 601,
         {
                anx7150_msg(("ANX7150_DDR_601_Maping!\r\n"));
                ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_BITCTRL_EN);

                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL23,    0x0B);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL22,    0x0A);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL21,    0x09);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL20,    0x08);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL19,    0x07);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL18,    0x06);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL17,    0x05);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL16,    0x04);

                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL15,    0x17);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL14,    0x16);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL13,    0x15);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL12,    0x14);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL11,    0x13);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL10,    0x12);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL9,     0x11);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL8,     0x10);

                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL7,     0x03);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL6,     0x02);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL5,     0x01);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL4,     0x00);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL3,     0x0F);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL2,     0x0E);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL1,     0x0D);
                ANX7150_i2c_write_p0_reg(VID_BIT_CTRL0,     0x0C);

         }
         else if (ANX7150_video_format_config == 0x08)//jack wen, DDR yc422, 656,
         {
                debug_err(("ANX7150_DDR_656_Maping! No Support Format!!!!!!\r\n"));

                ANX7150_clear_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_BITCTRL_EN);
         }

     }
     else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DV_BUSMODE);
        ANX7150_clear_p0_bit(ANX7150_VID_CAPCTRL0_REG, ANX7150_VID_CAPCTRL0_DDR_EDGE);
        ANX7150_clear_p0_bit(ANX7150_SYS_CTRL4_REG, 0x03);
     }

    if(cspace_y2r)
    {
        //debug_puts("Color space Y2R enabled********");
        ANX7150_set_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_CSPACE_Y2R);

        if(y2r_sel)
        {
            //debug_puts("Y2R_SEL!\n");
            ANX7150_set_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_Y2R_SEL);
        }
        else
        {
            ANX7150_clear_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_Y2R_SEL);
        }
    }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_CSPACE_Y2R);
    }

    if(up_sample)
    {
        //debug_puts("UP_SAMPLE!\n");
        ANX7150_set_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_UPSAMPLE);
    }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_UPSAMPLE);
    }

    if(range_y2r)
    {
        ANX7150_set_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_RANGE_Y2R);
    }
    else
    {
        ANX7150_clear_p0_bit(ANX7150_VID_MODE_REG, ANX7150_VID_MODE_RANGE_Y2R);
    }

    //if(!ANX7150_pix_rpt_set_by_sys)
    {
        if((ANX7150_video_timing_id == HDMI_720X480I60_16X9)
            || (ANX7150_video_timing_id == HDMI_720X576I50)
            || (ANX7150_video_timing_id == HDMI_1440X480I60))
            ANX7150_tx_pix_rpt = 1;
        else
            ANX7150_tx_pix_rpt = 0;
    }

    //set input pixel repeat times
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_MODE_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_MODE_REG, ((Reg & 0xfc) |ANX7150_in_pix_rpt));

    //set link pixel repeat times
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_CTRL_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_VID_CTRL_REG, ((Reg & 0xfc) |ANX7150_tx_pix_rpt));

     if((ANX7150_in_pix_rpt != ANX7150_in_pix_rpt_bkp)
        ||(ANX7150_tx_pix_rpt != ANX7150_tx_pix_rpt_bkp) )
    {
        ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, 0x02);
        ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL2_REG, 0x00);
        anx7150_msg(("MISC_Reset!\n\r"));
        ANX7150_in_pix_rpt_bkp = ANX7150_in_pix_rpt;
        ANX7150_tx_pix_rpt_bkp = ANX7150_tx_pix_rpt;
    }

    //enable video input
    ANX7150_set_p0_bit(ANX7150_VID_CTRL_REG, ANX7150_VID_CTRL_IN_EN);


    anx7150_msg(("Video configure OK!\n\r"));

    //
    //  KLINS: Skip Video not stable Check! We open IDE later...
    //
    /*
    while(1)
    {
        Reg = ANX7150_i2c_read_p0_reg(ANX7150_VID_STATUS_REG);
        if(!(Reg & ANX7150_VID_STATUS_VID_STABLE))
        {
            anx7150_msg(("Video not stable!!!!!!!!!!!!!!!!!\n\r"));
            break;
        }
        else
            break;
    }
    */

    if(cspace_y2r)
        ANX7150_RGBorYCbCr = ANX7150_RGB;

    //Enable video CLK,Format change after config video.
    ANX7150_set_p0_bit(ANX7150_INTR1_MASK_REG, 0x01);
    ANX7150_set_p0_bit(ANX7150_INTR3_MASK_REG, 0x40);



    if(guiHdmiEdidInfo & HDMITX_TV_HDMI)
    {
        ANX7150_set_p0_bit(ANX7150_SYS_CTRL1_REG, 0x02);
        anx7150_msg(("ANX7150 is set to HDMI mode\n\r"));
    }

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_CTRL1_REG);
    TX_is_HDMI = Reg & 0x02;

    if(TX_is_HDMI == 0x02)
    {
        ANX7150_Set_AVMute();//wen
        //ANX7150_Set_System_State(ANX7150_CONFIG_AUDIO);
    }


    //reset TMDS link to align 4 channels  xy 061120
    ANX7150_set_p0_bit(ANX7150_SRST_REG, ANX7150_TX_RST);
    ANX7150_clear_p0_bit(ANX7150_SRST_REG, ANX7150_TX_RST);

    //Enable TMDS clock output // just enable bit7, and let the other bits along to avoid overwriting.
    ANX7150_set_p0_bit(ANX7150_TMDS_CLKCH_CONFIG_REG, ANX7150_TMDS_CLKCH_MUTE);

    TimerDelayMs(400);  //delay_ms(10);
}

#endif



#if 1


UINT32 ANX7150_Config_I2s(void)
{
    UINT32 exe_result = 0x00;
    UINT32 Reg,Reg2;
    //BYTE c = 0x00;
    //BYTE c1 = 0x00;

    anx7150_msg(("ANX7150: config i2s audio.\r\n"));

    //select SCK as source
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG);
    Reg &=  ~ANX7150_HDMI_AUDCTRL1_CLK_SEL;
    ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, Reg);
    anx7150_msg(("select SCK as source, c = 0x%.2x\n\r", Reg));

    //config i2s channel
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG);
    Reg2 =(UINT32) s_ANX7150_audio_config.i2s_config.audio_channel;    // need BYTE[5:2]
    Reg2 &= 0x3c;
    Reg &= ~0x3c;
    Reg |= Reg2;
    ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, Reg);
    anx7150_msg(("config i2s channel, c = 0x%.2x\n\r", Reg));

    //config i2s format
    Reg =(UINT32) s_ANX7150_audio_config.i2s_config.i2s_format;
    ANX7150_i2c_write_p0_reg(ANX7150_I2S_CTRL_REG, Reg );
    anx7150_msg(("config i2s format, c = 0x%.2x\n\r",Reg ));

    //map i2s fifo
    // TODO: config I2S channel map register according to system


    //ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_CTRL_REG, c);

    //swap right/left channel
    /*ANX7150_i2c_read_p0_reg(ANX7150_I2SCH_SWCTRL_REG, &c);
    c1 = 0x00;
    c1 &= 0xf0;
    c &= ~0xf0;
    c |= c1;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_SWCTRL_REG, c);
    debug_printf("map i2s ffio, c = 0x%.2x\n",(WORD)c);*/

    //down sample
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL0_REG);
    Reg2 = (UINT32)s_ANX7150_audio_config.down_sample;
    Reg2 &= 0x60;
    Reg &= ~0x60;
    Reg |= Reg2;
    ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL0_REG, Reg);
    anx7150_msg(("down sample, c = 0x%.2x\n\r", Reg));

    //config i2s channel status(5 regs)
    Reg = s_ANX7150_audio_config.i2s_config.Channel_status1;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS1_REG, Reg);
    Reg = s_ANX7150_audio_config.i2s_config.Channel_status2;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS2_REG, Reg);
    Reg = s_ANX7150_audio_config.i2s_config.Channel_status3;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS3_REG, Reg);
    Reg = s_ANX7150_audio_config.i2s_config.Channel_status4;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS4_REG, Reg);
    //debug_printf("@@@@@@@@config i2s channel status4, c = 0x%.2x\n",(unsigned int)c);//jack wen

    Reg = s_ANX7150_audio_config.i2s_config.Channel_status5;
    ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS5_REG, Reg);
    anx7150_msg(("config i2s channel status, c = 0x%.2x\n\r",Reg));

    exe_result = ANX7150_i2s_input;
    anx7150_msg(("return = 0x%.2x\n\r", exe_result));

    // open corresponding interrupt
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_INTR1_MASK_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_INTR1_MASK_REG, (Reg | 0x22) );

    Reg = ANX7150_i2c_read_p0_reg(ANX7150_INTR3_MASK_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_INTR3_MASK_REG, (Reg | 0x20) );

    return exe_result;
}


UINT32 ANX7150_Config_Audio(void)
{
    UINT32  exe_result = 0x00;
    UINT32  Reg;
    UINT8   audio_layout = 0x00;
    UINT8   fs = 0x00;
    UINT32  ACR_N = 0x0000;


    //set audio clock edge
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL0_REG);
    ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL0_REG,  (Reg & 0xf7)|ANX7150_audio_clock_edge);

    //cts get select
    ANX7150_clear_p0_bit(ANX7150_HDMI_AUDCTRL0_REG, 0x10);

    anx7150_msg(("audio_type = 0x%.2x\n", s_ANX7150_audio_config.audio_type));
    if (s_ANX7150_audio_config.audio_type & ANX7150_i2s_input)
    {
        exe_result |= ANX7150_Config_I2s();
    }
    else
    {
        //disable I2S audio input
        debug_err(("ANX7150: disable I2S audio input.\r\n"));
        debug_err(("Interface Error! We support I2S only!\r\n"));

        Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG);
        Reg &= 0xc3;
        ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, Reg);
    }

    //
    //Config Audio Sampling Rate
    //
    {
        UINT32 temp;

        if(ANX7150_audio_id == HDMI_AUDIO32KHZ)
            temp = ANX7150_I2SCH_STATUS4_32Khz;
        else if(ANX7150_audio_id == HDMI_AUDIO44_1KHZ)
            temp = ANX7150_I2SCH_STATUS4_44Khz;
        else
            temp = ANX7150_I2SCH_STATUS4_48Khz;

        Reg = ANX7150_i2c_read_p0_reg(ANX7150_I2SCH_STATUS4_REG);
        Reg &= ~ 0x0F;
        Reg |= temp;
        ANX7150_i2c_write_p0_reg(ANX7150_I2SCH_STATUS4_REG, Reg);
    }





    if (s_ANX7150_audio_config.audio_type & ANX7150_spdif_input)
    {
        //exe_result |= ANX7150_Config_Spdif();
        debug_err(("Interface Error! We support I2S only! No support spdif!\r\n"));
    }
    else
    {
        //disable SPDIF audio input
        ANX7150_clear_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_SPDIFIN_EN);
    }

    if (s_ANX7150_audio_config.audio_type & ANX7150_super_audio_input)
    {
        //exe_result |= ANX7150_Config_Super_Audio();
        debug_err(("Interface Error! We support I2S only! No support super audio!\r\n"));
    }
    else
    {
        //disable super audio output
        //debug_puts("ANX7150: disable super audio output.");
        ANX7150_i2c_write_p0_reg(ANX7150_ONEBIT_AUD_CTRL_REG, 0x00);
    }

    if (s_ANX7150_audio_config.audio_type & 0x07 == 0x00)
    {
        debug_err(("ANX7150 input no audio type.!!!!!!!!!!!!!!!!!!!! \r\n"));
    }

    //audio layout
    if (s_ANX7150_audio_config.audio_type & ANX7150_i2s_input)
    {
        Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG);
        if( Reg & 0x38)//BIT[5:3]
        {
            audio_layout = 0x80;
        }
    }


    Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL0_REG);
    Reg |= audio_layout;
    ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL0_REG, Reg);

    if(  (s_ANX7150_audio_config.audio_type & 0x07) == exe_result )
    {

        //Initial N value
        Reg = ANX7150_i2c_read_p0_reg(ANX7150_I2SCH_STATUS4_REG);
        fs = Reg & 0x0F;

        // set default value to N
        switch(fs)
        {
            case(0x00)://44.1k
                ACR_N = ANX7150_N_44k;
                break;
            case(0x02)://48k
                ACR_N = ANX7150_N_48k;
                break;
            case(0x03)://32k
                ACR_N = ANX7150_N_32k;
                break;
            case(0x08)://88k
                ACR_N = ANX7150_N_88k;
                break;
            case(0x0a)://96k
                ACR_N = ANX7150_N_96k;
                break;
            case(0x0c)://176k
                ACR_N = ANX7150_N_176k;
                break;
            case(0x0e)://192k
                ACR_N = ANX7150_N_192k;
                break;
            default:
                debug_err(("ANX7150 Config Audio: Wrong fs.\r\n"));
                ACR_N = ANX7150_N_48k;
                break;
        }


        anx7150_msg(("KLINS: Audio is config ACR_N = %d, fs = %d \r\n",ACR_N,fs));

        // write N(ACR) to corresponding regs
        Reg = ACR_N & 0xFF;
        ANX7150_i2c_write_p1_reg(ANX7150_ACR_N1_SW_REG, Reg);
        Reg = (ACR_N>>8)&0xFF;
        ANX7150_i2c_write_p1_reg(ANX7150_ACR_N2_SW_REG, Reg);
        ANX7150_i2c_write_p1_reg(ANX7150_ACR_N3_SW_REG, 0x00);


        // set the relation of MCLK and Fs
        Reg = ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL0_REG);
        ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL0_REG, (Reg & 0xF8)|FREQ_MCLK);
        anx7150_msg(("Audio MCLK input mode is: %.2x\n\r",FREQ_MCLK));

        //Enable control of ACR
        ANX7150_set_p1_bit(ANX7150_INFO_PKTCTRL1_REG, ANX7150_INFO_PKTCTRL1_ACR_EN);

        //audio enable:
        ANX7150_set_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);

        /* Go To ANX7150_CONFIG_PACKETS State */
    }

    return exe_result;

}
#endif





#if 1


UINT8 ANX7150_Checksum(infoframe_struct *p)
{
    UINT8 checksum = 0x00;
    UINT8 i;

    checksum = p->type + p->length + p->version;
    for(i=1; i <= p->length; i++)
    {
        checksum += p->pb_byte[i];
    }
    checksum = ~checksum;
    checksum += 0x01;

    return checksum;
}


void ANX7150_Load_Infoframe(packet_type member, infoframe_struct *p)
{
    UINT8 address[8] = {0x00,0x20,0x40,0x60,0x80,0x80,0xa0,0xa0};
    UINT8 i;
    UINT32 c;

    p->pb_byte[0] = ANX7150_Checksum(p);

    // write infoframe to according regs
    ANX7150_i2c_write_p1_reg(address[member], p->type);
    ANX7150_i2c_write_p1_reg(address[member]+1, p->version);
    ANX7150_i2c_write_p1_reg(address[member]+2, p->length);

    for(i=0; i <= p->length; i++)
    {
        ANX7150_i2c_write_p1_reg(address[member]+3+i, p->pb_byte[i]);
        c = ANX7150_i2c_read_p1_reg(address[member]+3+i);
    }

}


UINT32 ANX7150_Config_Packet(void)
{
    UINT32 exe_result = 0x00;// There is no use in current solution
    UINT32 info_packet_sel;
    UINT32 Reg;
    UINT8  c;


    info_packet_sel = s_ANX7150_packet_config.packets_need_config;
    anx7150_msg(("info_packet_sel = 0x%.2x\n\r",info_packet_sel));

    // New packet?
    if( info_packet_sel != 0x00)
    {
        // avi infoframe
        if( info_packet_sel & ANX7150_avi_sel )
        {
            c = s_ANX7150_packet_config.avi_info.pb_byte[1];  //color space
            c &= 0x9f;
            c |= (ANX7150_RGBorYCbCr << 5);
            s_ANX7150_packet_config.avi_info.pb_byte[1] = c | 0x10;//***

            c = s_ANX7150_packet_config.avi_info.pb_byte[4];// vid ID
            c = c & 0x80;
            s_ANX7150_packet_config.avi_info.pb_byte[4] = c | ANX7150_video_timing_id;

            c = s_ANX7150_packet_config.avi_info.pb_byte[5]; //repeat times
            c = c & 0xf0;
            c |= (ANX7150_tx_pix_rpt & 0x0f);
            s_ANX7150_packet_config.avi_info.pb_byte[5] = c;//*****

            anx7150_msg(("config avi infoframe packet.\r\n"));

            // Disable repeater
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL1_REG);
            Reg &= ~ANX7150_INFO_PKTCTRL1_AVI_RPT;
            ANX7150_i2c_write_p1_reg(ANX7150_INFO_PKTCTRL1_REG, Reg);

            // Enable?wait:go
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL1_REG);
            if(Reg & ANX7150_INFO_PKTCTRL1_AVI_EN)
            {
                anx7150_msg(("wait disable, config avi infoframe packet.\r\n"));
                //return exe_result; //jack wen
            }

            // load packet data to regs
            ANX7150_Load_Infoframe( ANX7150_avi_infoframe,
                                    &(s_ANX7150_packet_config.avi_info));
            // Enable and repeater
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL1_REG);
            Reg |= 0x30;
            ANX7150_i2c_write_p1_reg(ANX7150_INFO_PKTCTRL1_REG, Reg);

            // complete avi packet
            anx7150_msg(("config avi infoframe packet done.\r\n"));
            s_ANX7150_packet_config.packets_need_config &= ~ANX7150_avi_sel;

        }

        // audio infoframe
        if( info_packet_sel & ANX7150_audio_sel )
        {
            anx7150_msg(("Config audio infoframe packet.\r\n"));

            // Disable repeater
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL2_REG);
            Reg &= ~ANX7150_INFO_PKTCTRL2_AIF_RPT;
            ANX7150_i2c_write_p1_reg(ANX7150_INFO_PKTCTRL2_REG, Reg);

            // Enable?wait:go
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL2_REG);
            if(Reg & ANX7150_INFO_PKTCTRL2_AIF_EN)
            {
                anx7150_msg(("wait disable, config audio infoframe packet.\r\n"));
                //return exe_result;//jack wen
            }
            // config packet

            // load packet data to regs
            ANX7150_Load_Infoframe( ANX7150_audio_infoframe,
                                    &(s_ANX7150_packet_config.audio_info));
            // Enable and repeater
            Reg = ANX7150_i2c_read_p1_reg(ANX7150_INFO_PKTCTRL2_REG);
            Reg |= 0x03;
            ANX7150_i2c_write_p1_reg(ANX7150_INFO_PKTCTRL2_REG, Reg);

            // complete avi packet
            anx7150_msg(("config audio infoframe packet done.\r\n"));
            s_ANX7150_packet_config.packets_need_config &= ~ANX7150_audio_sel;

        }

    }


    if( s_ANX7150_packet_config.packets_need_config  == 0x00)
    {
         anx7150_msg(("config packets done\n\r"));
         //ANX7150_Set_System_State(ANX7150_HDCP_AUTHENTICATION);
    }

    return exe_result;
}


#endif

#if 1


void ANX7150_Blue_Screen_Disable(void)
{
    ANX7150_clear_p0_bit(ANX7150_HDCP_CTRL1_REG, ANX7150_HDCP_CTRL1_BLUE_SCREEN_EN);
}

void ANX7150_HDCP_Process(void)
{
    UINT32  Reg;
    //UINT8   Bksv_valid=0;//wen HDCP CTS

    //if(0)
    #if 0
    { //HDCP_EN =1 means to do HDCP authentication,SWITCH4 = 0 means not to do HDCP authentication.

        delay_ms(10);//let unencrypted video play a while, required by HDCP CTS. SY//wen HDCP CTS
        ANX7150_Set_AVMute();//before auth, set_avmute//wen
        delay_ms(10);//wen HDCP CTS

        if( !ANX7150_hdcp_init_done )
        {
                ANX7150_i2c_read_p0_reg(ANX7150_SYS_CTRL1_REG, &c); //72:07.2 hdcp on//wen HDCP CTS
                ANX7150_i2c_write_p0_reg(ANX7150_SYS_CTRL1_REG, (c | ANX7150_SYS_CTRL1_HDCPMODE));//wen HDCP CTS
            if(ANX7150_edid_result.is_HDMI)
                    ANX7150_Hardware_HDCP_Auth_Init();
            else
            {   //DVI, disable 1.1 feature and enable HDCP two special point check
                    ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL1_REG, &c);
                    ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL1_REG,
                                                         (c & (~ANX7150_HDCP_CTRL1_HDCP11_EN) | ANX7150_LINK_CHK_12_EN));
            }

            //wen HDCP CTS
            if(!ANX7150_BKSV_SRM())
            {
                ANX7150_Blue_Screen_Enable();
                ANX7150_Clear_AVMute();
                Bksv_valid=0;
                return;
            }
            else //SY.
            {
                Bksv_valid=1;
                ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL0_REG, &c);
                ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, (c | 0x03));
            }

            ANX7150_hdcp_init_done = 1;
//wen HDCP CTS
        }


//wen HDCP CTS
        if((Bksv_valid) && (!ANX7150_hdcp_auth_en))
        {
                debug_puts("enable hw hdcp");
                ANX7150_RST_DDCChannel();
                ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL0_REG, &c);
                ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, (c | ANX7150_HDCP_CTRL0_HW_AUTHEN));
                ANX7150_hdcp_auth_en = 1;
        }

         if((Bksv_valid) && (ANX7150_hdcp_wait_100ms_needed))
        {
            ANX7150_hdcp_wait_100ms_needed = 0;
            //disable audio
            ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, &c);
            ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, c & (~ANX7150_HDMI_AUDCTRL1_IN_EN));
            debug_puts("++++++++ANX7150_hdcp_wait_100ms_needed----------+++++++++\n");
            delay_ms(100);
            return;
        }
//wen HDCP CTS

        if(ANX7150_hdcp_auth_pass)          //wen HDCP CTS
        {
                //Clear the SRM_Check_Pass BIT, then when reauthentication occurs, firmware can catch it.
                ANX7150_i2c_read_p0_reg(ANX7150_HDCP_CTRL0_REG, &c);
                ANX7150_i2c_write_p0_reg(ANX7150_HDCP_CTRL0_REG, c & 0xfc);

            //Enable HDCP Hardware encryption
            if(!ANX7150_hdcp_encryption)
            {
                ANX7150_HDCP_Encryption_Enable();
            }
            if(ANX7150_send_blue_screen)
            {
                ANX7150_Blue_Screen_Disable();
            }
             if(ANX7150_avmute_enable)
             {
                  ANX7150_Clear_AVMute();
             }
            //enable audio SY.
            ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, &c);
            c |= ANX7150_HDMI_AUDCTRL1_IN_EN;
            ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, c);
            debug_puts("@@@@@  HDCP Auth PASSED!   @@@@@\n");

            if (ANX7150_hdcp_bcaps & 0x40) //repeater
            {
                debug_puts("Find a repeater!");
                //actually it is KSVList check. we can't do SRM check due to the lack of SRM file. SY.
                if(!ANX7150_srm_checked)
                {
                    if (!ANX7150_IS_KSVList_VLD())
                    {
                        debug_puts("ksvlist not good. disable encryption");
                        ANX7150_HDCP_Encryption_Disable();
                        ANX7150_Blue_Screen_Enable();
                        ANX7150_Clear_AVMute();
                        ANX7150_ksv_srm_pass = 0;
                        ANX7150_Clean_HDCP();//SY.
                        //remove below will pass 1b-05/1b-06
                        //ANX7150_Set_System_State(ANX7150_WAIT_HOTPLUG);//SY.
                        return;
                    }
                    ANX7150_srm_checked=1;
                    ANX7150_ksv_srm_pass = 1;
                }
            }
            else
            {
                debug_puts("Find a receiver.");

            }
        }
        else                            //wen HDCP CTS
        {
            debug_puts("#####   HDCP Auth FAILED!   #####\n");
        //also need to disable HW AUTHEN
                ANX7150_hdcp_auth_en = 0;
                ANX7150_hdcp_init_done = 0;
                ANX7150_hdcp_wait_100ms_needed = 1; //wen, update 080703

             if(ANX7150_hdcp_encryption)
            {
                ANX7150_HDCP_Encryption_Disable();
            }
            if(!ANX7150_send_blue_screen)
            {
                ANX7150_Blue_Screen_Enable();
            }
            if(ANX7150_avmute_enable)
            {
                ANX7150_Clear_AVMute();
            }
            //disable audio
            ANX7150_i2c_read_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, &c);
            ANX7150_i2c_write_p0_reg(ANX7150_HDMI_AUDCTRL1_REG, c & (~ANX7150_HDMI_AUDCTRL1_IN_EN));

            return;
        }

    }
    //else                //wen HDCP CTS
    #else
    {
        anx7150_msg(("hdcp pin is off.\r\n"));

        //if(ANX7150_send_blue_screen)
        {
            ANX7150_Blue_Screen_Disable();
        }

        //if(ANX7150_avmute_enable)
        {
            ANX7150_Clear_AVMute();
        }

        //enable audio SY.
        ANX7150_set_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);
    }
    #endif

    //wen HDCP CTS
    Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_CTRL1_REG); //72:07.1 hdmi or dvi mode
    Reg = Reg & 0x02;
    if(Reg == 0x02)
    {
        anx7150_msg(("end of ANX7150_HDCP_Process(): in HDMI mode.\r\n"));
        //ANX7150_Set_System_State(ANX7150_PLAY_BACK);
    }
    else
    {
        anx7150_msg(("!end of ANX7150_HDCP_Process(): in DVI mode.\r\n"));
    }
    //wen HDCP CTS

}

void ANX7150_PLAYBACK_Process(void)
{


    #if 0
    if((sysState_bkp == WAIT_AUDIO) && (sysState == PLAYBACK))
    {
        ANX7150_system_config_done = 0;
        ANX7150_Set_System_State(ANX7150_CONFIG_AUDIO);
        sysState_bkp = null_state;
        return;
    }

    #endif
    ANX7150_set_p0_bit(ANX7150_HDMI_AUDCTRL1_REG, ANX7150_HDMI_AUDCTRL1_IN_EN);
}

#endif

#if 1

void ANX7150_TimerSlot1(void)
{

    {
        ANX7150_clear_p0_bit(ANX7150_HDCP_CTRL0_REG, ANX7150_HDCP_CTRL0_HW_AUTHEN);

        ANX7150_RST_DDCChannel();

        ANX7150_Parse_EDID();

        /* Go to Wait RX Sense State*/
    }


    {
        UINT32 Reg;

        Reg = ANX7150_i2c_read_p0_reg(ANX7150_SYS_STATE_REG);
        if(Reg & ANX7150_SYS_STATE_RSV_DET)
        {
            anx7150_msg(("Receiver sense active.\r\n"));
        }

        /*Go to Config Video State*/
    }


}



void ANX7150_TimerSlot2(void)
{

    //if(ANX7150_system_state == ANX7150_CONFIG_VIDEO)
    {
        ANX7150_Config_Video();
    }

    //if(ANX7150_system_state == ANX7150_CONFIG_AUDIO)
    {
        ANX7150_Config_Audio();
    }

    //if(ANX7150_system_state == ANX7150_CONFIG_PACKETS)
    {
        ANX7150_Config_Packet();
    }

}


void ANX7150_TimerSlot3(void)
{
{

    //if(ANX7150_system_state == ANX7150_HDCP_AUTHENTICATION)
    {
        ANX7150_HDCP_Process();
    }

    //if(ANX7150_system_state == ANX7150_PLAY_BACK)
    {
        ANX7150_PLAYBACK_Process();
    }
}
}

#endif
