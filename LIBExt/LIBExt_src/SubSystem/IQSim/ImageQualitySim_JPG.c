#include "ImageQualitySim_int.h"
#include "jpeg.h"

#if (JPG_DEBUG_FLOW == ENABLE)
#define debug_jpg   debug_err
#else
#define debug_jpg   debug_ind
#endif

UINT32 gJPG_quality = 95;
UINT32 gJPG_slice_h = 0;
UINT32 gJPG_addr_y = 0;
UINT32 gJPG_addr_cb = 0;
UINT32 gJPG_addr_cr = 0;

void IQSim_JPG_SetIQ(JPGCmpParam* jpgParam, int IQSimMode)
{
    gJPG_quality = JPG_quality;
}

void IQSim_JPG_SetIQ_2(JPGCmpParam* jpgParam, int IQSimMode)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_BASIC_IO_REG_STRUCT* pBasic = &(pBin->Basic); 
    
    gJPG_quality = pBasic->BASIC_IO_Register_4.Bit.JPEG_QVAL;
}

void IQSim_JPG_SetSrc(JPGCmpParam* jpgParam, int IQSimMode, SLICE_CTRL* pSliceCtrl)
{
    //slice mode
    
    if(pSliceCtrl->jpg_h + pSliceCtrl->slice_h > pSliceCtrl->total_h)
        gJPG_slice_h = pSliceCtrl->total_h - pSliceCtrl->jpg_h; 
    else
        gJPG_slice_h = pSliceCtrl->slice_h; 
    gJPG_addr_y = gBuf_IME[pSliceCtrl->jid].addr[0];
    gJPG_addr_cb = gBuf_IME[pSliceCtrl->jid].addr[1];
    gJPG_addr_cr = gBuf_IME[pSliceCtrl->jid].addr[2];
    jpgParam->LineOffsetY = gBuf_IME[pSliceCtrl->jid].lineoff;
    jpgParam->LineOffsetU = gBuf_IME[pSliceCtrl->jid].lineoff >> 1;
    jpgParam->LineOffsetV = gBuf_IME[pSliceCtrl->jid].lineoff >> 1;
}

void IQSim_JPG_SetDest(JPGCmpParam* jpgParam, int IQSimMode)
{
    JPG_YUV_FORMAT JpgFmt = 0;
    JPG_YUV_FORMAT fmt_lut[5]={JPGFormatYUV444, JPGFormatYUV422, JPGFormatYUV422, JPGFormatYUV420, JPGFormatYUV420}; 

    //slice mode
    jpgParam->JPGBSAddr = gBuf_JPG.addr[0]; //Compress Bitstream location,
    if(jpgParam->JPGBSBufSize)
        jpgParam->JPGBSBufSize[0] = gBuf_JPG.size;
    jpgParam->Width = gBuf_JPG.w;
    jpgParam->Height = gBuf_JPG.h;
    //jpgParam->bOuputJpgDC = FALSE;
    jpgParam->DCOutCfg.DCEnable = FALSE;
  
    JpgFmt = fmt_lut[IME_OUTPUT_YCC_422_COS];
    jpgParam->Mode  = JpgFmt;
}


//////////////////////////////////////////////////////////////////
void IQSim_JPG_Open(void)
{
}
void IQSim_JPG_Start(JPGCmpParam* jpgParam, int IQSimMode, BOOL bSlice)
{
    char ER_Code;
    jpgParam->Mode |= JPG_Enc_SrcSliceMode;

    debug_jpg(("JPG in w,h,fmt=%d,%d,%08x\r\n", 
        jpgParam->Width,
        jpgParam->Height,
        jpgParam->Mode & 0x03F00000
        ));
    debug_jpg(("JPG in lineoff[3]=%d,%d,%d\r\n", 
        jpgParam->LineOffsetY,
        jpgParam->LineOffsetU,
        jpgParam->LineOffsetV
        ));
    debug_jpg(("JPG in slice h=%d\r\n", 
        gJPG_slice_h));
    debug_jpg(("JPG in addr[3]=%08x,%08x,%08x\r\n", 
        gJPG_addr_y,
        gJPG_addr_cb,
        gJPG_addr_cr
        ));
    debug_jpg(("JPG out quality=%d\r\n", 
        gJPG_quality
        ));
    debug_jpg(("JPG start\r\n"));
    ER_Code = JpegEncOnePic(jpgParam, gJPG_quality);
    if(ER_Code != E_OK)
    {
        debug_err(("%s: JPG ENG CFG error\r\n",__func__));
        return;
    }        
    jpeg_setSliceEncAddr(gJPG_slice_h, gJPG_addr_y, gJPG_addr_cb, gJPG_addr_cr);
    jpeg_setStartEncode();
}
void IQSim_JPG_Next(void)
{
    debug_jpg(("JPG in slice h=%d\r\n", 
        gJPG_slice_h));
    debug_jpg(("JPG in addr[3]=%08x,%08x,%08x\r\n", 
        gJPG_addr_y,
        gJPG_addr_cb,
        gJPG_addr_cr
        ));
    debug_jpg(("JPG next\r\n"));
    jpeg_setSliceEncAddr(gJPG_slice_h, gJPG_addr_y, gJPG_addr_cb, gJPG_addr_cr);
}
void IQSim_JPG_Stop(void)
{
    jpeg_setEndEncode();
}
void IQSim_JPG_Close(void)
{
    jpeg_close();
    gBuf_JPG.size = jpeg_getComSize() +  JPGHeaderSize;
    debug_jpg(("JPG out size=%d\r\n", 
        gBuf_JPG.size));
}
UINT32 IQSim_JPG_Wait(void)
{
    UINT32 uiJpgStatus;
    debug_jpg(("JPG wait\r\n"));
    while(1)
    {
        uiJpgStatus = jpeg_waitDone();
        if (uiJpgStatus & (_JPEG_Slice_Done | _JPEG_Buf_End | _JPEG_Frame_End))
            break;
    }
    debug_jpg(("JPG finsih\r\n"));
    return uiJpgStatus;
}

//////////////////////////////////////////////////////////////////
IQ_BUF gBuf_JPG = {0};

void IQSim_JPG_NewBuffer(UINT32 w, UINT32 h)
{
    UINT32 buf_n = 0;
    UINT32 buf_w = 0;
    UINT32 buf_h = 0;
    IQ_BUF_FMT buf_fmt = 0;
    //output buffer number
    buf_n = 1;
    //output height
    buf_h = h;
    //output width
    buf_w = w;
    //output format
    buf_fmt = IQ_FMT_YCCJPG;
    //alloc buffer
    IQ_BUF_New(&gBuf_JPG, buf_fmt, buf_w, buf_h, 0);
}



