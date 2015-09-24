#include <string.h>
#include "debug.h"
#include "Type.h"
#include "Grph.h"
#include "sie_lib.h"
#include "ipe_lib.h"
#include "ife_lib.h"
#include "ime_lib.h"
#include "datastamp.h"
#include "pll.h"
//#NT#2011/01/22#ethanlau -begin
//#NT#for fpga test
#include "FileSysTsk.h"
#include "ippctrl.h"
//#NT#2011/01/22#ethanlau -end

typedef struct    _DATASTAMP_CTRL{
    BOOL            bEnable;
    UINT32          uiBufferAddr;
    DS_STAMP_INFOR  stmpInfor;
    UINT32          uiXPosition;
    UINT32          uiYPosition;

} DATASTAMP_CTRL, *pDATASTAMP_CTRL;

DATASTAMP_CTRL DataStampCtrlObject={0};//JJ added, 20051004, for data stamping


//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------
//this zone is for external Functions

void datastamp_Imprint(UINT SrcAddr, UINT SrcLineOffset, UINT StampAddr, UINT StampLineOffset, UINT StampH, UINT StampV, UB ColorKey, UINT32 BufferAddr)
{
    UINT32 Y=0;
    ER er_code = GRPH_SUCCESS;

    if(BufferAddr == 0)
    {
        //#NT#2011/07/28#Meg Lin -begin
        //modify for movie dateImprint

        //er_code = grph_setImg1(   StampAddr,
        //                StampH,
        //                StampV,
        //                StampLineOffset);
        er_code = grph_setImg1(   SrcAddr,
                        SrcLineOffset,
                        StampV,
                        StampLineOffset);
        if(er_code != GRPH_SUCCESS)
        {
            debug_msg(" @%d ",er_code);
        }
        //er_code = grph_setImg2(   SrcAddr,
        //                SrcLineOffset);
        er_code = grph_setImg2(   StampAddr,
                        StampH);
        if(er_code!= GRPH_SUCCESS)
        {
            debug_msg(" $%d ",er_code);
        }
        er_code = grph_setAOP(                0,          //BOOL b16BitPrc
                                    GRPH_DST_1,          //GRPH_DST_SEL dstSel
                                    GRPH_AOP_COLOR_EQ,//GRPH_AOP_COLOR_EQ,       //GRPH_AOP_MODE aopMode
                                    ColorKey);  //UINT32 uiProperty == Color Key
        //#NT#2011/07/28#Meg Lin -end
        if(er_code!= GRPH_SUCCESS)
        {
            debug_msg(" #%d ",er_code);
        }
    }
    else
    {
        //prepare buffer
        for(Y=0; Y<StampV; Y++)
            memcpy((void*)(BufferAddr+Y*StampH), (void*)(SrcAddr+Y*SrcLineOffset), StampH);
        //step 1

        grph_setImg1(   BufferAddr,
                        StampH,
                        StampV,
                        StampH);
        grph_setImg2(   StampAddr,
                        StampLineOffset);
        grph_setAOP(    0,          //BOOL b16BitPrc
                                    0,          //GRPH_DST_SEL dstSel
                                    0x03,       //GRPH_AOP_MODE aopMode
                                    ColorKey);  //UINT32 uiProperty == Color Key

        grph_setImg1(   SrcAddr,
                        SrcLineOffset,
                        StampV,
                        StampH);
        grph_setImg2(   BufferAddr,
                        StampH);
        grph_setAOP(    0,          //BOOL b16BitPrc
                        0,          //GRPH_DST_SEL dstSel
                        0x0D,       //GRPH_AOP_MODE aopMode
                        0x1A);  //UINT32 uiProperty == Color Key
    }
}


//----------------------------------------------------------------------------------------------------
#if 0
#pragma mark -
#endif
//----------------------------------------------------------------------------------------------------

//#NT#2008/07/17#JJ Huang -begin
//#NT#DataStamp API Intergration080717
/**
    Enable/Disable Data-Stamp

    Enable/Disable Data-Stamp

    @param[in] bEn          Enable/Disable

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_Enable(BOOL bEn)
{
    DataStampCtrlObject.bEnable=bEn;

    return DATASTAMP_E_OK;
}

/**
    Check Data-Stamp Enable

    Check Data-Stamp Enable

    @return Enable or Disable
*/
BOOL datastamp_IsEnable(void)
{
    return DataStampCtrlObject.bEnable;
}

/**
    Set Date-Image/Stamp information

    Set Date-Image/Stamp information

    @param[in] pstmpinfor   A pointer of structure with Stamp-information

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_SetStampInfor(DS_STAMP_INFOR *pstmpinfor)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    #if 1
    memcpy(&DataStampCtrlObject.stmpInfor,pstmpinfor, sizeof(DS_STAMP_INFOR));
    #else
    DataStampCtrlObject.stmpInfor.AddrY       = pstmpinfor->AddrY;
    DataStampCtrlObject.stmpInfor.AddrCb      = pstmpinfor->AddrCb;
    DataStampCtrlObject.stmpInfor.AddrCr      = pstmpinfor->AddrCr;
    DataStampCtrlObject.stmpInfor.LineOffset  = pstmpinfor->LineOffset;
    DataStampCtrlObject.stmpInfor.Width       = pstmpinfor->Width;
    DataStampCtrlObject.stmpInfor.Height      = pstmpinfor->Height;
    DataStampCtrlObject.stmpInfor.ColorKeyY   = pstmpinfor->ColorKeyY;
    DataStampCtrlObject.stmpInfor.ColorKeyCb  = pstmpinfor->ColorKeyCb;
    DataStampCtrlObject.stmpInfor.ColorKeyCr  = pstmpinfor->ColorKeyCr;
    DataStampCtrlObject.stmpInfor.Format      = pstmpinfor->Format;
    #endif

    if(pstmpinfor->Format == DS_YCCFORMAT_444)
    {
        if(DataStampCtrlObject.stmpInfor.LineOffset%8 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }
    else
    {
        if(DataStampCtrlObject.stmpInfor.LineOffset%4 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }

    return uiReturn;
}

/**
    Get Date-Image/Stamp information

    Get Date-Image/Stamp information

    @param[in] pstmpinfor   A pointer of structure to get Stamp-information

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_GetStampInfor(DS_STAMP_INFOR *pstmpinfor)
{
    #if 1
    memcpy(pstmpinfor, &DataStampCtrlObject.stmpInfor, sizeof(DS_STAMP_INFOR));
    #else
    //#NT#2011/01/17#ethanlau -begin
    //#NT#fpga test for video data stamp,read file from sd card
    char Y[40],Cb[40],Cr[40];
    UW sizeY,sizeCb,sizeCr;
    UINT32 fsStatus;
    datastamp_Enable(ENABLE);
    sprintf(Y, "A:\\FPGA\\Y.RAW");
    sprintf(Cb, "A:\\FPGA\\Cb.RAW");
    sprintf(Cr, "A:\\FPGA\\Cr.RAW");
    sizeY = 480*64;
    sizeCb = sizeCr = 240*32;
    DataStampCtrlObject.stmpInfor.AddrY = ippbuf_GetBuf(IPPBUF_VID_DATA_STAMP) + GetIPPSizeInfor(_ImeOut2Hsize) * Get_IME_OutInfo(IPP_SliceMode_S2S,ImeOutInfo_FMT);
    DataStampCtrlObject.stmpInfor.AddrCb = DataStampCtrlObject.stmpInfor.AddrY + sizeY;
    DataStampCtrlObject.stmpInfor.AddrCr = DataStampCtrlObject.stmpInfor.AddrCb + sizeCb;
    FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, Y, (UINT8 *)(DataStampCtrlObject.stmpInfor.AddrY), &sizeY, 0, FST_TIME_INFINITE);
    if(fsStatus != FST_STA_OK)
    {
        debug_err(("datastamp - card read error!!"));
    }
    FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, Cb, (UINT8 *)(DataStampCtrlObject.stmpInfor.AddrCb), &sizeCb, 0, FST_TIME_INFINITE);
    if(fsStatus != FST_STA_OK)
    {
        debug_err(("datastamp - card read error!!"));
    }
    FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, Cr, (UINT8 *)(DataStampCtrlObject.stmpInfor.AddrCr), &sizeCr, 0, FST_TIME_INFINITE);
    if(fsStatus != FST_STA_OK)
    {
        debug_err(("datastamp - card read error!!"));
    }
    DataStampCtrlObject.stmpInfor.Width = DataStampCtrlObject.stmpInfor.LineOffset = 480;
    DataStampCtrlObject.stmpInfor.Height = 64;
    DataStampCtrlObject.stmpInfor.Format = DS_YCCFORMAT_420;
    DataStampCtrlObject.stmpInfor.ColorKeyY = 0;
    DataStampCtrlObject.stmpInfor.ColorKeyCb = DataStampCtrlObject.stmpInfor.ColorKeyCr = 0;
    //#NT#2011/01/17#ethanlau -end
    pstmpinfor->AddrY        = DataStampCtrlObject.stmpInfor.AddrY;
    pstmpinfor->AddrCb       = DataStampCtrlObject.stmpInfor.AddrCb;
    pstmpinfor->AddrCr       = DataStampCtrlObject.stmpInfor.AddrCr;
    pstmpinfor->LineOffset   = DataStampCtrlObject.stmpInfor.LineOffset;
    pstmpinfor->Width        = DataStampCtrlObject.stmpInfor.Width;
    pstmpinfor->Height       = DataStampCtrlObject.stmpInfor.Height;
    pstmpinfor->ColorKeyY    = DataStampCtrlObject.stmpInfor.ColorKeyY;
    pstmpinfor->ColorKeyCb   = DataStampCtrlObject.stmpInfor.ColorKeyCb;
    pstmpinfor->ColorKeyCr   = DataStampCtrlObject.stmpInfor.ColorKeyCr;
    pstmpinfor->Format       = DataStampCtrlObject.stmpInfor.Format;
    #endif

    return DATASTAMP_E_OK;
}
//#NT#2008/07/17#JJ Huang -end
//#NT#2011/07/28#Meg Lin -begin
UINT32 datastamp_GetStampAddrY(DS_STAMP_INFOR *pstmpinfor)
{
    pstmpinfor->AddrY = DataStampCtrlObject.stmpInfor.AddrY;
    pstmpinfor->AddrCb = DataStampCtrlObject.stmpInfor.AddrCb;
    pstmpinfor->AddrCr = DataStampCtrlObject.stmpInfor.AddrCr;
    return DATASTAMP_E_OK;
}
//#NT#2011/07/28#Meg Lin -end
//#NT#2008/07/17#JJ Huang -begin
//#NT#DataStamp API Intergration080717
/**
    Set Date-Image/Stamp Position

    Set up where to put on stamp

    @param[in] uiXPos       X direction positioning
    @param[in] uiYPos       Y direction positioning

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_SetPosition(UINT32 uiXPos, UINT32 uiYPos)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    DataStampCtrlObject.uiXPosition = uiXPos;
    DataStampCtrlObject.uiYPosition = uiYPos;

    if(uiXPos>1024 || uiYPos>1024)
        uiReturn = DATASTAMP_E_PARAERROR;

    return uiReturn;
}

/**
    Get Date-Image/Stamp Position

    Get the information of where to put on stamp

    @param[in] puiXPos      A pointer to get X direction positioning
    @param[in] puiYPos      A pointer to get Y direction positioning

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastamp_GetPosition(UINT32* puiXPos, UINT32* puiYPos)
{
    *puiXPos = DataStampCtrlObject.uiXPosition;
    *puiYPos = DataStampCtrlObject.uiYPosition;

    return DATASTAMP_E_OK;
}
//#NT#2008/07/17#JJ Huang -end

/**
    DataStamp Bacis Imprinting API

    Imprint stamp-image into background-image

    @param[in] BackgroundInfo   A pointer of structure with background-image information
    @param[in] StampInfo        A pointer of structure with stamp-image information

    @return void
*/
void datastamp_ImprintYCC(DS_BACKGROUND_INFOR *BackgroundInfo,DS_STAMP_INFOR *StampInfo, UINT32 BufferAddr)
{
//    UINT YSrcStart,CbSrcStart,CrSrcStart;
//    UINT DataStampHStartPxl,DataStampVStartPxl;
//    UINT Shift=10;
    UINT32 _Shift;

//    if(BufferAddr == 0)
//    {
        //Assign New Buffer
//        BufferAddr = DataStampCtrlObject.uiBufferAddr;
//    }

//    //Fixed data stamp coordinate same as large image
//    DataStampHStartPxl = 0;//((StampSrcInfo->SrcHsize)*Hposition)>>Shift;//StampStartRatioShift;
//    DataStampHStartPxl &= 0xfffffff8;//needed to be word-aligned for Cb,Cr
//    DataStampVStartPxl = 0;//((StampSrcInfo->SrcVsize)*Vposition)>>Shift;//StampStartRatioShift;

//    YSrcStart=(StampSrcInfo->SrcAddrY)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)+DataStampHStartPxl;
//    CbSrcStart=(StampSrcInfo->SrcAddrCb)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)/2+DataStampHStartPxl/2;
//    CrSrcStart=(StampSrcInfo->SrcAddrCr)+DataStampVStartPxl*(StampSrcInfo->SrcYLineOffset)/2+DataStampHStartPxl/2;
    //#NT#2011/01/17#ethanlau -begin
    //#NT#add 420 fmt

    switch(StampInfo->Format)
    {
        case DS_YCCFORMAT_420:
            _Shift = 2;
            break;
        default:
            _Shift =1;
            break;
    }

                //Y
                datastamp_Imprint(BackgroundInfo->AddrY,
                            BackgroundInfo->LineOffset,
                            StampInfo->AddrY,
                            StampInfo->Width,
                            StampInfo->Width,
                            StampInfo->Height,
                            StampInfo->ColorKeyY,
                            BufferAddr);

                //Cb
                datastamp_Imprint(BackgroundInfo->AddrCb,
                            (BackgroundInfo->LineOffset)/2,
                            StampInfo->AddrCb,
                            (StampInfo->Width)/2,
                            (StampInfo->Width)/2,
                            StampInfo->Height/_Shift,
                            StampInfo->ColorKeyCb,
                            BufferAddr);


                //Cr
                datastamp_Imprint(BackgroundInfo->AddrCr,
                            (BackgroundInfo->LineOffset)/2,
                            StampInfo->AddrCr,
                            (StampInfo->Width)/2,
                            (StampInfo->Width)/2,
                            StampInfo->Height/_Shift,
                            StampInfo->ColorKeyCr,
                            BufferAddr);

    //#NT#2011/01/17#ethanlau -end
}

//chg stamp fmt to target img fmt(only 422 to 420)
//ImgFmt: Target Img fmt
//workingBuf: TempBuf. size must be no less than stamp size
//_case:1 for normal image,2 for thumb.
void datastamp_ChkStampFmt(UINT32 ImgFmt,DS_STAMP_INFOR *pStampInfo,UINT32 workingBuf,UINT32 _case)
{
    //fmt from ime map to DS fmt
    switch(ImgFmt)
    {
        case IME_OUTPUT_YCC_420_COS:
        case IME_OUTPUT_YCC_420_CEN:
            ImgFmt = DS_YCCFORMAT_420;
            break;
        case IME_OUTPUT_YCC_422_COS:
        case IME_OUTPUT_YCC_422_CEN:
            ImgFmt = DS_YCCFORMAT_422;
            break;
        case IME_OUTPUT_YCC_444:
            ImgFmt = DS_YCCFORMAT_444;
            break;
        default:
            break;
    }

    if(ImgFmt == pStampInfo->Format)
        return;
    //chg org ds fmt to target fmt
    switch(ImgFmt)
    {
        case DS_YCCFORMAT_420:
            grph_open();
            grph_init(PLL_CLKSEL_GRAPHIC_160);
            grph_enableDMA();
            grph_setImg1(pStampInfo->AddrCb,
                 pStampInfo->Width*2,
                 pStampInfo->Height/2,
                 pStampInfo->Width);
            grph_setImg2(workingBuf,pStampInfo->Width);
            grph_setAOP(FALSE,GRPH_DST_2,GRPH_AOP_A_COPY,0);

            grph_setImg1(workingBuf,
                         pStampInfo->Width,
                         pStampInfo->Height/2,
                         pStampInfo->Width);
            grph_setImg2(pStampInfo->AddrCb,pStampInfo->Width);
            grph_setAOP(FALSE,GRPH_DST_2,GRPH_AOP_A_COPY,0);

            grph_setImg1(pStampInfo->AddrCr,
                 pStampInfo->Width*2,
                 pStampInfo->Height/2,
                 pStampInfo->Width);
            grph_setImg2(workingBuf,pStampInfo->Width);
            grph_setAOP(FALSE,GRPH_DST_2,GRPH_AOP_A_COPY,0);

            grph_setImg1(workingBuf,
                         pStampInfo->Width,
                         pStampInfo->Height/2,
                         pStampInfo->Width);
            grph_setImg2(pStampInfo->AddrCr,pStampInfo->Width);
            grph_setAOP(FALSE,GRPH_DST_2,GRPH_AOP_A_COPY,0);
            grph_disableDMA();
            grph_swReset();
            grph_close();
            break;
        case DS_YCCFORMAT_422:
        default:
            break;
    }

    pStampInfo->Format = ImgFmt;
    //#NT#2011/08/25#Ethan Lau -begin
    //#NT#reset  datastamp infor
    switch(_case)
    {
        case 1:
            datastamp_SetStampInfor(pStampInfo);
            break;
        case 2:
            datastamp_SetStampInforThumb(pStampInfo);
            break;
        default:
            break;
    }
    //#NT#2011/08/25#Ethan Lau -end
}

void datastamp_Open(UINT32 uiClkRate)
{
    grph_open();
    //#NT#full speed, for slice mode grph application
    grph_init(uiClkRate);//PLL_CLKSEL_GRAPHIC_160
    grph_enableDMA();
}

void datastamp_Close(void)
{
    grph_disableDMA();//DMA seems to keep moving unless being disabled,
    grph_swReset();
    grph_close();//mark this line or the second picture will make the cam. abnormal
}

//#NT#2009/08/10#Hideo Lin -begin
//#NT#Date stamp for screen-nail test
DATASTAMP_CTRL DataStampCtrlObjectThumb = {0};

/**
    Enable/Disable Data-Stamp

    Enable/Disable Data-Stamp

    @param[in] bEn          Enable/Disable

    @return
        - @b DATASTAMP_E_OK: operation successful
        - @b Ohterwise: Error
*/
UINT32 datastampThumb_Enable(BOOL bEn)
{
    DataStampCtrlObjectThumb.bEnable=bEn;

    return DATASTAMP_E_OK;
}

/**
    Check Data-Stamp Enable

    Check Data-Stamp Enable

    @return Enable or Disable
*/
BOOL datastampThumb_IsEnable(void)
{
    return DataStampCtrlObjectThumb.bEnable;
}

UINT32 datastamp_SetStampInforThumb(DS_STAMP_INFOR *pstmpinfor)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    memcpy(&DataStampCtrlObjectThumb, pstmpinfor, sizeof(DS_STAMP_INFOR));

    if(pstmpinfor->Format == DS_YCCFORMAT_444)
    {
        if(DataStampCtrlObjectThumb.stmpInfor.LineOffset%8 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }
    else
    {
        if(DataStampCtrlObjectThumb.stmpInfor.LineOffset%4 !=0)
        {
            uiReturn = DATASTAMP_E_SIZEERROR;
        }
    }

    return uiReturn;
}

UINT32 datastamp_GetStampInforThumb(DS_STAMP_INFOR *pstmpinfor)
{
    memcpy(pstmpinfor, &DataStampCtrlObjectThumb, sizeof(DS_STAMP_INFOR));

    return DATASTAMP_E_OK;
}

UINT32 datastamp_SetPositionThumb(UINT32 uiXPos, UINT32 uiYPos)
{
    UINT32 uiReturn=DATASTAMP_E_OK;

    DataStampCtrlObjectThumb.uiXPosition = uiXPos;
    DataStampCtrlObjectThumb.uiYPosition = uiYPos;

    if(uiXPos>1024 || uiYPos>1024)
        uiReturn = DATASTAMP_E_PARAERROR;

    return uiReturn;
}

UINT32 datastamp_GetPositionThumb(UINT32* puiXPos, UINT32* puiYPos)
{
    *puiXPos = DataStampCtrlObjectThumb.uiXPosition;
    *puiYPos = DataStampCtrlObjectThumb.uiYPosition;

    return DATASTAMP_E_OK;
}
//#NT#2009/08/10#Hideo Lin -end