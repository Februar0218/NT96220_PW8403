#ifndef _ANX7150_H
#define _ANX7150_H

#include "gpio.h"

#define ANX7150_UNDERSCAN_HANDLE  0

#define ANX7150_DEBUG  0
#if ANX7150_DEBUG
#define anx7150_msg(msg)       debug_msg msg
#define ANX7150_FORCE_PLUG      0
#else
#define anx7150_msg(msg)
#endif
#define ANX7150_I2CSPEEDUP      1
#define ANX7150_DUMP_REGACCESS  0


//
// Anx7150 Interface Configuration definitions
//
//@{
/*
    0x00: input color space is RGB
    0x01: input color space is YCbCr422
    0x02: input color space is YCbCr444
*/
#define ANX7150_INPUT_COLORSPACE 0x01

/*
    0x00:use rising edge to latch even numbered pixel data//jack wen
    0x01:use falling edge to latch even numbered pixel data
*/
#define ANX7150_IDCK_EDGE_DDR 0x00

/*
    Select Audio Clock Edge

    0x00:use MCLK and SCK rising edge to latch audio data
    0x08:use MCLK and SCK falling edge to latch audio data
    0x80:use MCLK and SCK falling edge to latch audio data
*/
#define ANX7150_AUD_CLK_EDGE 0x00

/*
    Select audio hardware interface

    0x01:audio input comes from I2S
    0x02:audio input comes from SPDIF
    0x04:audio input comes from one bit audio
*/
#define ANX7150_AUD_HW_INTERFACE 0x01

/*
    Select I2S word length if audio HW interface is I2S

    0x02 = 16 bits;
    0x04 = 18 bits;
    0x08 = 19 bits;
    0x0A = 20 bits(maximal word length is 20bits);
    0x0C = 17 bits;
    0x03 = 20 bits(maximal word length is 24bits);
    0x05 = 22 bits;
    0x09 = 23 bits;
    0x0B = 24 bits;
    0x0D = 21 bits;
*/
#define ANX7150_I2S_WORD_LENGTH 0x02

/*
    Select I2S format if audio HW interface is I2S

    0x00: fist bit shift(philips spec)
    0x01: no shift
*/
#define ANX7150_I2S_SHIFT_CTRL 0x01

/*
    0x00: SD data MSB first
    0x01: LSB first
*/
#define ANX7150_I2S_DIR_CTRL 0x00

/*
    0x00: left polarity when word select is low
    0x01: left polarity when word select is high
*/
#define ANX7150_I2S_WS_POL 0x01

/*
    0x00: data is left justified
    0x01: data is right justified
*/
#define ANX7150_I2S_JUST_CTRL 0x00

/*
    Select MCLK and Fs relationship if audio HW interface is I2S

    0x00: MCLK = 128 * Fs
    0x01: MCLK = 256 * Fs
    0x02: MCLK = 384 * Fs
    0x03: MCLK = 512 * Fs
*/
#define ANX7150_MCLK_Fs_RELATION 0x01

/*
    Select I2S channel numbers if audio HW interface is I2S

    0x00: Disable
    0x01: Enable channel input;
*/
#define ANX7150_I2S_CH0_ENABLE 0x01
#define ANX7150_I2S_CH1_ENABLE 0x00
#define ANX7150_I2S_CH2_ENABLE 0x00
#define ANX7150_I2S_CH3_ENABLE 0x00
//@}


// Video Interface ID NVT support 3&7 only
#define ANX7150_RGB_YCrCb444_SepSync        0
#define ANX7150_YCrCb422_SepSync            1
#define ANX7150_YCrCb422_EmbSync            2
#define ANX7150_YCMux422_SepSync_Mode1      3// NVT: Support CCIR601 8bit
#define ANX7150_YCMux422_SepSync_Mode2      4
#define ANX7150_YCMux422_EmbSync_Mode1      5
#define ANX7150_YCMux422_EmbSync_Mode2      6
#define ANX7150_RGB_YCrCb444_DDR_SepSync    7// NVT: Support CCIR601 8bit DDR
#define ANX7150_RGB_YCrCb444_DDR_EmbSync    8
#define ANX7150_RGB_YCrCb444_SepSync_No_DE  9
#define ANX7150_YCrCb422_SepSync_No_DE      10

#define HDMITX_EDIDNUM                  34

//
// I2C Clock = PCLK/ (DIV*2+4+GSR)
//           = 48Mhz / (237*2+4+4) = 99.58Khz
//
#define ANX7150_I2C_DIV     237
#define ANX7150_I2C_TSR     10
#define ANX7150_I2C_GSR     4


// HDMI-TX IP Register Bank Address
#define ANX7150_REGBANK0    0x72
#define ANX7150_REGBANK1    0x7A

#define ANX7150_READ        0x01
#define ANX7150_WRITE       0x00

#define ANX7150_RGB         0x00
#define ANX7150_YCbCr422    0x01
#define ANX7150_YCbCr444    0x02

#define ANX7150_CSC_BT709   1
#define ANX7150_CSC_BT601   0

// fs -> N(ACR)
#define ANX7150_N_32k   0x1000
#define ANX7150_N_44k   0x1880
#define ANX7150_N_88k   0x3100
#define ANX7150_N_176k  0x6200
#define ANX7150_N_48k   0x1800
#define ANX7150_N_96k   0x3000
#define ANX7150_N_192k  0x6000

// audio type
#define ANX7150_i2s_input           0x01
#define ANX7150_spdif_input         0x02
#define ANX7150_super_audio_input   0x04

#define input_pixel_clk_1x_repeatition 0x00
#define input_pixel_clk_2x_repeatition 0x01
#define input_pixel_clk_4x_repeatition 0x03

#define YCbCr422        0x20
#define null            0
#define source_ratio    0x08


#define ANX7150_avi_sel     0x01
#define ANX7150_audio_sel   0x02
#define ANX7150_spd_sel     0x04
#define ANX7150_mpeg_sel    0x08
#define ANX7150_acp_sel     0x10
#define ANX7150_isrc1_sel   0x20
#define ANX7150_isrc2_sel   0x40
#define ANX7150_vendor_sel  0x80



typedef enum
{
    ANX7150_avi_infoframe,
    ANX7150_audio_infoframe
}packet_type;

typedef struct
{
    UINT8 type;
    UINT8 version;
    UINT8 length;
    UINT8 pb_byte[28];
}infoframe_struct;


typedef struct
{
    UINT32              packets_need_config;//which infoframe packet is need updated
    infoframe_struct    avi_info;
    infoframe_struct    audio_info;
} config_packets;

typedef struct
{
    UINT8 audio_channel;
    UINT8 i2s_format;
    UINT8 i2s_swap;
    UINT8 Channel_status1;
    UINT8 Channel_status2;
    UINT8 Channel_status3;
    UINT8 Channel_status4;
    UINT8 Channel_status5;
} i2s_config_struct;

typedef struct
{
    UINT8 one_BIT_ctrl;

} super_audio_config_struct;

typedef struct
{
    UINT8 audio_type;            // audio type
                                // #define ANX7150_i2s_input 0x01
                                // #define ANX7150_spdif_input 0x02
                                // #define ANX7150_super_audio_input 0x04

    UINT8 down_sample;     // 0x72:0x50
                                // 0x00:    00  no down sample
                                // 0x20:    01  2 to 1 down sample
                                // 0x60:    11  4 to 1 down sample
                                // 0x40:    10  reserved

    i2s_config_struct i2s_config;
    super_audio_config_struct super_audio_config;

} audio_config_struct;


#define ANX7150_Progressive 0
#define ANX7150_Interlace 0x08
#define ANX7150_Neg_Hsync_pol 0x20
#define ANX7150_Pos_Hsync_pol 0
#define ANX7150_Neg_Vsync_pol 0x40
#define ANX7150_Pos_Vsync_pol 0

struct ANX7150_video_timingtype{ //CEA-861C format
    UINT8 ANX7150_640x480p_60Hz[18];//format 1
    UINT8 ANX7150_720x480p_60Hz[18];//format 2 & 3
    UINT8 ANX7150_1280x720p_60Hz[18];//format 4
    UINT8 ANX7150_1920x1080i_60Hz[18];//format 5
    UINT8 ANX7150_720x480i_60Hz[18];//format 6 & 7
    //BYTE ANX7150_720x240p_60Hz[18];//format 8 & 9
    //BYTE ANX7150_2880x480i_60Hz[18];//format 10 & 11
    //BYTE ANX7150_2880x240p_60Hz[18];//format 12 & 13
    //BYTE ANX7150_1440x480p_60Hz[18];//format 14 & 15
    //BYTE ANX7150_1920x1080p_60Hz[18];//format 16
    UINT8 ANX7150_720x576p_50Hz[18];//format 17 & 18
    UINT8 ANX7150_1280x720p_50Hz[18];//format 19
    UINT8 ANX7150_1920x1080i_50Hz[18];//format 20*/
    UINT8 ANX7150_720x576i_50Hz[18];//format 21 & 22
    /* BYTE ANX7150_720x288p_50Hz[18];//formats 23 & 24
    BYTE ANX7150_2880x576i_50Hz[18];//formats 25 & 26
    BYTE ANX7150_2880x288p_50Hz[18];//formats 27 & 28
    BYTE ANX7150_1440x576p_50Hz[18];//formats 29 & 30
    BYTE ANX7150_1920x1080p_50Hz[18];//format 31
    BYTE ANX7150_1920x1080p_24Hz[18];//format 32
    BYTE ANX7150_1920x1080p_25Hz[18];//format 33
    BYTE ANX7150_1920x1080p_30Hz[18];//format 34*/
};




//
//  ANX7150 Registers Definition
//
#define ANX7150_VND_IDL_REG             0x00
#define ANX7150_VND_IDH_REG             0x01
#define ANX7150_DEV_IDL_REG             0x02
#define ANX7150_DEV_IDH_REG             0x03
#define ANX7150_DEV_REV_REG             0x04

#define ANX7150_SRST_REG                0x05
#define ANX7150_TX_RST                  0x40
#define ANX7150_SRST_VIDCAP_RST         0x20    // bit position
#define ANX7150_SRST_AFIFO_RST          0x10   // bit position
#define ANX7150_SRST_HDCP_RST           0x08    // bit position
#define ANX7150_SRST_VID_FIFO_RST       0x04   // bit position
#define ANX7150_SRST_AUD_RST            0x02   // bit position
#define ANX7150_SRST_SW_RST             0x01   // bit position

#define ANX7150_SYS_STATE_REG           0x06
#define ANX7150_SYS_STATE_AUD_CLK_DET   0x20    // bit position
#define ANX7150_SYS_STATE_AVMUTE        0x10   // bit position
#define ANX7150_SYS_STATE_HP            0x08   // bit position
#define ANX7150_SYS_STATE_VSYNC         0x04   // bit position
#define ANX7150_SYS_STATE_CLK_DET       0x02   // bit position
#define ANX7150_SYS_STATE_RSV_DET       0x01   // bit position

#define ANX7150_SYS_CTRL1_REG           0x07
#define ANX7150_SYS_CTRL1_LINKMUTE_EN   0x80    // bit position
#define ANX7150_SYS_CTRL1_HDCPHPD_RST   0x40   // bit position
#define ANX7150_SYS_CTRL1_PDINT_SEL     0x20   // bit position
#define ANX7150_SYS_CTRL1_DDC_FAST      0x10   // bit position
#define ANX7150_SYS_CTRL1_DDC_SWCTRL    0x08    // bit position
#define ANX7150_SYS_CTRL1_HDCPMODE      0x04   // bit position
#define ANX7150_SYS_CTRL1_HDMI          0x02   // bit position
#define ANX7150_SYS_CTRL1_PWDN_CTRL     0x01    // bit position

#define ANX7150_SYS_CTRL2_REG           0x08
#define ANX7150_SYS_CTRL2_DDC_RST       0x08  // bit position
#define ANX7150_SYS_CTRL2_TMDSBIST_RST  0x04  // bit position
#define ANX7150_SYS_CTRL2_MISC_RST      0x02  // bit position
#define ANX7150_SYS_CTRL2_HW_RST        0x01  // bit position

#define ANX7150_SYS_CTRL3_REG           0x09
#define ANX7150_SYS_CTRL3_I2C_PWON      0x02
#define ANX7150_SYS_CTRL3_PWON_ALL      0x01

#define ANX7150_SYS_CTRL4_REG           0x0b

#define ANX7150_VID_STATUS_REG          0x10
#define ANX7150_VID_STATUS_VID_STABLE   0x20   // bit position
#define ANX7150_VID_STATUS_EMSYNC_ERR   0x10    // bit position
#define ANX7150_VID_STATUS_FLD_POL      0x08   // bit position
#define ANX7150_VID_STATUS_TYPE         0x04   // bit position
#define ANX7150_VID_STATUS_VSYNC_POL    0x02   // bit position
#define ANX7150_VID_STATUS_HSYNC_POL    0x01    // bit position

#define ANX7150_VID_MODE_REG            0x11
#define ANX7150_VID_MODE_CHKSHARED_EN   0x80   // bit position
#define ANX7150_VID_MODE_LINKVID_EN     0x40   // bit position
#define ANX7150_VID_MODE_RANGE_Y2R      0x20   // bit position
#define ANX7150_VID_MODE_CSPACE_Y2R     0x10    // bit position
#define ANX7150_VID_MODE_Y2R_SEL        0x08   // bit position
#define ANX7150_VID_MODE_UPSAMPLE       0x04   // bit position

#define ANX7150_VID_CTRL_REG            0x12
#define ANX7150_VID_CTRL_IN_EN          0x10   // bit position
#define ANX7150_VID_CTRL_YCBIT_SEL      0x08   // bit position
#define ANX7150_VID_CTRL_BITCTRL_EN     0x04    // bit position

#define ANX7150_VID_CAPCTRL0_REG            0x13
#define ANX7150_VID_CAPCTRL0_DEGEN_EN       0x80   // bit position
#define ANX7150_VID_CAPCTRL0_EMSYNC_EN      0x40   // bit position
#define ANX7150_VID_CAPCTRL0_DEMUX_EN       0x20   // bit position
#define ANX7150_VID_CAPCTRL0_INV_IDCK       0x10    // bit position
#define ANX7150_VID_CAPCTRL0_DV_BUSMODE     0x08   // bit position
#define ANX7150_VID_CAPCTRL0_DDR_EDGE       0x04   // bit position
#define ANX7150_VID_CAPCTRL0_VIDBIT_SWAP    0x02   // bit position
#define ANX7150_VID_CAPCTRL0_VIDBIST_EN     0x01   // bit position

#define ANX7150_VID_CAPCTRL1_REG            0x14
#define ANX7150_VID_CAPCTRL1_FORMAT_SEL     0x80   // bit position
#define ANX7150_VID_CAPCTRL1_VSYNC_POL      0x40   // bit position
#define ANX7150_VID_CAPCTRL1_HSYNC_POL      0x20   // bit position
#define ANX7150_VID_CAPCTRL1_INV_FLDPOL     0x10    // bit position
#define ANX7150_VID_CAPCTRL1_VID_TYPE       0x08    // bit position

#define ANX7150_H_RESL_REG                  0x15
#define ANX7150_H_RESH_REG                  0x16
#define ANX7150_VID_PIXL_REG                0x17
#define ANX7150_VID_PIXH_REG                0x18
#define ANX7150_H_FRONTPORCHL_REG           0x19
#define ANX7150_H_FRONTPORCHH_REG           0x1A
#define ANX7150_HSYNC_ACT_WIDTHL_REG        0x1B
#define ANX7150_HSYNC_ACT_WIDTHH_REG        0x1C
#define ANX7150_H_BACKPORCHL_REG            0x1D
#define ANX7150_H_BACKPORCHH_REG            0x1E
#define ANX7150_V_RESL_REG                  0x1F
#define ANX7150_V_RESH_REG                  0x20
#define ANX7150_ACT_LINEL_REG               0x21
#define ANX7150_ACT_LINEH_REG               0x22
#define ANX7150_ACT_LINE2VSYNC_REG          0x23
#define ANX7150_VSYNC_WID_REG               0x24
#define ANX7150_VSYNC_TAIL2VIDLINE_REG      0x25
#define ANX7150_VIDF_HRESL_REG              0x26
#define ANX7150_VIDF_HRESH_REG              0x27
#define ANX7150_VIDF_PIXL_REG               0x28
#define ANX7150_VIDF_PIXH_REG               0x29
#define ANX7150_VIDF_HFORNTPORCHL_REG       0x2A
#define ANX7150_VIDF_HFORNTPORCHH_REG       0x2B
#define ANX7150_VIDF_HSYNCWIDL_REG          0x2C
#define ANX7150_VIDF_HSYNCWIDH_REG          0x2D
#define ANX7150_VIDF_HBACKPORCHL_REG        0x2E
#define ANX7150_VIDF_HBACKPORCHH_REG        0x2F
#define ANX7150_VIDF_VRESL_REG              0x30
#define ANX7150_VIDF_VRESH_REG              0x31
#define ANX7150_VIDF_ACTVIDLINEL_REG        0x32
#define ANX7150_VIDF_ACTVIDLINEH_REG        0x33
#define ANX7150_VIDF_ACTLINE2VSYNC_REG      0x34
#define ANX7150_VIDF_VSYNCWIDLINE_REG       0x35
#define ANX7150_VIDF_VSYNCTAIL2VIDLINE_REG  0x36

//Video input data bit control registers

#define VID_BIT_CTRL0   0x37      //added
#define VID_BIT_CTRL1   0x38
#define VID_BIT_CTRL2   0x39
#define VID_BIT_CTRL3   0x3A
#define VID_BIT_CTRL4   0x3B
#define VID_BIT_CTRL5   0x3C
#define VID_BIT_CTRL6   0x3D
#define VID_BIT_CTRL7   0x3E
#define VID_BIT_CTRL8   0x3F
#define VID_BIT_CTRL9   0x48
#define VID_BIT_CTRL10  0x49
#define VID_BIT_CTRL11  0x4A
#define VID_BIT_CTRL12  0x4B
#define VID_BIT_CTRL13  0x4C
#define VID_BIT_CTRL14  0x4D
#define VID_BIT_CTRL15  0x4E
#define VID_BIT_CTRL16  0x4F
#define VID_BIT_CTRL17  0x89
#define VID_BIT_CTRL18  0x8A
#define VID_BIT_CTRL19  0x8B
#define VID_BIT_CTRL20  0x8C
#define VID_BIT_CTRL21  0x8D
#define VID_BIT_CTRL22  0x8E
#define VID_BIT_CTRL23  0x8F


#define ANX7150_INTR_STATE_REG              0x40

#define ANX7150_INTR_CTRL_REG               0x41

#define ANX7150_INTR_CTRL_SOFT_INTR         0x04    // bit position
#define ANX7150_INTR_CTRL_TYPE              0x02    // bit position
#define ANX7150_INTR_CTRL_POL               0x01    // bit position

#define ANX7150_INTR1_STATUS_REG            0x42
#define ANX7150_INTR1_STATUS_CTS_CHG        0x80    // bit position
#define ANX7150_INTR1_STATUS_AFIFO_UNDER    0x40    // bit position
#define ANX7150_INTR1_STATUS_AFIFO_OVER     0x20    // bit position
#define ANX7150_INTR1_STATUS_SPDIF_ERR      0x10    // bit position
#define ANX7150_INTR1_STATUS_SW_INT         0x08    // bit position
#define ANX7150_INTR1_STATUS_HP_CHG         0x04    // bit position
#define ANX7150_INTR1_STATUS_CTS_OVRWR      0x02    // bit position
#define ANX7150_INTR1_STATUS_CLK_CHG        0x01    // bit position

#define ANX7150_INTR2_STATUS_REG            0x43
#define ANX7150_INTR2_STATUS_ENCEN_CHG      0x80    // bit position
#define ANX7150_INTR2_STATUS_HDCPLINK_CHK   0x40    // bit position
#define ANX7150_INTR2_STATUS_HDCPENHC_CHK   0x20    // bit position
#define ANX7150_INTR2_STATUS_BKSV_RDY       0x10    // bit position
#define ANX7150_INTR2_STATUS_PLLLOCK_CHG    0x08    // bit position
#define ANX7150_INTR2_STATUS_SHA_DONE       0x04    // bit position
#define ANX7150_INTR2_STATUS_AUTH_CHG       0x02    // bit position
#define ANX7150_INTR2_STATUS_AUTH_DONE      0x01    // bit position

#define ANX7150_INTR3_STATUS_REG            0x44
#define ANX7150_INTR3_STATUS_SPDIFBI_ERR    0x80    // bit position
#define ANX7150_INTR3_STATUS_VIDF_CHG       0x40    // bit position
#define ANX7150_INTR3_STATUS_AUDCLK_CHG     0x20    // bit position
#define ANX7150_INTR3_STATUS_DDCACC_ERR     0x10    // bit position
#define ANX7150_INTR3_STATUS_DDC_NOACK      0x08    // bit position
#define ANX7150_INTR3_STATUS_VSYNC_DET      0x04    // bit position
#define ANX7150_INTR3_STATUS_RXSEN_CHG      0x02    // bit position
#define ANX7150_INTR3_STATUS_SPDIF_UNSTBL   0x01    // bit position

#define ANX7150_INTR1_MASK_REG              0x45
#define ANX7150_INTR2_MASK_REG              0x46
#define ANX7150_INTR3_MASK_REG              0x47

#define ANX7150_HDMI_AUDCTRL0_REG           0x50
#define ANX7150_HDMI_AUDCTRL0_LAYOUT        0x80    // bit position
#define ANX7150_HDMI_AUDCTRL0_DOWN_SMPL     0x60    // bit position
#define ANX7150_HDMI_AUDCTRL0_CTSGEN_SC     0x10    // bit position
#define ANX7150_HDMI_AUDCTRL0_INV_AUDCLK    0x08    // bit position

#define ANX7150_HDMI_AUDCTRL1_REG           0x51
#define ANX7150_HDMI_AUDCTRL1_IN_EN         0x80    // bit position
#define ANX7150_HDMI_AUDCTRL1_SPDIFIN_EN    0x40    // bit position
#define ANX7150_HDMI_AUDCTRL1_SD3IN_EN      0x20    // bit position
#define ANX7150_HDMI_AUDCTRL1_SD2IN_EN      0x10    // bit position
#define ANX7150_HDMI_AUDCTRL1_SD1IN_EN      0x08    // bit position
#define ANX7150_HDMI_AUDCTRL1_SD0IN_EN      0x04    // bit position
#define ANX7150_HDMI_AUDCTRL1_SPDIFFS_OVRWR 0x02    // bit position
#define ANX7150_HDMI_AUDCTRL1_CLK_SEL       0x01    // bit position

#define ANX7150_I2S_CTRL_REG                0x52
#define ANX7150_I2S_CTRL_VUCP               0x80    // bit position
#define SPDIF_IN_SEL                        0x10    // 0-spdif, 1-multi with sd0
#define ANX7150_I2S_CTRL_SHIFT_CTRL         0x08    // bit position
#define ANX7150_I2S_CTRL_DIR_CTRL           0x04    // bit position
#define ANX7150_I2S_CTRL_WS_POL             0x02    // bit position
#define ANX7150_I2S_CTRL_JUST_CTRL          0x01    // bit position

#define ANX7150_I2SCH_CTRL_REG              0x53
#define ANX7150_I2SCH_FIFO3_SEL             0xC0    // bit position
#define ANX7150_I2SCH_FIFO2_SEL             0x30    // bit position
#define ANX7150_I2SCH_FIFO1_SEL             0x0C    // bit position
#define ANX7150_I2SCH_FIFO0_SEL             0x03    // bit position

#define ANX7150_I2SCH_SWCTRL_REG            0x54

#define ANX7150_I2SCH_SWCTRL_SW3            0x80    // bit position
#define ANX7150_I2SCH_SWCTRL_SW2            0x40    // bit position
#define ANX7150_I2SCH_SWCTRL_SW1            0x20    // bit position
#define ANX7150_I2SCH_SWCTRL_SW0            0x10    // bit position
#define ANX7150_I2SCH_SWCTRL_INWD_LEN       0xE0    // bit position
#define ANX7150_I2SCH_SWCTRL_INWD_MAX       0x01    // bit position

#define ANX7150_SPDIFCH_STATUS_REG          0x55
#define ANX7150_SPDIFCH_STATUS_FS_FREG      0xF0    // bit position
#define ANX7150_SPDIFCH_STATUS_WD_LEN       0x0E    // bit position
#define ANX7150_SPDIFCH_STATUS_WD_MX        0x01    // bit position

#define ANX7150_I2SCH_STATUS1_REG           0x56
#define ANX7150_I2SCH_STATUS1_MODE          0xC0   // bit position
#define ANX7150_I2SCH_STATUS1_PCM_MODE      0x38   // bit position
#define ANX7150_I2SCH_STATUS1_SW_CPRGT      0x04   // bit position
#define ANX7150_I2SCH_STATUS1_NON_PCM       0x02    // bit position
#define ANX7150_I2SCH_STATUS1_PROF_APP      0x01   // bit position

#define ANX7150_I2SCH_STATUS2_REG           0x57

#define ANX7150_I2SCH_STATUS3_REG           0x58
#define ANX7150_I2SCH_STATUS3_CH_NUM        0xF0    // bit position
#define ANX7150_I2SCH_STATUS3_SRC_NUM       0x0F    // bit position



#define ANX7150_I2SCH_STATUS4_REG           0x59
#define ANX7150_I2SCH_STATUS4_44Khz         0x00
#define ANX7150_I2SCH_STATUS4_48Khz         0x02
#define ANX7150_I2SCH_STATUS4_32Khz         0x03


#define ANX7150_I2SCH_STATUS5_REG           0x5A

#define ANX7150_I2SCH_STATUS5_WORD_MAX      0x01 // bit position

#define ANX7150_HDMI_AUDSTATUS_REG          0x5B

#define ANX7150_HDMI_AUDSTATUS_SPDIF_DET    0x01   // bit position

#define ANX7150_HDMI_AUDBIST_CTRL_REG       0x5C

#define ANX7150_HDMI_AUDBIST_EN3            0x08    // bit position
#define ANX7150_HDMI_AUDBIST_EN2            0x04   // bit position
#define ANX7150_HDMI_AUDBIST_EN1            0x02    // bit position
#define ANX7150_HDMI_AUDBIST_EN0            0x01   // bit position

#define ANX7150_AUD_INCLK_CNT_REG           0x5D
#define ANX7150_AUD_DEBUG_STATUS_REG        0x5E

#define ANX7150_ONEBIT_AUD_CTRL_REG         0x60

#define ANX7150_ONEBIT_AUD_CTRL_SEN7        0x80    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN6        0x40    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN5        0x20    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN4        0x10    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN3        0x08    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN2        0x04    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN1        0x02    // bit position
#define ANX7150_ONEBIT_AUD_CTRL_SEN0        0x01    // bit position

#define ANX7150_ONEBIT_AUD0_CTRL_REG        0x61
#define ANX7150_ONEBIT_AUD1_CTRL_REG        0x62
#define ANX7150_ONEBIT_AUD2_CTRL_REG        0x63
#define ANX7150_ONEBIT_AUD3_CTRL_REG        0x64

#define ANX7150_ONEBIT_AUDCLK_CTRL_REG      0x65

#define ANX7150_ONEBIT_AUDCLK_DET           0x08    // bit position

#define ANX7150_SPDIF_ERR_THRSHLD_REG       0x66
#define ANX7150_SPDIF_ERR_CNT_REG           0x67

#define ANX7150_HDMI_LINK_CTRL_REG          0x70

#define ANX7150_HDMI_LINK_DATA_MUTEEN1      0x80    // bit position
#define ANX7150_HDMI_LINK_DATA_MUTEEN0      0x40    // bit position
#define ANX7150_HDMI_LINK_CLK_MUTEEN2       0x20    // bit position
#define ANX7150_HDMI_LINK_CLK_MUTEEN1       0x10    // bit position
#define ANX7150_HDMI_LINK_CLK_MUTEEN0       0x08    // bit position
#define ANX7150_HDMI_LINK_DEC_DE            0x04    // bit position
#define ANX7150_HDMI_LINK_PRMB_INC          0x02    // bit position
#define ANX7150_HDMI_LINK_AUTO_PROG         0x01    // bit position

#define ANX7150_VID_CAPCTRL2_REG            0x71

#define ANX7150_VID_CAPCTRL2_CHK_UPDATEEN   0x10   // bit position

#define ANX7150_LINK_MUTEEE_REG             0x72

#define ANX7150_LINK_MUTEEE_AVMUTE_EN2      0x20    // bit position
#define ANX7150_LINK_MUTEEE_AVMUTE_EN1      0x10    // bit position
#define ANX7150_LINK_MUTEEE_AVMUTE_EN0      0x08    // bit position
#define ANX7150_LINK_MUTEEE_AUDMUTE_EN2     0x04    // bit position
#define ANX7150_LINK_MUTEEE_AUDMUTE_EN1     0x02    // bit position
#define ANX7150_LINK_MUTEEE_AUDMUTE_EN0     0x01    // bit position

#define ANX7150_SERDES_TEST0_REG            0x73
#define ANX7150_SERDES_TEST1_REG            0x74
#define ANX7150_SERDES_TEST2_REG            0x75

#define ANX7150_PLL_TX_AMP                  0x76


#define ANX7150_DDC_SLV_ADDR_REG            0x80
#define ANX7150_DDC_SLV_SEGADDR_REG         0x81
#define ANX7150_DDC_SLV_OFFADDR_REG         0x82
#define ANX7150_DDC_ACC_CMD_REG             0x83
#define ANX7150_DDC_ACCNUM0_REG             0x84
#define ANX7150_DDC_ACCNUM1_REG             0x85

#define ANX7150_DDC_CHSTATUS_REG            0x86

#define ANX7150_DDC_CHSTATUS_DDCERR         0x80    // bit position
#define ANX7150_DDC_CHSTATUS_DDC_OCCUPY     0x40    // bit position
#define ANX7150_DDC_CHSTATUS_FIFO_FULL      0x20    // bit position
#define ANX7150_DDC_CHSTATUS_FIFO_EMPT      0x10    // bit position
#define ANX7150_DDC_CHSTATUS_NOACK          0x08    // bit position
#define ANX7150_DDC_CHSTATUS_FIFO_RD        0x04    // bit position
#define ANX7150_DDC_CHSTATUS_FIFO_WR        0x02    // bit position
#define ANX7150_DDC_CHSTATUS_INPRO          0x01    // bit position

#define ANX7150_DDC_FIFO_ACC_REG            0x87
#define ANX7150_DDC_FIFOCNT_REG             0x88

#define ANX7150_SYS_PD_REG                  0x90
#define ANX7150_SYS_PD_PLL                  0x80    // bit position
#define ANX7150_SYS_PD_TMDS                 0x40    // bit position
#define ANX7150_SYS_PD_TMDS_CLK             0x20    // bit position
#define ANX7150_SYS_PD_MISC                 0x10    // bit position
#define ANX7150_SYS_PD_LINK                 0x08    // bit position
#define ANX7150_SYS_PD_IDCK                 0x04    // bit position
#define ANX7150_SYS_PD_AUD                  0x02    // bit position
#define ANX7150_SYS_PD_MACRO_ALL            0x01    // bit position

#define ANX7150_LINKFSM_DEBUG0_REG          0x91
#define ANX7150_LINKFSM_DEBUG1_REG          0x92

#define ANX7150_PLL_CTRL0_REG               0x93
#define ANX7150_PLL_CTRL0_CPREG_BLEED       0x02    // bit position
#define ANX7150_PLL_CTRL0_TEST_EN           0x01    // bit position

#define ANX7150_PLL_CTRL1_REG               0x94
#define ANX7150_PLL_CTRL1_TESTEN            0x80    // bit position

#define ANX7150_OSC_CTRL_REG                0x95
#define ANX7150_OSC_CTRL_TESTEN             0x80    // bit position
#define ANX7150_OSC_CTRL_SEL_BG             0x40    // bit position

#define ANX7150_TMDS_CH0_CONFIG_REG         0x96
#define ANX7150_TMDS_CH0_TESTEN             0x20    // bit position
#define ANX7150_TMDS_CH0_AMP                0x1C    // bit position
#define ANX7150_TMDS_CHO_EMP                0x03    // bit position

#define ANX7150_TMDS_CH1_CONFIG_REG         0x97
#define ANX7150_TMDS_CH1_TESTEN             0x20    // bit position
#define ANX7150_TMDS_CH1_AMP                0x1C    // bit position
#define ANX7150_TMDS_CH1_EMP                0x03    // bit position

#define ANX7150_TMDS_CH2_CONFIG_REG         0x98
#define ANX7150_TMDS_CH2_TESTEN             0x20    // bit position
#define ANX7150_TMDS_CH2_AMP                0x1C    // bit position
#define ANX7150_TMDS_CH2_EMP                0x03    // bit position

#define ANX7150_TMDS_CLKCH_CONFIG_REG       0x99
#define ANX7150_TMDS_CLKCH_MUTE             0x80    // bit position
#define ANX7150_TMDS_CLKCH_TESTEN           0x08    // bit position
#define ANX7150_TMDS_CLKCH_AMP              0x07    // bit position

#define ANX7150_CHIP_CTRL_REG               0x9A
#define ANX7150_CHIP_CTRL_PRBS_GENEN        0x80    // bit position
#define ANX7150_CHIP_CTRL_LINK_DBGSEL       0x70    // bit position
#define ANX7150_CHIP_CTRL_VIDCHK_EN         0x08    // bit position
#define ANX7150_CHIP_CTRL_MISC_TIMER        0x04    // bit position
#define ANX7150_CHIP_CTRL_PLL_RNG           0x02    // bit position
#define ANX7150_CHIP_CTRL_PLL_MAN           0x01    // bit position

#define ANX7150_CHIP_STATUS_REG             0x9B
#define ANX7150_CHIP_STATUS_GPIO            0x80    // bit position
#define ANX7150_CHIP_STATUS_SDA             0x40    // bit position
#define ANX7150_CHIP_STATUS_SCL             0x20    // bit position
#define ANX7150_CHIP_STATUS_PLL_HSPO        0x04    // bit position
#define ANX7150_CHIP_STATUS_PLL_LOCK        0x02    // bit position
#define ANX7150_CHIP_STATUS_MISC_LOCK       0x01    // bit position

#define ANX7150_DBG_PINGPIO_CTRL_REG        0x9C
#define ANX7150_DBG_PINGPIO_VDLOW_SHAREDEN  0x04    // bit position
#define ANX7150_DBG_PINGPIO_GPIO_ADDREN     0x02    // bit position
#define ANX7150_DBG_PINGPIO_GPIO_OUT        0x01    // bit position

#define ANX7150_CHIP_DEBUG0_CTRL_REG        0x9D
#define ANX7150_CHIP_DEBUG0_PRBS_ERR        0xE0       // bit position
#define ANX7150_CHIP_DEBUG0_CAPST           0x1F       // bit position

#define ANX7150_CHIP_DEBUG1_CTRL_REG        0x9E
#define ANX7150_CHIP_DEBUG1_SDA_SW          0x80    // bit position
#define ANX7150_CHIP_DEBUG1_SCL_SW          0x40    // bit position
#define ANX7150_CHIP_DEBUG1_SERDES_TESTEN   0x20    // bit position
#define ANX7150_CHIP_DEBUG1_CLK_BYPASS      0x10    // bit position
#define ANX7150_CHIP_DEBUG1_FORCE_PLLLOCK   0x08    // bit position
#define ANX7150_CHIP_DEBUG1_PLLLOCK_BYPASS  0x04    // bit position
#define ANX7150_CHIP_DEBUG1_FORCE_HP        0x02    // bit position
#define ANX7150_CHIP_DEBUG1_HP_DEGLITCH     0x01    // bit position

#define ANX7150_CHIP_DEBUG2_CTRL_REG        0x9F
#define ANX7150_CHIP_DEBUG2_EXEMB_SYNCEN    0x04    // bit position
#define ANX7150_CHIP_DEBUG2_VIDBIST         0x02    // bit position

#define ANX7150_VID_INCLK_REG               0x5F

#define ANX7150_HDCP_STATUS_REG             0xA0
#define ANX7150_HDCP_STATUS_ADV_CIPHER      0x80    // bit position
#define ANX7150_HDCP_STATUS_R0_READY        0x10    // bit position
#define ANX7150_HDCP_STATUS_AKSV_ACT        0x08    // bit position
#define ANX7150_HDCP_STATUS_ENCRYPT         0x04    // bit position
#define ANX7150_HDCP_STATUS_AUTH_PASS       0x02    // bit position
#define ANX7150_HDCP_STATUS_KEY_DONE        0x01    // bit position

#define ANX7150_HDCP_CTRL0_REG              0xA1
#define ANX7150_HDCP_CTRL0_STORE_AN         0x80    // bit position
#define ANX7150_HDCP_CTRL0_RX_REP           0x40    // bit position
#define ANX7150_HDCP_CTRL0_RE_AUTH          0x20    // bit position
#define ANX7150_HDCP_CTRL0_SW_AUTHOK        0x10    // bit position
#define ANX7150_HDCP_CTRL0_HW_AUTHEN        0x08    // bit position
#define ANX7150_HDCP_CTRL0_ENC_EN           0x04    // bit position
#define ANX7150_HDCP_CTRL0_BKSV_SRM         0x02    // bit position
#define ANX7150_HDCP_CTRL0_KSV_VLD          0x01    // bit position

#define ANX7150_HDCP_CTRL1_REG              0xA2
#define ANX7150_LINK_CHK_12_EN              0x40
#define ANX7150_HDCP_CTRL1_DDC_NOSTOP       0x20    // bit position
#define ANX7150_HDCP_CTRL1_DDC_NOACK        0x10    // bit position
#define ANX7150_HDCP_CTRL1_EDDC_NOACK       0x08    // bit position
#define ANX7150_HDCP_CTRL1_BLUE_SCREEN_EN   0x04    // bit position
#define ANX7150_HDCP_CTRL1_RCV11_EN         0x02    // bit position
#define ANX7150_HDCP_CTRL1_HDCP11_EN        0x01    // bit position

#define ANX7150_HDCP_Link_Check_FRAME_NUM_REG   0xA3
#define ANX7150_HDCP_AKSV1_REG                  0xA5
#define ANX7150_HDCP_AKSV2_REG                  0xA6
#define ANX7150_HDCP_AKSV3_REG                  0xA7
#define ANX7150_HDCP_AKSV4_REG                  0xA8
#define ANX7150_HDCP_AKSV5_REG                  0xA9

#define ANX7150_HDCP_AN1_REG                0xAA
#define ANX7150_HDCP_AN2_REG                0xAB
#define ANX7150_HDCP_AN3_REG                0xAC
#define ANX7150_HDCP_AN4_REG                0xAD
#define ANX7150_HDCP_AN5_REG                0xAE
#define ANX7150_HDCP_AN6_REG                0xAF
#define ANX7150_HDCP_AN7_REG                0xB0
#define ANX7150_HDCP_AN8_REG                0xB1

#define ANX7150_HDCP_BKSV1_REG              0xB2
#define ANX7150_HDCP_BKSV2_REG              0xB3
#define ANX7150_HDCP_BKSV3_REG              0xB4
#define ANX7150_HDCP_BKSV4_REG              0xB5
#define ANX7150_HDCP_BKSV5_REG              0xB6

#define ANX7150_HDCP_RI1_REG                0xB7
#define ANX7150_HDCP_RI2_REG                0xB8

#define ANX7150_HDCP_PJ_REG                 0xB9
#define ANX7150_HDCP_RX_CAPS_REG            0xBA
#define ANX7150_HDCP_BSTATUS0_REG           0xBB
#define ANX7150_HDCP_BSTATUS1_REG           0xBC

#define ANX7150_HDCP_AMO0_REG               0xD0
#define ANX7150_HDCP_AMO1_REG               0xD1
#define ANX7150_HDCP_AMO2_REG               0xD2
#define ANX7150_HDCP_AMO3_REG               0xD3
#define ANX7150_HDCP_AMO4_REG               0xD4
#define ANX7150_HDCP_AMO5_REG               0xD5
#define ANX7150_HDCP_AMO6_REG               0xD6
#define ANX7150_HDCP_AMO7_REG               0xD7

#define ANX7150_HDCP_DBG_CTRL_REG           0xBD

#define ANX7150_HDCP_DBG_ENC_INC            0x08    // bit position
#define ANX7150_HDCP_DBG_DDC_SPEED          0x06    // bit position
#define ANX7150_HDCP_DBG_SKIP_RPT           0x01    // bit position

#define ANX7150_HDCP_KEY_STATUS_REG         0xBE
#define ANX7150_HDCP_KEY_BIST_EN            0x04    // bit position
#define ANX7150_HDCP_KEY_BIST_ERR           0x02    // bit position
#define ANX7150_HDCP_KEY_CMD_DONE           0x01    // bit position

#define ANX7150_KEY_CMD_REGISTER            0xBF   //added

#define ANX7150_HDCP_AUTHDBG_STATUS_REG     0xC7
#define ANX7150_HDCP_ENCRYPTDBG_STATUS_REG  0xC8
#define ANX7150_HDCP_FRAME_NUM_REG          0xC9

#define ANX7150_DDC_MSTR_INTER_REG          0xCA
#define ANX7150_DDC_MSTR_LINK_REG           0xCB

#define ANX7150_HDCP_BLUESCREEN0_REG        0xCC
#define ANX7150_HDCP_BLUESCREEN1_REG        0xCD
#define ANX7150_HDCP_BLUESCREEN2_REG        0xCE
//  DEV_ADDR = 0x7A or 0x7E
#define ANX7150_INFO_PKTCTRL1_REG           0xC0
#define ANX7150_INFO_PKTCTRL1_SPD_RPT       0x80    // bit position
#define ANX7150_INFO_PKTCTRL1_SPD_EN        0x40    // bit position
#define ANX7150_INFO_PKTCTRL1_AVI_RPT       0x20    // bit position
#define ANX7150_INFO_PKTCTRL1_AVI_EN        0x10    // bit position
#define ANX7150_INFO_PKTCTRL1_GCP_RPT       0x08    // bit position
#define ANX7150_INFO_PKTCTRL1_GCP_EN        0x04    // bit position
#define ANX7150_INFO_PKTCTRL1_ACR_NEW       0x02    // bit position
#define ANX7150_INFO_PKTCTRL1_ACR_EN        0x01    // bit position

#define ANX7150_INFO_PKTCTRL2_REG           0xC1
#define ANX7150_INFO_PKTCTRL2_UD1_RPT       0x80    // bit position
#define ANX7150_INFO_PKTCTRL2_UD1_EN        0x40    // bit position
#define ANX7150_INFO_PKTCTRL2_UD0_RPT       0x20    // bit position
#define ANX7150_INFO_PKTCTRL2_UD0_EN        0x10    // bit position
#define ANX7150_INFO_PKTCTRL2_MPEG_RPT      0x08    // bit position
#define ANX7150_INFO_PKTCTRL2_MPEG_EN       0x04    // bit position
#define ANX7150_INFO_PKTCTRL2_AIF_RPT       0x02    // bit position
#define ANX7150_INFO_PKTCTRL2_AIF_EN        0x01    // bit position

#define ANX7150_ACR_N1_SW_REG               0xC2
#define ANX7150_ACR_N2_SW_REG               0xC3
#define ANX7150_ACR_N3_SW_REG               0xC4

#define ANX7150_ACR_CTS1_SW_REG             0xC5
#define ANX7150_ACR_CTS2_SW_REG             0xC6
#define ANX7150_ACR_CTS3_SW_REG             0xC7

#define ANX7150_ACR_CTS1_HW_REG             0xC8
#define ANX7150_ACR_CTS2_HW_REG             0xC9
#define ANX7150_ACR_CTS3_HW_REG             0xCA

#define ANX7150_ACR_CTS_CTRL_REG            0xCB

#define ANX7150_GNRL_CTRL_PKT_REG           0xCC
#define ANX7150_GNRL_CTRL_CLR_AVMUTE        0x02    // bit position
#define ANX7150_GNRL_CTRL_SET_AVMUTE        0x01    // bit position

#define ANX7150_AUD_PKT_FLATCTRL_REG        0xCD
#define ANX7150_AUD_PKT_AUTOFLAT_EN         0x80    // bit position
#define ANX7150_AUD_PKT_FLAT                0x07    // bit position




#endif
