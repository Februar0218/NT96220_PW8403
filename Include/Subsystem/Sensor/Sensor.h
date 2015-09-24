/**
    @file       sensor.h

    @brief      Define sensor interface function

    @note      Sensor Driver APIs

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
 #ifndef _SENSOR_H
#define _SENSOR_H

//----------Header file include-------------------------------------------------
#include "Type.h"
#include "sie_lib.h"

/**
     Sensor mode
*/
typedef enum _SENSOR_MODE
{
    SENSOR_MODE_UNKNOWN     = 0,    ///< Unknow Mode
    SENSOR_MODE_640X480     = 1,    ///< VGA
    SENSOR_MODE_1280X720    = 2,    ///< 720P
    SENSOR_MODE_1280X960    = 3,    ///< 1280x960
    SENSOR_MODE_1280X480    = 4,    ///< 1280x480
    SENSOR_MODE_1440X1080   = 5,    ///< 1440x1080
    SENSOR_MODE_2560X960    = 6,    ///< 2560x960
    SENSOR_MODE_FULL        = 7,    ///< full resolution
    SENSOR_MODE_1440X540    = 8,    ///< 1440x540
    SENSOR_MODE_1920X1080    = 9,    ///< 1920x1080
    SENSOR_MODE_1280X720P60    = 10,    ///< 720P60
    SENSOR_MODE_1024X768    = 11,   /// XGA  // added for MI3130 preview mode [NTK#RandyHo]<2012/04/13>
    SENSOR_MODE_320X240    = 12,    ///< 720P60
    SENSOR_MODE_320X240P60    = 13,    ///< 720P60
    SENSOR_MODE_320X240P120    = 14,    ///< 720P60
    SENSOR_MODE_640X480P60    = 15,    ///< 720P60
    SENSOR_MODE_960X720    = 16,    ///< 960X720   //#NTK#2012/06/14#Robin Chen
    SENSOR_MODE_1280X720P50 = 17,    ///< 720P60
    ENUM_DUMMY4WORD(SENSOR_MODE)
} SENSOR_MODE, *PSENSOR_MODE;

/**
     Sensor ouput ratio
*/
typedef enum _SENSOR_RATIO
{
    SENSOR_RATIO_9_16,
    SENSOR_RATIO_2_3,
    SENSOR_RATIO_3_4,
    SENSOR_RATIO_1_1,
    SENSOR_RATIO_4_3,
    SENSOR_RATIO_3_2,
    SENSOR_RATIO_16_9,
    SENSOR_RATIO_Num,
    ENUM_DUMMY4WORD(SENSOR_RATIO)
} SENSOR_RATIO, *PSENSOR_RATIO;

/**
     Sensor regiser command structure
*/
typedef struct _SENSOR_CMD
{
    UINT32 uiAddr;       ///< address
    UINT32 uiData;       ///< data
} SENSOR_CMD,*PSENSOR_CMD;

/**
 * SIE sync setting
 */
typedef struct
{
    //active area info
    UINT32 Act_StartX;      ///<
    UINT32 Act_StartY;      ///<
    UINT32 Act_CenterX;     ///<
    UINT32 Act_CenterY;     ///<
    UINT32 Act_SizeX;     ///<
    UINT32 Act_SizeY;     ///<

    //STR_DIV *hd_DivisionSeq;///< input division sequence at capture mode
    //UINT32 *hd_DivisionNum; ///< total number of input divisions at capture mode
    //STR_HDVD HDVD_Sync;     ///< sync and total periods of VD and HD
    UINT32 HD_sync;         ///< HD sync period
    UINT32 VD_sync;         ///< VD sync period
    UINT32 HD_total;        ///< HD total period
    UINT32 VD_total;        ///< VD total period

    //STR_OB_Param ob_area;   ///<
    UINT32 OB_X0;           ///< left end of the window
    UINT32 OB_Y0;           ///< top of the window
    UINT32 OB_X1;           ///< right end of the window
    UINT32 OB_Y1;           ///< bottom of the window
    UINT32 OB_Thres;        ///< calculation threshld. if pixel value < threshld, it will be taken into account.
    UINT32 OB_SubSample;    ///< horizontal subsample rate: 1/(1+subsample).
    UINT32 OB_Gain;         ///<
    UINT32 OB_Offset;       ///<

    UINT8 padding;          ///<
} SYNC_INFO, *PSYNC_INFO;

/**
 * Preview data info
 */
typedef struct
{
    UINT32 uiFps;               ///< 1: 15, 2: 30, 3: 60
    UINT32 uiBayer;             ///< RG, or BG, or GR, or GB
    UINT32 uiBinning;           ///< binning cappbility
    UINT32 uiSkip;              ///< skip capability
    UINT32 uiPclk;              ///< pclk (unit:MHz)
    //#NT#2011/11/23#Ethan Lau -begin
    //SYNC_INFO *pPrvSyncInfo;    ///<
    //SENSOR_CMD *pCmd;           ///< command table
    //UINT32 uiCmdNum;            ///< total command number
    //#NT#2011/11/23#Ethan Lau -end
} PREVIEW_DATA_INFO, *PPREVIEW_DATA_INFO;

/**
 * Capture data info
 */
typedef struct
{
    UINT32 uiWidth;             ///< width
    UINT32 uiHeight;            ///< height
    UINT32 uiBayer;             ///< RG, or BG, or GR, or GB
    UINT32 uiPclk;              ///< pclk (unit:MHz)
    //STR_FLDNUM *RemapFieldNum;  ///< input field sequence at capture mode
    //#NT#2011/11/23#Ethan Lau -begin
    //SYNC_INFO *pCapSyncInfo;    ///<
    //SENSOR_CMD *pCmd;           ///< command table
    //UINT32 uiCmdNum;            ///< total command number
    //#NT#2011/11/23#Ethan Lau -end
} CAPTURE_DATA_INFO, *PCAPTURE_DATA_INFO;

/**
    sensor device info
*/
typedef struct
{
    UINT32 uiType;                  ///< 0: CCD, 1: CMOS
    UINT32 uiSenID;                 ///< sensor id
    UINT32 uiCellWidth;             ///< cell size, width, micro-meter * 100, eg. 1.75um -> 175
    UINT32 uiCellHeight;            ///< cell size, width, micro-meter * 100, eg. 1.75um -> 175
    PREVIEW_DATA_INFO *pPrvMode;    ///< preview mode
    CAPTURE_DATA_INFO *pStillMode;  ///< still mode
} SENSOR_INFO, *PSENSOR_INFO;

/**
    sensor gain info
*/
typedef struct
{
    UINT8 max_db;               ///<
    UINT8 min_db;               ///<

    UINT16 double_gain_step;    ///< the steps per "double gain increment"
    UINT16 db_step;             ///< minimum db increment

    UINT16 ref_gain;            ///< reference gain e.g. iso100 agc gain
} SENSOR_GAIN_INFO, *PSENSOR_GAIN_INFO;

/**
    sensor eshutter info
*/
typedef struct
{
    UINT32 uiFrame;         ///< exposure frame
    UINT32 uiLine;          ///< exposure line
    UINT32 uiPixel;         ///< exposure pixel
    UINT32 uiGain;          ///< agc gain

    UINT32 uiVDLength;      ///< vd length
    UINT32 uiSubCount;      ///< hd sub count
    UINT32 uiExpoRatio;     ///< exposure ratio
} EXPOSURE_SETTING, *PEXPOSURE_SETTING;

/**
    sensor status
*/
typedef struct
{
    UINT32 uiSensorMode;    ///< current sensor mode
    UINT32 uiReadoutMode;   ///< sensor readout mode
    UINT32 uiRatio;         ///< sensor output ratio
    UINT32 uiBayer;         ///< bayer start pixel
    //#NT#2011/11/23#Ethan Lau -begin
    UINT32 uiBinning;       ///< binning cappbility
    UINT32 uiSkip;          ///< skip capability
    //#NT#2011/11/23#Ethan Lau -end
    UINT32 uiPclk;          ///< pixel clock
    SYNC_INFO *pSyncInfo;   ///< current sync info
} SENSOR_STATUS, *PSENSOR_STATUS;

typedef struct
{
    //Common
    void    (*fpSifInit)(UINT32*);
    void    (*fpInit)(void);
    void    (*fpPwron)(UINT32);
    void    (*fpPwroff)(UINT32);
    void    (*fpSleep)(UINT32);
    void    (*fpWakeup)(UINT32);
    void    (*fpWrite)(UINT32, UINT32);
    void    (*fpRead)(UINT32, UINT32*);
    //#NT#2011/11/23#Ethan Lau -begin
    void    (*fpGetStatus)(UINT32,PSENSOR_STATUS);
    void    (*fpGetCurStatus)(PSENSOR_STATUS);
    //#NT#2011/11/23#Ethan Lau -end
    void    (*fpChgMode)(UINT32);
    //#NT#2011/10/26#Ethan Lau -begin
    //#NT#add sensor feature
    void    (*fpFlip)(UINT32);
    //#NT#2011/10/26#Ethan Lau -end
    //AE
    void    (*fpGetExpo)(EXPOSURE_SETTING*);
    void    (*fpCalLine)(UINT32);
    void    (*fpCalGain)(UINT32);
    void    (*fpSetFrame)(UINT32);
    void    (*fpSetLine)(UINT32);
    void    (*fpSetPixel)(UINT32);
    void    (*fpSetVdLength)(UINT32);
    void    (*fpSetGain)(UINT32);
    //#NT#2012/05/28#KS hung -begin
    //#NT#For M-shutter calibrarion
    BOOL    (*fpcheckExposureLongTime)(void);
    void    (*fpSetCalMshutterDelayTime)(UINT32);

    UINT32  (*fpGetCalMshutterDelayTime)(void);
    void    (*fpTriggerGlobalReset)(void);
    void    (*fpClearGlobalReset)(void);
    //#NT#2012/05/28#KS hung -end
} SENSOR_TAB, *PSENSOR_TAB;

typedef struct
{
    //#NT#2011/11/23#Ethan Lau -begin
    //#NT#removed
    //PSENSOR_TAB pSensorDrv;         ///< sensor driver
    //PSENSOR_INFO pSensorInfo;       ///< sensor information
    //#NT#2011/11/23#Ethan Lau -end
    BOOL bSieMClkEn;                ///< Enable/Disable MCLK to sensor
    UINT32 uiMClkSrc;               ///< config MCLK source
    UINT32 uiMClkDiv;               ///< config MCLK frequency
    UINT32 uiSifch[2];              ///< sif channel.   When use i2c,uiSifch[0] as i2c clk,
                                    ///<                uiSifch[0] = 1=>50 , 2=>100 , 3=>200 , 4=>400 KHZ
    UINT32 uiPrvMode;               ///< preview mode
} SENSOR_APPOBJ, *PSENSOR_APPOBJ;

typedef enum
{
    sen_Flip_NULL  = 0,
    sen_Flip_H     = 1,
    sen_Flip_V     = 2,
    sen_Flip_H_V   = 3,
    sen_Flip_MAX   = 4,
    ENUM_DUMMY4WORD(SEN_FLIP)
}SEN_FLIP;

/**
    General Sensor APIs
*/
/**
    Initialize sensor

    @param[in] SensorObj     Sensor object
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_Open(PSENSOR_APPOBJ pSensorCtrlObj);

/**
    Turn on sensor step by step

    @param[in] uiStage    power on stage
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_PwrOn(UINT32 uiStage);

/**
    Turn off sensor step by step

    @param[in] uiStage    power off stage
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_PwrOff(UINT32 uiStage);

/**
    Set sensor to standby state

    @param[in] uiStage    standby flow stage
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_Sleep(UINT32 uiStage);

/**
    Resume the sensor from the standby

    @param[in] uiStage
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_WakeUp(UINT32 uiStage);

/**
    configure sensor register

    @param[in] uiAddr    register address
    @param[in] uiValue   desire value
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_WriteReg(UINT32 uiAddr, UINT32 uiValue);

/**
    read sensor register

    @param[in] uiAddr    register address
    @param[out] uiValue  register setting
    @return - 0 suceess, < 0 failed
 */
extern UINT32 Sensor_ReadReg(UINT32 uiAddr);

/**
    Set the current operation mode

    @param[in] uiMode   change sensor mode
    @return - 0 suceess, < 0 failed
 */
extern void Sensor_ChgMode(UINT8 uiMode);

/**
    set exposure time

    @param[in]  time - exposure time (unit : us)
    @return - 0 suceess, < 0 failed
*/
extern void Sensor_SetExpTime(UINT32 uiTime);

/**
    calculate and set agc gain

    @param[in]  uiIso - iso value (unit : ISO speed)
    @return - 0 suceess, < 0 failed
*/
extern void Sensor_SetISO(UINT32 uiIso);

/**
    set sensor agc gain

    @param[in]  uiGain - agc gain (unit : correspond to sensor AGC gain)
    @return - 0 suceess, < 0 failed
*/
extern void Sensor_SetGain(UINT32 uiGain);

/**
    Get current status of sensor

    @param[in] *SensorStatus - pointer to specification mode status
    @param[in] uiSenMode - sensor mode
    @return
 */
extern void Sensor_GetStatus(PSENSOR_STATUS pSensorStatus);

/**
    Get idicate status of sensor

    @param[in] *SensorStatus - pointer to specification mode status
    @param[in] uiSenMode - sensor mode
    @return
 */
extern void Sensor_GetStatus_Indicated(UINT32 uiSenMode,PSENSOR_STATUS pSensorStatus);

/**
    Get current exposure setting

    @param
    @return - pointer to current exposure setting
 */
extern void Sensor_GetExpoSetting(EXPOSURE_SETTING *pExpo);

/**
     patch flip registor into sensor registor set,
     flip will be worked at next sensor chage mode operation

     @param[in] direction       flip direction
     @return error msg
*/
extern ER Sensor_Flip(SEN_FLIP direction);


extern BOOL Sensor_checkExposureLongTime(void);
extern void Sensor_SetCalMshutterDelayTime(UINT32 DelayTime);
extern UINT32 Sensor_GetCalMshutterDelayTime(void);
extern void Sensor_TriggerGlobalReset(void);
extern void Sensor_ClearGlobalReset(void);

#endif
