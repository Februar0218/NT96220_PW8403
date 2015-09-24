#include "ImageAlgInfor.h"
#include "Alg_IPLMode.h"
#include "UMSDC.h"

#include "aaa_lib.h"
#include "jpeg.h"
#include "ImageQualitySim_int.h"

BOOL bIQSimMode = FALSE;
BOOL bIQSimExec = FALSE;
//memory
UINT32 uiTotalAddr = 0;
UINT32 uiTotalSize = 0;
//bin
UINT32 uiBinAddr = 0;
UINT32 uiBinSize = 0;
//raw
UINT32 uiRawAddr = 0;
UINT32 uiRawSize = 0;
UINT32 uiRawWidth = 0;
UINT32 uiRawHeight = 0;
UINT32 uiRawBpp = 0;
UINT32 uiRawCfa = 0;
//jpg
UINT32 uiJpgAddr = 0;
UINT32 uiJpgSize = 0;
UINT32 uiJpgWidth = 0;
UINT32 uiJpgHeight = 0;
UINT32 uiJpgQulaity = 0;

#define IQSIM_INFO_BINADDR

UINT32 IQSim_Init(void)
{
    UINT32   IPL_Status_Sim = 0;
    
    debug_msg("[IQSim] INIT\r\n");
    
    //bin addr is assigned by project
    debug_msg("[IQSim] bin addr=0x%x\r\n", uiBinAddr);
    
    uiBinSize = 
        sizeof(IQSIM_BIN);
    debug_msg("[IQSim] bin size=%d\r\n", uiBinSize);

    IPL_Status_Sim = IPL_GetMode();
    if((IPL_Status_Sim == IPL_MODE_PREVIEW)  && (USBMSDC_GetStatus() == MSDC_COMMANDOK) && (bIQSimMode == FALSE)) // IPL_MODE_PREVIEW = 2
    { 
        debug_msg("IPL is under Photo mode (with Sensor)\r\n");

        debug_msg("stop AE,AWB,...\r\n");
        aaa_AeLock(TRUE);
        aaa_AwbLock(TRUE);
        //aaa_FdLock(TRUE);
        //aaa_SdLock(TRUE);
        //aaa_DisLock(TRUE); //Linkin

        debug_msg("stop IPL\r\n");
        sie_waitVD(10);
        {
            //StopPreviewDramData();
            IPL_OBJ IplObj;
            if (IPL_GetMode() != IPL_MODE_OFF)
            {
                // Set to off mode
                IplObj.uiCmd = IPL_CMD_CHG_MODE;
                IplObj.Mode = IPL_MODE_OFF;
                IPL_SetCmd(&IplObj);
            }
        }                
        bIQSimMode = TRUE;
    }
    else if((IPL_Status_Sim == IPL_MODE_OFF) && (USBMSDC_GetStatus() == MSDC_COMMANDOK) && (bIQSimMode == FALSE)) // IPL_MODE_OFF = 2
    {
        debug_msg("IPL is not under Photo mode\r\n");
        debug_msg("stop IPL\r\n");
        {
            ER e;
            e = ime_pause(); if(e!=E_OK)goto stop_end;
            e = ipe_pause(); if(e!=E_OK)goto stop_end;
            e = sie_pause(); if(e!=E_OK)goto stop_end;
            sie_waitVD(1);
            e = ipe_reset(ON); if(e!=E_OK)goto stop_end;
            e = ipe_reset(OFF); if(e!=E_OK)goto stop_end;
            e = ime_setReset(); if(e!=E_OK)goto stop_end;
            e = sie_close(); if(e!=E_OK)goto stop_end;
            e = ipe_close(); if(e!=E_OK)goto stop_end;
            e = ife_close(); if(e!=E_OK)goto stop_end;
            e = ime_close(); if(e!=E_OK)goto stop_end;
            jpeg_setEndEncode();
            e = jpeg_close(); if(e!=E_OK)goto stop_end;
        }
        stop_end:
            bIQSimMode = TRUE;
    }
    else     
    {
        //if(bIQSimMode == TRUE)
        // do something ...
    }
    
    if(bIQSimMode == FALSE)
    {
        debug_err(("EVB cannot ready!\r\n"));
        return 0; 
    }
    else
    {
        debug_msg("EVB ready\r\n");
    }
    return 1;
}

UINT32 IQSim_Exit(void)
{
    debug_msg("[IQSim] EXIT\r\n");
    return 1;
}

BUF_ALLOC gAllBuf;

UINT32 IQSim_Exec(void)
{
    IQSIM_BIN* pBin = (IQSIM_BIN*)uiBinAddr;
    NT96220_BASIC_IO_REG_STRUCT* pBasic = &(pBin->Basic); 
    debug_msg("[IQSim] EXECUTE\r\n");
    
    uiRawAddr = ((uiBinAddr + uiBinSize)+15)/16*16;
    uiRawWidth = pBasic->BASIC_IO_Register_0.Bit.IN_WIDTH;
    uiRawHeight = pBasic->BASIC_IO_Register_1.Bit.IN_HEIGHT;
    uiRawBpp = pBasic->BASIC_IO_Register_4.Bit.IN_PACKBITS; if(uiRawBpp == 0)uiRawBpp = 8; else uiRawBpp = 10;
    IQSim_RAW_SetBPP(uiRawBpp);
    IQSim_RAW_SetCFA(pBasic->BASIC_IO_Register_4.Bit.IN_CFA);
    uiRawSize = uiRawWidth*uiRawHeight*uiRawBpp/8;
    debug_msg("[IQSim] raw addr=0x%x\r\n", uiRawAddr);
    debug_msg("[IQSim] raw size=0x%x\r\n", uiRawSize);
    debug_msg("[IQSim] raw w,h,bpp=%d,%d,%d\r\n", uiRawWidth, uiRawHeight, uiRawBpp);

    uiTotalAddr = uiRawAddr + uiRawSize;
    uiTotalSize = uiTotalSize - uiBinSize - uiRawSize;
    BUF_ALLOC_Assign(&gAllBuf);
    BUF_ALLOC_Open(uiTotalAddr, uiTotalSize);

    uiJpgWidth = pBasic->BASIC_IO_Register_2.Bit.OUT_WIDTH;
    uiJpgHeight = pBasic->BASIC_IO_Register_3.Bit.OUT_HEIGHT;
    uiJpgQulaity = pBasic->BASIC_IO_Register_4.Bit.JPEG_QVAL;
    debug_msg("[IQSim] jpg w,h,quality=%d,%d,%d\r\n", uiJpgWidth, uiJpgHeight, uiJpgQulaity);

    debug_msg("[IQSim] alloc sie buf\r\n");
    IQSim_SIE_NewBuffer(uiRawWidth, uiRawHeight);
    debug_msg("[IQSim] alloc ime buf\r\n");
    IQSim_IME_NewBuffer(uiJpgWidth, uiJpgHeight);
    debug_msg("[IQSim] alloc ife buf\r\n");
    IQSim_IFE_NewBuffer(uiJpgWidth, uiJpgHeight);
    debug_msg("[IQSim] attach raw buf\r\n");
    IQ_BUF_Attach(&gBuf_RAW, 
        (uiRawBpp == 10)?(IQ_FMT_RAW10):(IQ_FMT_RAW8), 
        uiRawWidth, uiRawHeight, 0, uiRawSize, uiRawAddr, 0, 0);
    //debug_msg("[IQSim] alloc jpg buf\r\n");
    //IQSim_JPG_NewBuffer(uiJpgWidth, uiJpgHeight);
    debug_msg("[IQSim] attach jpg buf\r\n");
    IQ_BUF_Attach(&gBuf_JPG, 
        IQ_FMT_YCCJPG, 
        uiJpgWidth, uiJpgHeight, 0, uiRawSize, uiRawAddr, 0, 0);

    bIQSimExec = TRUE;
    //capture begin
    IQSim_Capture(0);    
    //capture end    
    uiJpgAddr = gBuf_JPG.addr[0];
    uiJpgSize = gBuf_JPG.size;
    debug_msg("[IQSim] jpg addr=0x%x\r\n", uiJpgAddr);
    debug_msg("[IQSim] jpg size=0x%x\r\n", uiJpgSize);
    bIQSimExec = FALSE;
    
    BUF_ALLOC_Close();
    
    return 0;
}


