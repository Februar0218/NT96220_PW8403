//inlcude
#include "Dcout.h"
#include "Ime_lib.h"
#include "imagealginfor.h"
#include "ippbuffer.h"
#include "debug.h"
#include "sen_common.h"
#include "utility.h"
#include "Dzoom.h"
//define
#define Dcout_DEBUG DISABLE
#if Dcout_DEBUG
#define Dcout_MSG(...) debug_msg("^G Dc:"__VA_ARGS__)
#else
#define Dcout_MSG(...)
#endif

typedef enum _DELAYTRIGGER
{
    UNKNOW = 0,
    Delay0 = 1,
    Delay1 = 2,
    Delay2 = 3,
    Delay3 = 4,
    Max_Delay = 5,
    ENUM_DUMMY4WORD(DELAYTRIGGER)
}DELAYTRIGGER;
typedef struct _DCOUT_IMEINFO
{
    UINT32 uiImeSliceNum;
    UINT32 uiImeSliceH;
    UINT32 uiImePingPongNum;
    UINT32 uiImeAddrY[IME_OUTPUT_PPB_5+1];
    UINT32 uiImeAddrCb[IME_OUTPUT_PPB_5+1];
    UINT32 uiImeAddrCr[IME_OUTPUT_PPB_5+1];
}DCOUT_IMEINFO;
typedef void (*fpFunction)(PDCOUT_OBJ pObj);

#define Dcout_OP_PAUSE   0xffff
//static api
static void Dcout_event(UINT32 uiEventStatus);
static void Dcout_FTrigger(BOOL bLastTrig);
static void Dcout_STrigger(BOOL bLastTrig);
static ER StateMachChk(UINT32 Op);
static void Dcout_SetDcoutsize(PDCOUT_OBJ pObj);
static void Dcout_FOpen(PDCOUT_OBJ pObj);
static void Dcout_SOpen(PDCOUT_OBJ pObj);
static void Dcout_TriggerIdx(PDCOUT_OBJ pObj);
static void Dcout_close(PDCOUT_OBJ pObj);
//#NT#2011/12/09#Ethan Lau -begin
//#NT#refine dcout api
static __inline BOOL Dcout_GetCurOpmode(void);
static __inline void Dcout_SetCurOpmode(UINT32 mode);
//#NT#2011/12/09#Ethan Lau -end
//static var
static fpFunction m_DcoutOpTbl[DCOUT_OP_Max] = {
    NULL,
    Dcout_SetDcoutsize,
    Dcout_TriggerIdx,
    Dcout_FOpen,
    Dcout_SOpen,
    Dcout_close,
};
static DcoutSize m_DcoutSize = {0};
static DCOUT_IMEINFO m_Dcout_ImeInfo = {0};
static JPGLIB_DC_OUT_CFG m_jpgDcout = {0};
static UINT32 m_JpgQ = 85;
static UINT32 m_uiJpgFmt = JPGFormatYUV422;
static UINT32 m_uiJpgTrigIdx = 0;//cfg jpg start at which ime frame end
static volatile DCOUT_OP m_CurOpmode = DCOUT_OP_CLOSE;
static volatile UINT32 m_uiJpg_fc = 0;
static volatile UINT32 m_uiJpgSliceCounter = 0;
static volatile UINT32 m_uiIme_fc = 0;
static volatile UINT32 m_uiImeSliceCounter = 0;
const DELAYTRIGGER m_JPG_DELAYTRIGGER_SLICE = Delay1;//jpg start trigger at which ime slice done,
                                                     //if stamp needed,must >= delay1
const DELAYTRIGGER m_JPG_DELAYTRIGGER_FRAME = Delay1;//jpg start trigger at which ime slice done,
                                                     //if stamp needed,must >= delay1
//flag for control flow
static volatile BOOL m_bSync = FALSE;
static volatile BOOL m_bJpgFirstTrig = FALSE;
static volatile BOOL m_bJpgStop = FALSE;
static volatile BOOL m_bIMEdone = FALSE,m_bJPGdone = FALSE;
//#NT#2011/12/02#Ethan Lau -begin
//#NT#patch jpg video condition constraint
static volatile UINT32 m_uiJpgStatus=0;
//#NT#2011/12/02#Ethan Lau -end
//intern api
static void Dcout_event(UINT32 uiEventStatus)
{
    if(uiEventStatus & IME_INT_SLICE_DN1)
    {
        if(m_bSync)//sync timing after hook api
        {
            if( Dcout_GetCurOpmode() == DCOUT_OP_SOPEN)
            {
                if(m_bJpgFirstTrig == FALSE)
                {
                    //decide jpg start trigger or not, trigger at m_JPG_DELAYTRIGGER_SLICE slice done
                    //after m_uiJpgTrigIdx ime frame end
                    if( m_uiImeSliceCounter >= (m_uiJpgTrigIdx*m_Dcout_ImeInfo.uiImeSliceNum+ (m_JPG_DELAYTRIGGER_SLICE-1)) )
                    {
                        Dcout_STrigger(FALSE);//fist jpg slice start
                        m_bJpgFirstTrig = TRUE;
                        Dcout_MSG("Str\r\n");
                    }
                }
            }
            m_uiImeSliceCounter++;
        }
    }

    if(uiEventStatus & IME_INT_FRM)
    {
        if( Dcout_GetCurOpmode() == DCOUT_OP_FOPEN)
        {
            //decide jpg start trigger or not, trigger at m_JPG_DELAYTRIGGER_FRAME frame end
            //after m_uiJpgTrigIdx ime frame end
            if(m_uiIme_fc >= (m_uiJpgTrigIdx + (m_JPG_DELAYTRIGGER_FRAME-1)) )
            {
                if(Sen_GetIPLStopStatus()==_SIE_PAUSE)//this status is changed at ime frameend(ime last frame end)
                {
                    //#NT#2011/12/09#Ethan Lau -begin
                    //#NT#jpg will be triggered until last ime frame-end.
                    //Dcout_SetCurOpmode(Dcout_OP_PAUSE);
                    Dcout_FTrigger(TRUE);
                    //#NT#2011/12/09#Ethan Lau -end
                    Dcout_MSG("dcout status chg to pause before jpg start trigger\r\n");
                }
                else
                {
                    //#NT#2011/12/09#Ethan Lau -begin
                    //#NT# jpg trig (n)st fame when ime have (n+1)st frame-end
                    //#NT# let post-process have chance to op. within 1 frame-end
                    Dcout_FTrigger(FALSE);
                    //#NT#2011/12/09#Ethan Lau -end
                    //mark,move to ime_isrCB FRM status
                    //Sen_IsrEventProc(SEN_ISR_PHOTODISP_IDE, uiEventStatus);//ctrl ide idx
                }
            }
            else
            {
                if(Sen_GetIPLStopStatus()==_IPEIME_PAUSE)//jpg don't start yet, but pipeline is chging to stop
                {
                    Dcout_SetCurOpmode(Dcout_OP_PAUSE);
                    Dcout_MSG("dcout status chg to pause before jpg start trigger\r\n");
                }
                else
                {
                    //donothing
                }
            }
        }
        else if( Dcout_GetCurOpmode() == DCOUT_OP_SOPEN)
        {
            m_bSync = TRUE;
            if(m_bJpgFirstTrig)
            {
                //jpeg will will not encode last frame when piple line prepare to stop
                if(Sen_GetIPLStopStatus()==_IPEIME_PAUSE)//ime 2nd frame end from the end
                {
                    m_bJpgStop = TRUE;
                }
            }
            else
            {
                if(Sen_GetIPLStopStatus()==_IPEIME_PAUSE)//pipeline pause before jpg start
                {
                    Dcout_SetCurOpmode(Dcout_OP_PAUSE);
                }
            }
        }
        else
        {
        }
        m_uiIme_fc++;
    }

    //other event
    /**/
}

void JpgDcout_isrCB(UINT32 IntStatus)
{
    JPGLIB_DC_OUT_ADDR jpg_dcoutaddr;
    //#NT#2011/12/02#Ethan Lau -begin
    //#NT#use or to instead of equalize,in case of avoiding get no status,like frame end.
    //#NT#side effect,can't get indicated timing,like (n)st frameend,(n+1)st frameend
    m_uiJpgStatus |= IntStatus;
    //#NT#2011/12/02#Ethan Lau -end
    if(IntStatus & _JPEG_Slice_Done)
    {
        //if( ((m_uiJpgSliceCounter+1) % m_Dcout_ImeInfo.uiImeSliceNum) == m_Dcout_ImeInfo.uiImeSliceNum)//last jpg slice
        if((m_uiJpgSliceCounter % m_Dcout_ImeInfo.uiImeSliceNum) == (m_Dcout_ImeInfo.uiImeSliceNum - 1))//last jpg slice
        {
            //do nothing,jpg trigger next frame by current jpg frame end
            //Dcout_TrigCtrl(DCOUT_ISR_JPG);
            Dcout_MSG("l");
        }
        else
        {
            //following have 2 condition:
            //1.if isr timing is ime frameend is faster than the jpg 2nd slice done from the end,
            //  jpg will not have last image.
            //2.if isr timming is jpg 2nd slice done from the end is faster than ime frameend,
            //  jpg will have last image.
            if(m_bJpgStop)//jpg prepare to stop(slice of last jpg frame)
            {
                //if((m_uiJpgSliceCounter % m_Dcout_ImeInfo.uiImeSliceNum) == m_Dcout_ImeInfo.uiImeSliceNum-3)
                //{
                //    Dcout_TrigCtrl(DCOUT_ISR_JPG);
                //}

                if((m_uiJpgSliceCounter % m_Dcout_ImeInfo.uiImeSliceNum) == m_Dcout_ImeInfo.uiImeSliceNum - 2)
                {
                    Dcout_STrigger(TRUE);//last jpg trig,and chg m_CurOpmode to pause
                }
                else//remained jpg slice after ime frame end
                {
                    Dcout_TrigCtrl(DCOUT_ISR_JPG);
                }
            }
            else
            {
                Dcout_TrigCtrl(DCOUT_ISR_JPG);
            }
        }
        m_uiJpgSliceCounter++;
        Dcout_MSG("fd ");
    }

    if(IntStatus & _JPEG_Frame_End)
    {
        if( Dcout_GetCurOpmode() == DCOUT_OP_SOPEN)
        {
            //dcout only support 3 frame ping-pong
            switch(m_uiJpg_fc %3)//dc out next destination
            {
                case 0:
                    jpg_dcoutaddr.DCYAddr =ippbuf_GetBuf(IPPBUF_PRV_IME_P1_2);
                    break;
                case 1:
                    jpg_dcoutaddr.DCYAddr =ippbuf_GetBuf(IPPBUF_PRV_IME_P1_3);
                    break;
                case 2:
                    jpg_dcoutaddr.DCYAddr =ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
                    break;
                default:
                    break;
            }

            jpg_dcoutaddr.DCUAddr =jpg_dcoutaddr.DCYAddr + GetIPPSizeInfor(_JpgDCoutYlineoffset)*GetIPPSizeInfor(_JpgDCoutVsize);
            switch(m_uiJpgFmt)
            {
                case JPGFormatYUV420:
                    jpg_dcoutaddr.DCVAddr = jpg_dcoutaddr.DCUAddr + GetIPPSizeInfor(_JpgDCoutUVlineoffset)*GetIPPSizeInfor(_JpgDCoutVsize)/2;
                    break;
                default:
                    debug_err(("%s %d\r\n",__LINE__,__func__));
                case JPGFormatYUV422:
                    jpg_dcoutaddr.DCVAddr = jpg_dcoutaddr.DCUAddr + GetIPPSizeInfor(_JpgDCoutUVlineoffset)*GetIPPSizeInfor(_JpgDCoutVsize);
                    break;
            }
            //when m_bJpgStop == TRUE, still need to chg out addr, or photodisplaytsk will not be get last dcout image.
            JpegSetDCOutAddr(&jpg_dcoutaddr);
            if(!m_bJpgStop)
            {
                Dcout_TrigCtrl(DCOUT_ISR_JPG);//trigger next frame jpg encode
            }
        }
        else if( Dcout_GetCurOpmode() == DCOUT_OP_FOPEN)
        {}
        else
        {}

        if(GetIPPFuncInfor(JPG_DCOUT_EN) == ENABLE)
        {
            Sen_IsrEventProc(SEN_ISR_PHOTODISP_IDE, IntStatus);
        }
        m_uiJpg_fc++;
        m_uiJpgSliceCounter = 0;//rst ,incase of last slice done have no interrupt occurred
        Dcout_MSG("fe \r\n");
    }
}

//IF bLastTrig == TRUE,trigger jpg last slice.
static void Dcout_STrigger(BOOL bLastTrig)
{
//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
    UINT32 ImeBuffIndx=0;
    JPGLIB_ENC_TRIG jpg_slice = {0};
    UINT32 i;
    jpg_slice.SliceHeight = m_Dcout_ImeInfo.uiImeSliceH;

    if(bLastTrig)//last jpg slice trigger
    {
        Dcout_SetCurOpmode(Dcout_OP_PAUSE);
    }

    //jpg must be triggered when ime slice done,so just need to get ready id
    ImeBuffIndx = ime_getPath1CurrentBufferIndex();

    for(i=0;i<(m_JPG_DELAYTRIGGER_SLICE-1);i++)
    {
        if(ImeBuffIndx >0 )
            ImeBuffIndx--;
        else
            ImeBuffIndx = (m_Dcout_ImeInfo.uiImePingPongNum - 1);
    }
    jpg_slice.YAddr = m_Dcout_ImeInfo.uiImeAddrY[ImeBuffIndx];
    jpg_slice.UAddr = m_Dcout_ImeInfo.uiImeAddrCb[ImeBuffIndx];
    jpg_slice.VAddr = m_Dcout_ImeInfo.uiImeAddrCr[ImeBuffIndx];
    JpegEncVidTrigger(&jpg_slice);
//#NT#2011/06/17#Ben Wang -end
}

//IF bLastTrig == TRUE,trigger jpg last frame.
static void Dcout_FTrigger(BOOL bLastTrig)
{
    UINT32 ImeBuffIndx,i;
//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
    JPGLIB_ENC_TRIG jpg_frame = {0};
    //#NT#2011/12/09#Ethan Lau -begin
    if(bLastTrig)//last jpg trigger
    {
        Dcout_SetCurOpmode(Dcout_OP_PAUSE);
    }
    //#NT#2011/12/09#Ethan Lau -end
    ImeBuffIndx = ime_getPath1CurrentBufferIndex();

    for(i=0;i<(m_JPG_DELAYTRIGGER_FRAME-1);i++)
    {
        if(ImeBuffIndx >0 )
            ImeBuffIndx--;
        else
            ImeBuffIndx = (m_Dcout_ImeInfo.uiImePingPongNum - 1);
    }

    jpg_frame.YAddr = m_Dcout_ImeInfo.uiImeAddrY[ImeBuffIndx];
    jpg_frame.UAddr = m_Dcout_ImeInfo.uiImeAddrCb[ImeBuffIndx];
    jpg_frame.VAddr = m_Dcout_ImeInfo.uiImeAddrCr[ImeBuffIndx];
    JpegEncVidTrigger(&jpg_frame);
//#NT#2011/06/17#Ben Wang -end
}

static void Dcout_SetDcoutsize(PDCOUT_OBJ pObj)
{
    JPGLIB_DC_OUT_CFG Jpg_dcout = {0};
    UINT32 HRatio, VRatio;

    Jpg_dcout.FrameWidth = pObj->uiInSizeH;
    Jpg_dcout.FrameHeight = pObj->uiInSizeV;
//    Jpg_dcout.PanelWidth = pObj->uiPanelSizeH;
//    Jpg_dcout.PanelHeight = pObj->uiPanelSizeV;
    //if (GetIPPSizeInfor(_SensorRatioIdx) != gImageAlgInfo.ImageRatio)
    if (gImageAlgInfo.DispRatio != gImageAlgInfo.ImageRatio)
    {
        HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
        VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
    
        if(gImageAlgInfo.DsiplayFitWindow == _WindowFitAuto)
        {
            Jpg_dcout.PanelWidth = Adj_HSize(pObj->uiPanelSizeH, HRatio, 16);
            Jpg_dcout.PanelHeight = Adj_VSize(pObj->uiPanelSizeV, VRatio, 4);
        }
        else if(gImageAlgInfo.DsiplayFitWindow == _WindowFitHori)
        {
            Jpg_dcout.PanelWidth = Adj_HSize(pObj->uiPanelSizeH, IMG_RATIO_UINT, 16);
            Jpg_dcout.PanelHeight = Adj_VSize(pObj->uiPanelSizeV, VRatio*IMG_RATIO_UINT/HRatio, 4);
        }
        else if(gImageAlgInfo.DsiplayFitWindow == _WindowFitVert)
        {
            Jpg_dcout.PanelWidth = Adj_HSize(pObj->uiPanelSizeH, HRatio*IMG_RATIO_UINT/VRatio, 16);
            Jpg_dcout.PanelHeight = Adj_VSize(pObj->uiPanelSizeV, IMG_RATIO_UINT, 4);
        }
        //debug_err(("Jpg_dcout.Panel(%d,%d)\r\n", Jpg_dcout.PanelWidth,Jpg_dcout.PanelHeight));
    }
    else
    {
        Jpg_dcout.PanelWidth = pObj->uiPanelSizeH;
        Jpg_dcout.PanelHeight = pObj->uiPanelSizeV;
    }
    Jpg_dcout.YUVFormat = pObj->Fmt;

    JpegGetDCOutConfig(&Jpg_dcout);
    pObj->pDcoutSize->uiSizeH = Jpg_dcout.DCWidth;
    pObj->pDcoutSize->uiSizeV = Jpg_dcout.DCHeight;
    pObj->pDcoutSize->uiLineoffsetY = Jpg_dcout.DCYLineOffset;
    pObj->pDcoutSize->uiLineoffsetUV = Jpg_dcout.DCULineOffset;

    //debug_err(("JpegGetDCOutConfig: (%d,%d), lineofst(%d,%d)\r\n", Jpg_dcout.DCWidth,Jpg_dcout.DCHeight,Jpg_dcout.DCYLineOffset,Jpg_dcout.DCULineOffset));
    if(Jpg_dcout.DCULineOffset != Jpg_dcout.DCVLineOffset)
        debug_err(("^R %d %s (%d %d)dcout lineoffset U!=V\r\n",__LINE__,__func__,Jpg_dcout.DCULineOffset,Jpg_dcout.DCVLineOffset));

    SetIPPSizeInfor(_JpgDCoutHsize, Jpg_dcout.DCWidth);
    SetIPPSizeInfor(_JpgDCoutVsize, Jpg_dcout.DCHeight);
    SetIPPSizeInfor(_JpgDCoutYlineoffset, Jpg_dcout.DCYLineOffset);
    SetIPPSizeInfor(_JpgDCoutUVlineoffset, Jpg_dcout.DCULineOffset);

    m_DcoutSize.uiSizeH = Jpg_dcout.DCWidth;
    m_DcoutSize.uiSizeV = Jpg_dcout.DCHeight;
    m_DcoutSize.uiLineoffsetY = Jpg_dcout.DCYLineOffset;
    m_DcoutSize.uiLineoffsetUV = Jpg_dcout.DCULineOffset;

    m_jpgDcout = Jpg_dcout;
}

static void Dcout_FOpen(PDCOUT_OBJ pObj)
{
    JPGLIB_CFG jpgFullFramecfg = {0};
    //JPGLIB_DC_OUT_CFG Jpg_dcout = {0};
    UINT32 i=0;
//#NT#2011/06/16#Ben Wang -begin
//#NT#refine UserJpeg API
    if( (m_JPG_DELAYTRIGGER_FRAME == UNKNOW)||(m_JPG_DELAYTRIGGER_FRAME >= Max_Delay))
    {
        debug_err(("%s Jpg Delay trigger const(%d) error\r\n",__func__,m_JPG_DELAYTRIGGER_FRAME));
    }
    //Jpg_dcout = m_jpgDcout;
    if(GetIPPFuncInfor(JPG_DCOUT_EN))
    {
        debug_err(("^R %s:video framemode do not support encode dcout\r\n",__func__));
        SetIPPFuncInfor(JPG_DCOUT_EN, DISABLE);
    }
    JpegOpen();
    //pass cb addr
    pObj->fpImeIsrCB = Dcout_event;

    m_uiJpgFmt = pObj->Fmt;
    //get info from ime engine
    m_Dcout_ImeInfo.uiImePingPongNum = ime_getEngineInfo(IME_GET_PPB_NUM);
    for(i=0; i<m_Dcout_ImeInfo.uiImePingPongNum ;i++)
    {
        m_Dcout_ImeInfo.uiImeAddrY[i] = ime_getOutputPath1Add(i,IME_CHANNEL_Y);
        m_Dcout_ImeInfo.uiImeAddrCb[i] = ime_getOutputPath1Add(i,IME_CHANNEL_CB);
        m_Dcout_ImeInfo.uiImeAddrCr[i] = ime_getOutputPath1Add(i,IME_CHANNEL_CR);
    }

    {

        jpgFullFramecfg.BSBufAddr = ippbuf_GetBuf(IPPBUF_JPGADDR);//jpgcmpparam.JPGBSAddr;
        jpgFullFramecfg.BSBufSize = (JpegEncGetData(JPG_MAX_FRAME_SIZE)* JPG_BUF_NUM);
        jpgFullFramecfg.FrameWidth = pObj->uiInSizeH;//jpgcmpparam.Width;
        jpgFullFramecfg.FrameHeight = pObj->uiInSizeV;//jpgcmpparam.Height;
        jpgFullFramecfg.YUVFormat = pObj->Fmt;
        jpgFullFramecfg.SliceMode = 0;
        jpgFullFramecfg.FP_JPGLIB_CB = JpgDcout_isrCB;
        //#NT#2011/07/22#Ben Wang -begin
        //#NT#Add BRC flow
        jpgFullFramecfg.BrcMode = JPG_Enc_RhoBRCtl;
        //#NT#2011/07/22#Ben Wang -end
        JpegEncVidConfig(&jpgFullFramecfg);
//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
        //JpegEncBSBufInit(&jpgFullFramecfg);
//#NT#2011/06/17#Ben Wang -end
    }
//#NT#2011/06/16#Ben Wang -end
}

static void Dcout_SOpen(PDCOUT_OBJ pObj)
{
    JPGLIB_CFG jpgslicecfg = {0};
    JPGLIB_DC_OUT_CFG Jpg_dcout = {0};
    UINT32 i=0;
//#NT#2011/06/16#Ben Wang -begin
//#NT#refine UserJpeg API
//#NT#2011/07/06#Ben Wang -begin
//#NT#remove useless code
    JpegOpen();

    if( (m_JPG_DELAYTRIGGER_SLICE == UNKNOW)||(m_JPG_DELAYTRIGGER_SLICE >= Max_Delay))
    {
        debug_err(("%s Jpg Delay trigger const(%d) error\r\n",__func__,m_JPG_DELAYTRIGGER_SLICE));
    }
    Jpg_dcout = m_jpgDcout;
    Jpg_dcout.DCEnable = GetIPPFuncInfor(JPG_DCOUT_EN);;

    jpgslicecfg.BSBufAddr =  ippbuf_GetBuf(IPPBUF_JPGADDR);//pObj->uiBufaddr;
    jpgslicecfg.BSBufSize = (JpegEncGetData(JPG_MAX_FRAME_SIZE)* JPG_BUF_NUM);
    jpgslicecfg.FrameWidth =  pObj->uiInSizeH;//GetIPPSizeInfor(_ImeOut2Vsize);
    jpgslicecfg.FrameHeight =  pObj->uiInSizeV;//GetIPPSizeInfor(_ImeOut2Hsize);
//#NT#2011/07/08#Ben Wang -end
    jpgslicecfg.YUVFormat = pObj->Fmt;

    //pass cb addr
    pObj->fpImeIsrCB = Dcout_event;

    m_uiJpgFmt = jpgslicecfg.YUVFormat;
    //get info from ime engine
    m_Dcout_ImeInfo.uiImeSliceNum = ime_getEngineInfo(IME_GET_OPERATED_SLICE_NUM);
    m_Dcout_ImeInfo.uiImeSliceH = ime_getEngineInfo(IME_GET_SLICE_HEIGHT);
    m_Dcout_ImeInfo.uiImePingPongNum = ime_getEngineInfo(IME_GET_PPB_NUM);
    for(i=0; i<m_Dcout_ImeInfo.uiImePingPongNum ;i++)
    {
        m_Dcout_ImeInfo.uiImeAddrY[i] = ime_getOutputPath1Add(i,IME_CHANNEL_Y);
        m_Dcout_ImeInfo.uiImeAddrCb[i] = ime_getOutputPath1Add(i,IME_CHANNEL_CB);
        m_Dcout_ImeInfo.uiImeAddrCr[i] = ime_getOutputPath1Add(i,IME_CHANNEL_CR);
    }

    jpgslicecfg.FP_JPGLIB_CB = JpgDcout_isrCB;
    jpgslicecfg.SliceMode = 1;
    //#NT#2011/07/22#Ben Wang -begin
    //#NT#Add BRC flow
    jpgslicecfg.BrcMode = JPG_Enc_RhoBRCtl;
    //#NT#2011/07/22#Ben Wang -end
//#NT#2011/06/17#Ben Wang -begin
//#NT#refine UserJpeg API
    //JpegEncBSBufInit(&jpgslicecfg);

    JpegEncVidConfig(&jpgslicecfg);
    //JpegEncUpdateHDR(&jpgcmpparam,85);
//#NT#2011/06/17#Ben Wang -end
//#NT#2011/06/1#Ben Wang -end

    //#NT#set jpeg encode dcout addr
    Jpg_dcout.DCYAddr = ippbuf_GetBuf(IPPBUF_PRV_IME_P1);
    Jpg_dcout.DCUAddr = Jpg_dcout.DCYAddr +
        m_DcoutSize.uiLineoffsetY * m_DcoutSize.uiSizeV;

    switch(jpgslicecfg.YUVFormat)
    {
        case JPGFormatYUV420:
            Jpg_dcout.DCVAddr = Jpg_dcout.DCUAddr +
                m_DcoutSize.uiLineoffsetUV * m_DcoutSize.uiSizeV/2;
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
        case JPGFormatYUV422:
            Jpg_dcout.DCVAddr = Jpg_dcout.DCUAddr +
                m_DcoutSize.uiLineoffsetUV * m_DcoutSize.uiSizeV;
            break;
    }
    JpegEncDCOutConfig(&Jpg_dcout);

    m_jpgDcout = Jpg_dcout;
}

static void Dcout_TriggerIdx(PDCOUT_OBJ pObj)
{
    m_uiJpgTrigIdx = pObj->TriggerIdx;
}

static void Dcout_close(PDCOUT_OBJ pObj)
{
    UINT32 uiJpgStatus=0;

    while(1)
    {
        //#NT#2011/12/05#Ethan Lau -begin
        //#NT#for image pipeline lib to get jpg status under streaming condition
        //uiJpgStatus = JpegWaitDone();
        TimerDelayMs(2);
        uiJpgStatus = GetJpgStatus();
        //#NT#2011/12/05#Ethan Lau -end
        if (uiJpgStatus & _JPEG_Frame_End)
        {
            JpegClose();
            if(g_fpAlgExport)
            {
                g_fpAlgExport(ALGMSG_VIDEO_RECSTOP, &gAlgMsg);
            }
            break;
        }
    }
    //#NT#2011/06/17#Jarkko Chang -begin
    SetIPPFuncInfor(JPG_DCOUT_EN, DISABLE);
    //#NT#2011/06/17#Jarkko Chang -end
    //reset jpg global counter/flag
    m_uiJpg_fc = 0;
    m_uiJpgSliceCounter = 0;
    m_uiIme_fc = 0;
    m_uiImeSliceCounter = 0;
    m_uiJpgSliceCounter = 0;
    m_bJpgFirstTrig = FALSE;
    m_bSync = FALSE;
    m_bJpgStop = FALSE;
    m_bIMEdone = FALSE;
    m_bJPGdone = FALSE;
    m_uiJpgStatus = 0;
}

static __inline BOOL Dcout_GetCurOpmode(void)
{
    return m_CurOpmode;
}

static __inline void Dcout_SetCurOpmode(UINT32 mode)
{
    m_CurOpmode = mode;
}

static ER StateMachChk(UINT32 Op)
{
    ER er_code;
    switch(Op)
    {
        case DCOUT_OP_SIZEINFO:
            switch(Dcout_GetCurOpmode())
            {
                case DCOUT_OP_FOPEN:
                case DCOUT_OP_SOPEN:
                case Dcout_OP_PAUSE:
                    er_code= E_SYS;
                    break;
                default:
                    er_code= E_OK;
                    break;
            }
            break;
        case DCOUT_OP_TRIGGERIDX:
            switch(Dcout_GetCurOpmode())
            {
                case DCOUT_OP_FOPEN:
                case DCOUT_OP_SOPEN:
                    er_code = E_SYS;
                    break;
                default:
                    er_code= E_OK;
                    break;
            }
            break;
        case DCOUT_OP_FOPEN:
        case DCOUT_OP_SOPEN:
            switch(Dcout_GetCurOpmode())
            {
                case DCOUT_OP_FOPEN:
                case DCOUT_OP_SOPEN:
                case Dcout_OP_PAUSE:
                    er_code= E_SYS;
                    break;
                default:
                    er_code= E_OK;
                    break;
            }
            break;
        case DCOUT_OP_CLOSE:
            switch(Dcout_GetCurOpmode())
            {
                case Dcout_OP_PAUSE:
                    er_code= E_OK;
                    break;
                default:
                    er_code= E_SYS;
                    break;
            }
            break;
        default:
            debug_err(("%d %s\r\n",__LINE__,__func__));
            er_code= E_SYS;
            break;
    }
    return er_code;
}

//extern api
DCOUT_OP Dcout_GetCurOp(void)
{
    return Dcout_GetCurOpmode();
}

void Dcout_SetJpgQ(UINT32 JpgQ)
{
    m_JpgQ = JpgQ;
}

void Dcout_TrigCtrl(DCOUT_ISR ISR)
{
    if(Dcout_GetCurOpmode() == DCOUT_OP_SOPEN)
    {
        switch(ISR)
        {
            case DCOUT_ISR_IME:
                if(m_bIMEdone)
                {
                    debug_err(("s-trig:id->jd "));
                }
                m_bIMEdone = TRUE;
                break;
            case DCOUT_ISR_JPG:
                if(m_bJPGdone)
                {
                    debug_err(("s-trig:jd->id "));
                }
                m_bJPGdone = TRUE;
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
                break;
        }
        if((m_bIMEdone==TRUE) && (m_bJPGdone==TRUE))
        {
            Dcout_STrigger(FALSE);
            ime_triggerSlice();

            m_bIMEdone = FALSE;
            m_bJPGdone = FALSE;
            //debug_msg("T ");
        }
        else if(!m_bJpgFirstTrig)
        {
            if(m_bIMEdone==TRUE)
            {
                ime_triggerSlice();
                m_bIMEdone = FALSE;
            }
        }
#if Dcout_DEBUG
        if((m_bIMEdone==TRUE) && (m_bJPGdone==FALSE))
        {
            Dcout_MSG(" i->j ");
        }
        else if((m_bIMEdone==FALSE) && (m_bJPGdone==TRUE))
        {
            Dcout_MSG(" j->i ");
        }
#endif
    }
    else if(Dcout_GetCurOpmode() == DCOUT_OP_FOPEN)
    {
    }
    else//case for jpg does not hook on ime isr yet
    {
        switch(ISR)
        {
            case DCOUT_ISR_IME:
                Dcout_MSG(" IME T ");
                ime_triggerSlice();
                break;
            default:
                debug_err(("%d %s\r\n",__LINE__,__func__));
                break;
        }
    }
}

ER Dcout_Setcmd(PDCOUT_OBJ pObj)
{
    ER er_code;
    er_code = StateMachChk(pObj->Op);

    if(er_code == E_OK)
    {
        m_DcoutOpTbl[pObj->Op](pObj);
        debug_msg("Dcout chg status from %d to %d\r\n",Dcout_GetCurOpmode(),pObj->Op);
        Dcout_SetCurOpmode(pObj->Op);
    }
    else
        debug_err(("^R %s fail, status from %d to %d\r\n",__func__,Dcout_GetCurOpmode(),pObj->Op));

    return er_code;
}

UINT32 GetJpgStatus(void)
{
    UINT32 status;
    if(Dcout_GetCurOpmode() == DCOUT_OP_CLOSE)
        return 0;
    status = m_uiJpgStatus;
    m_uiJpgStatus = 0;
    return status;
}
