/**
    Public header file for IPL command.

    This file is the header file that define the API and data type for IPL command.

    @file       Alg_IPLMode.h
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

/** \addtogroup  */
//@{
#ifndef _ALG_IPLMODE_H
#define _ALG_IPLMODE_H
#include "imagealginfor.h"
#include "Type.h"

/** \addtogroup mISYSAlg */
//@{

#define IPL_CMD_CHG_MODE        0x00000001      ///< change mode command.
#define IPL_CMD_SET_DZOOM       0x00000008      ///< change digital zoom command.
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
#define IPL_CMD_SLEEP           0x00000010      ///< enter sleep mode command.
#define IPL_CMD_WAKE_UP         0x00000020      ///< leave sleep mode command.
//#NT#2009/10/08#Bowen Li -end

//#NT#2009/10/29#Bowen Li -begin
//#NT#expand enum size to word (4 bytes)
typedef enum
{
    IPL_MODE_OFF = 0,           ///< IPL off mode.
    IPL_MODE_IDLE = 1,          ///< IPL idle mode.
    IPL_MODE_PREVIEW = 2,       ///< IPL photo mode.
    IPL_MODE_VIDEO = 3,         ///< IPL video mode.
    IPL_MODE_VIDEOREC = 4,      ///< IPL video recording mode.
    IPL_MODE_PCC = 5,           ///< IPL pcc mode.
    IPL_MODE_CAP = 6,           ///< IPL capture mode.
    IPL_MODE_NUM = 7,
    ENUM_DUMMY4WORD(IPL_MODE)
} IPL_MODE;

//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
typedef enum
{
    IPL_SLEEP_LEVEL_0 = 0,      ///< sleep level 0.
    IPL_SLEEP_LEVEL_1,          ///< sleep level 1.
    IPL_SLEEP_LEVEL_2,          ///< sleep level 2.
    IPL_SLEEP_LEVEL_3,          ///< sleep level 3.
    ENUM_DUMMY4WORD(IPL_SLEEP_LEVEL)
} IPL_SLEEP_LEVEL;
//#NT#2009/10/08#Bowen Li -end
//#NT#2009/10/29#Bowen Li -end

typedef struct
{
    UINT32 uiCmd;                   ///< IPL command.
    IPL_MODE Mode;                  ///< IPL mode setting.
    UINT32 uiImeOutWidth1;          ///< ime path1 output width. for display so far
    UINT32 uiImeOutHeight1;         ///< ime path1 output height.
    UINT32 uiImeOutYoft1;
    UINT32 uiImeOutUVoft1;
    UINT32 uiImeOutWidth2;          ///< ime path2 output width. for recording file so far
    UINT32 uiImeOutHeight2;         ///< ime path2 output height.
    UINT32 uiImeOutYoft2;
    UINT32 uiImeOutUVoft2;
    UINT32 uiDzoomStep;             ///< digital zoom step.
    IME_OUTFMT uiImgFmt;            ///< fmt for path1 and path2.
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    IPL_SLEEP_LEVEL uiSleepLevel;   ///< the level of the sleep mode.
//#NT#2009/10/08#Bowen Li -end
} IPL_OBJ;


ER IPL_SetCmd(IPL_OBJ *Obj);
IPL_MODE IPL_GetMode(void);
IPL_MODE IPL_GetNextMode(void);
//#NT#20090929#Bowen Li -begin
//#NT#add new function for change clock
ER IPL_Stop2Dram(void);
//#NT#20090929#Bowen Li -end
#endif

//@}
