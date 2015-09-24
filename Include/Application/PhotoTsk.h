
#ifndef _PHOTOTSK_H
#define _PHOTOTSK_H

#include "Type.h"
#include "ImageAlgInfor.h"
#include "appcommon.h"
#include "Lens.h"
#include "ImageEffect.h"
#include "Alg_IPLMode.h"

//
//FLG_ID_PHOTO, "Bit 31 reserved"
//
#define FLGPHOTO_1STIDLE        FLGPTN_BIT(0)
#define FLGPHOTO_2NDIDLE        FLGPTN_BIT(1)
#define FLGPHOTO_3RDIDLE        FLGPTN_BIT(2)

#define FLGPHOTO_ZOOMIN         FLGPTN_BIT(3)      // zoom-in on preview for photo mode
#define FLGPHOTO_ZOOMOUT        FLGPTN_BIT(4)      // zoom-out on preview for photo mode
#define FLGPHOTO_AE             FLGPTN_BIT(5)      // AE on preview for photo mode
#define FLGPHOTO_AWB            FLGPTN_BIT(6)      // AWB on preview for photo mode
#define FLGPHOTO_FRAME_DONE     FLGPTN_BIT(7)      // sie frame done
#define FLGPHOTO_FD             FLGPTN_BIT(8)      //Face Detection
#define FLGPHOTO_AF_VA          FLGPTN_BIT(9)      //AF VA
#define FLGPHOTO_SD             FLGPTN_BIT(10)     //Smile Detection
#define FLGPHOTO_CA             FLGPTN_BIT(11)     //CA process
#define FLGPHOTO_IE             FLGPTN_BIT(12)     // Image effect process
#define FLGPHOTO_IE_DONE        FLGPTN_BIT(13)     // Image effect process done
//#define FLGPHOTO_NVTSMR         FLGPTN_BIT(14)     // Nvt smear reduction: FW calculation

#define FLGPHOTO_VIDOPENGRPH    FLGPTN_BIT(14)     // open grph for vid stamp
#define FLGPHOTO_VIDCLOSEGRPH   FLGPTN_BIT(15)     // close grph

#define FLGAE_IDLE              FLGPTN_BIT(16)
#define FLGAWB_IDLE             FLGPTN_BIT(17)
//#NT#2011/12/08#Ethan Lau -begin
//#NT#rst phototsk ime relative cnt
#define FLGPHOTO_RSTIMECNT      FLGPTN_BIT(18)
//#NT#2011/12/08#Ethan Lau -end

//#NT#2012/05/07#KS Hung -begin
//#NT#Support pseudo optical zoom
#define FLGPHOTO_FAKE_OZOOM     FLGPTN_BIT(19)
//#NT#2012/05/07#KS Hung -end
#define REV1                    FLGPTN_BIT(30)
#define REV2                    FLGPTN_BIT(31)
//
#define PHOTO_1ST   FLGPHOTO_1STIDLE
#define PHOTO_2ND   FLGPHOTO_2NDIDLE
#define PHOTO_3RD   FLGPHOTO_3RDIDLE
#define PHOTO_AE    FLGAE_IDLE
#define PHOTO_AWB   FLGAWB_IDLE

#define COLOR_Y         0
#define COLOR_CB        1
#define COLOR_CR        2
#define LOCK            1
#define UNLOCK          0


typedef void (*FPPHOTOCB)(unsigned int MessageID, AlgMsgInfo * pAlgMsg_infor);
typedef void (*FPPHOTOAE)(void);
typedef void (*FPPHOTOAWB)(void);
typedef void (*FPPHOTOAF)(void);
typedef void (*FPPHOTOFD)(void);
typedef void (*FPPHOTOSD)(void);
typedef void (*FPPHOTOCA)(void);
//-----------------------------------------------------------------------
// Global data release
//-----------------------------------------------------------------------
extern AlgMsgInfo    gAlgMsg;
//-----------------------------------------------------------------------
// function prototype description
//-----------------------------------------------------------------------
extern ER      Photo_Open(PPHOTO_APPOBJ pPhotoInfor);
extern ER      Photo_Close(void);
extern UINT    Photo_SetMemory(UINT S_Addr,UINT Size);
extern INT32   Photo_RegisterAE(FPPHOTOAE pPhotoAE);
extern INT32   Photo_RegisterAWB(FPPHOTOAWB pPhotoAWB);
extern INT32   Photo_RegisterFD(FPPHOTOFD pPhotoFD);
extern INT32   Photo_RegisterSD(FPPHOTOSD pPhotoSD);
extern INT32   Photo_RegisterCA(FPPHOTOCA pPhotoCA);
extern void    Photo_LockAE(UB lock);
extern BOOL    Photo_IsAELock(void);
extern void    Photo_LockAWB(UB lock);
extern BOOL    Photo_IsAWBLock(void);
extern void    Photo_LockFD(UB lock);
extern BOOL    Photo_IsFDLock(void);
extern void    Photo_LockSD(UB lock);
extern BOOL    Photo_IsSDLock(void);
extern void    Photo_SetAeRate(UB rate);
extern UB      Photo_GetAERate(void);
extern void    Photo_SetAwbRate(UB rate);
extern UB      Photo_GetAWBRate(void);
extern void    Photo_SetFdRate(UB rate);
extern UB      Photo_GetFdRate(void);
extern void    Photo_SetSdRate(UB rate);
extern UB      Photo_GetSdRate(void);
extern void    Photo_GetHist(UW *Histogram);
extern BOOL    Photo_WaitPhotoIdle(BOOL bWaitInfinite, FLGPTN TaskSel);

extern INT32   Photo_DZoom_In(void);
extern INT32   Photo_DZoom_Out(void);
extern INT32   Photo_DZoom_StepIn(void);
extern INT32   Photo_DZoom_StepOut(void);
extern INT32   Photo_DZoom_Stop(void);
extern INT32   Photo_DZoom_GotoStep(unsigned int dzoom_step);
extern UINT    Photo_GetDZoomIdx(void);
extern UINT    Photo_GetMaxDzoomIdx(void);

extern BOOL    Photo_WaitIEDone(BOOL bWaitInfinite);
//#NT#2011/02/11#ethanlau -begin
//#NT# phototsk1 app
extern ER Photo_ConfigStamp(UINT32 ImgH,UINT ImgV,UINT32 StampBuf);
//#NT#2011/02/11#ethanlau -end
extern void Photo_VidstampEnable(BOOL enable);
//#NT#2012/06/05#KS Hung -begin
//#NT#Using digital zoom fills the pseudo optical zoom.
extern void Photo_SetFakeZoom(UINT32 uiFakeZoom);
extern void Photo_DZoom_GotoStep2(UINT32 dzoom_step);
//#NT#2012/06/05#KS Hung -end
//#NT#2012/07/06#KS Hung -begin
//#NT#Set the Dzoom index of max need
extern void Photo_SetMaxNeedZoomIndex(UINT32 uiMaxNeedZoomIndex);
//#NT#2012/07/06#KS Hung -end


typedef enum
{
    PHOTO_VIEWTYPE_NORMAL  = 0,
    PHOTO_VIEWTYPE_SINGLE  = 1,
    PHOTO_VIEWTYPE_MULTI   = 2

} PHOTO_VIEWTYPE;  //Photo view type

//-----------------------------------------------------------------------
// Photo-Multi-View APIs with Initial Structure  -Begin
//-----------------------------------------------------------------------

//#NT#2009/11/02#Niven Cho -begin
//#NT#Added., Indicate Open/Close without animation for 2 effect change directly (as awb<->ev)
//#NT#2009/10/08#YC Peng -begin
//#NT#Add ARF sub-effect for EVARF/AWBARF mode
typedef struct _PHOTO_MULTI_VIEW_INIT{
    IE_MODE         Mode; //!< Setting of IE_MODE_XXXXX
    IE_INDEX*       pTbl; //!< User Define Effects Table
    UINT32          nItem;//!< Total Items of pTbl
    UINT32          iItem;//!< Current Item of pTbl
    void            (*FP_MultiViewOpenCB)(void);//!< Callback function for MultiView open
    void            (*FP_MultiViewCloseCB)(void);//!< Callback function for MultiView close
    IE_INDEX        ARFSubIndex; //!< User Define ARF Sub-effect index
    BOOL            bSkipOpenAnimation;  //!< Skip Open Scale Down Animation
}tPHOTO_MULTI_VIEW_INIT;
//#NT#2009/10/08#YC Peng -end
typedef struct _PHOTO_MULTI_VIEW_CLOSE{
    BOOL            bApply;
    BOOL            bSkipCloseAnimation;
}tPHOTO_MULTI_VIEW_CLOSE;
extern BOOL     Photo_MultiView_Open(PHOTO_VIEWTYPE FromViewType, tPHOTO_MULTI_VIEW_INIT* pInit);
extern BOOL     Photo_MultiView_Close(PHOTO_VIEWTYPE ToViewType,tPHOTO_MULTI_VIEW_CLOSE* pClose);
//#NT#2009/11/02#Niven Cho -end
extern BOOL     Photo_MultiView_MoveLeft(void);
extern BOOL     Photo_MultiView_MoveRight(void);
extern UINT32   Photo_MultiView_GetSelectedIndex(void);
//-----------------------------------------------------------------------
// Photo-Multi-View APIs with Initial Structure  -End
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Photo-Single-View APIs with Initial Structure  -Begin
//-----------------------------------------------------------------------
typedef struct _PHOTO_SINGLE_VIEW_INIT{
    IE_MODE         Mode; //!< Setting of IE_MODE_XXXXX
    IE_INDEX        Index; //!< User Define Effects Table
}tPHOTO_SINGLE_VIEW_INIT;

//#NT#2009/11/06#YC Peng -begin
//#NT#Add image ratio function for image effect
extern BOOL Photo_SingleView_Open(PHOTO_VIEWTYPE FromViewType, tPHOTO_SINGLE_VIEW_INIT* pInit);
extern BOOL Photo_SingleView_Close(PHOTO_VIEWTYPE ToViewType);
extern INT32 Photo_SingleView_SetImageRatio(_IMGRatio_sel RatioIdx);
extern void PhotoDisplay_SingleView_DisplayFBLock(UINT32 LockEn);
//#NT#2009/11/06#YC Peng -end

//-----------------------------------------------------------------------
// Photo-Multi-View APIs with Initial Structure  -End
//-----------------------------------------------------------------------

#endif // _PHOTOTSK_H