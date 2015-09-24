#ifndef _DATASTAMP_H
#define _DATASTAMP_H

#include "Type.h"


//#NT#2008/07/17#JJ Huang -begin
//#NT#DataStamp API Intergration080717
/**
    IPPBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
    //Total mode, an unreal mode for resources maintain
    DS_YCCFORMAT_444=0,
    DS_YCCFORMAT_422,
    DS_YCCFORMAT_420,
    DS_YCCFORMAT_MAXIDX
} DS_YCC_FORMAT;


/**
    DS_STAMP_INFOR
    Data-Stamp Information (Date-Image)
*/
typedef struct    _DS_STAMP_INFOR{
    UINT32  AddrY;
    UINT32  AddrCb;
    UINT32  AddrCr;
    UINT32  LineOffset;
    UINT16  Width;
    UINT16  Height;
    UINT8   ColorKeyY;
    UINT8   ColorKeyCb;
    UINT8   ColorKeyCr;
    DS_YCC_FORMAT   Format;
} DS_STAMP_INFOR, *pDS_STAMP_INFOR;

/**
    DS_BACKGROUND_INFOR
    Background Image Information (Original-Image)
*/
typedef struct    _DS_BACKGROUND_INFOR{
    UINT32  AddrY;
    UINT32  AddrCb;
    UINT32  AddrCr;
    UINT32  LineOffset;
    //#NT#2011/01/17#ethanlau -begin
    //#NT#image fmt
    DS_YCC_FORMAT  Format;
    //#NT#2011/01/17#ethanlau -end
} DS_BACKGROUND_INFOR, *pDS_BACKGROUND_INFOR;

/**
    DataStamp Error Message
*/
#define DATASTAMP_E_OK                0           /* Normal termination */
#define DATASTAMP_E_SIZEERROR         1           /* Size incorrect */
#define DATASTAMP_E_PARAERROR         2           /* parameter incorrect */

#define StampStartRatioShift          10
//API for System
extern UINT32 datastamp_Enable(BOOL bEn);
extern UINT32 datastamp_SetStampInfor(DS_STAMP_INFOR *pstmpinfor);
extern UINT32 datastamp_SetPosition(UINT32 uiXPos, UINT32 uiYPos);

//API for Sensor Lib
extern BOOL datastamp_IsEnable(void);
extern UINT32 datastamp_GetStampInfor(DS_STAMP_INFOR *pstmpinfor);
extern void datastamp_ImprintYCC(DS_BACKGROUND_INFOR *BackgroundInfo,DS_STAMP_INFOR *StampInfo, UINT32 BufferAddr);
extern UINT32 datastamp_GetPosition(UINT32* puiXPos, UINT32* puiYPos);
//#NT#2008/07/17#JJ Huang -end
extern void datastamp_Open(UINT32 uiClkRate);
extern void datastamp_Close(void);
extern void datastamp_ChkStampFmt(UINT32 _Fmt,DS_STAMP_INFOR *pStampInfo,UINT32 workingBuf,UINT32 _case);
extern UINT32 datastamp_GetStampAddrY(DS_STAMP_INFOR *pstmpinfor);//2011/07/28 Meg Lin
//#NT#2009/08/10#Hideo Lin -begin
//#NT#Date stamp for screen-nail test
extern UINT32 datastamp_SetStampInforThumb(DS_STAMP_INFOR *pstmpinfor);
extern UINT32 datastamp_GetStampInforThumb(DS_STAMP_INFOR *pstmpinfor);
extern UINT32 datastamp_SetPositionThumb(UINT32 uiXPos, UINT32 uiYPos);
extern UINT32 datastamp_GetPositionThumb(UINT32* puiXPos, UINT32* puiYPos);
extern BOOL datastampThumb_IsEnable(void);
extern UINT32 datastampThumb_Enable(BOOL bEn);
//#NT#2009/08/10#Hideo Lin -end

#endif// _DATASTAMP_H


