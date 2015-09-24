/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2010.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: Preview Image Process                                              *
* Description:                                                             *
* Author:Yuppie Lin                                                        *
****************************************************************************/
#include <string.h>
#include <stdio.h>
#include "Type.h"

UINT32          gPrvIfeFlag = FALSE;
UINT32          gPrvIfeTotalSteps = 0;
//KH mark
#if 0
IFE_FILT_PRAM   gIfe_prv_param;
#endif
UINT8           gPrvIfe_weights[15] = {2,3,4,4,4,4,4,4,
                              4,4,4,4,4,3,2};
UINT8           gPrvIfe_weightr[8] = {1, 2, 3, 4, 4, 3, 2, 1};
UINT8           gPrvIfe_rngth[4] = {20,22,24,26};

void Set_PrvIfeFlag(UINT32 flag)
{
    gPrvIfeFlag = flag;
}

UINT32   Get_PrvIfeFlag(void)
{
    return gPrvIfeFlag;
}

void Set_PrvIfeTotalSteps(UINT32 steps)
{
    gPrvIfeTotalSteps = steps;
}

UINT32   Get_PrvIfeTotalSteps(void)
{
    return gPrvIfeTotalSteps;
}

void PrvIfeOpen(UINT32 path)
{
//KH mark
#if 0
    IFE_FILT_PRAM   ife_param;
    IFE_OPENOBJ     IFEOpenObj;

    IFEOpenObj.FP_IFEISR_CB = ife_isrCB;
    ife_open(&IFEOpenObj);
    gIfe_prv_param.src_fmt = IFE_SRC_NORMAL;
    //ife_param.filt_dir = IFE_FILTDIRH;
    gIfe_prv_param.inbit = IFE_8BIT;
    gIfe_prv_param.outbit = IFE_8BIT;
    gIfe_prv_param.src = IFE_SRC_DRAM;
    gIfe_prv_param.weights = gPrvIfe_weights;
    ife_param.weightr = gPrvIfe_weightr;
    ife_param.rngth = gPrvIfe_rngth;
    gIfe_prv_param.rdelta = 0;
    gIfe_prv_param.rthceil = 255;
    gIfe_prv_param.sonlyen = IFE_SONLY_ENABLE;
    gIfe_prv_param.sonlylen = 2;
    gIfe_prv_param.clampth = 0;
    gIfe_prv_param.clampdiv = 0;
    gIfe_prv_param.crvmapen = IFE_CRVMAP_DISABLE;
    if(path == 1) {
        Set_PrvIfeTotalSteps(8);
    }
    else {
        Set_PrvIfeTotalSteps(4);
    }
    Set_PrvIfeFlag(ENABLE);
#endif    
}

void PrvIfeClose(void)
{
    Set_PrvIfeFlag(DISABLE);
    ife_close();
}

void PrvIfeProcess(UINT32 step)
{
//KH mark
#if 0
    switch(step) {
        case 0:
            gIfe_prv_param.filt_dir = IFE_FILTDIRH;
            gIfe_prv_param.in_addr  = ime_getActOutputPath1Add(IME_CHANNEL_CB);
            gIfe_prv_param.out_addr = ime_getActOutputPath1Add(IME_CHANNEL_CB);
            gIfe_prv_param.width    = GetIPPSizeInfor(_ImeOut1Hsize)/2;
            gIfe_prv_param.height   = GetIPPSizeInfor(_ImeOut1Vsize);
            gIfe_prv_param.ofsi     = GetIPPSizeInfor(_ImeOut1LineOffset)/2;
            gIfe_prv_param.ofso     = GetIPPSizeInfor(_ImeOut1LineOffset)/2;

            break;
        case 1:
            gIfe_prv_param.filt_dir = IFE_FILTDIRV;
            break;
        case 2:
            gIfe_prv_param.filt_dir = IFE_FILTDIRH;
            gIfe_prv_param.in_addr  = ime_getActOutputPath1Add(IME_CHANNEL_Y);
            gIfe_prv_param.out_addr = ime_getActOutputPath1Add(IME_CHANNEL_Y);
            gIfe_prv_param.width    = GetIPPSizeInfor(_ImeOut1Hsize)/2;
            gIfe_prv_param.height   = GetIPPSizeInfor(_ImeOut1Vsize);
            gIfe_prv_param.ofsi     = GetIPPSizeInfor(_ImeOut1LineOffset);
            gIfe_prv_param.ofso     = GetIPPSizeInfor(_ImeOut1LineOffset);
            break;
        case 3:
            gIfe_prv_param.filt_dir = IFE_FILTDIRV;
            break;
        case 4:
            gIfe_prv_param.filt_dir = IFE_FILTDIRH;
            gIfe_prv_param.in_addr  = ime_getActOutputPath2Add(IME_CHANNEL_CB);
            gIfe_prv_param.out_addr = ime_getActOutputPath2Add(IME_CHANNEL_CB);
            gIfe_prv_param.width    = GetIPPSizeInfor(_ImeOut2Hsize)/2;
            gIfe_prv_param.height   = GetIPPSizeInfor(_ImeOut2Vsize);
            gIfe_prv_param.ofsi     = GetIPPSizeInfor(_ImeOut2LineOffset)/2;
            gIfe_prv_param.ofso     = GetIPPSizeInfor(_ImeOut2LineOffset)/2;

            break;
        case 5:
            gIfe_prv_param.filt_dir = IFE_FILTDIRV;
            break;
        case 6:
            gIfe_prv_param.filt_dir = IFE_FILTDIRH;
            gIfe_prv_param.in_addr  = ime_getActOutputPath2Add(IME_CHANNEL_Y);
            gIfe_prv_param.out_addr = ime_getActOutputPath2Add(IME_CHANNEL_Y);
            gIfe_prv_param.width    = GetIPPSizeInfor(_ImeOut2Hsize)/2;
            gIfe_prv_param.height   = GetIPPSizeInfor(_ImeOut2Vsize);
            gIfe_prv_param.ofsi     = GetIPPSizeInfor(_ImeOut2LineOffset);
            gIfe_prv_param.ofso     = GetIPPSizeInfor(_ImeOut2LineOffset);
            break;
        case 7:
            gIfe_prv_param.filt_dir = IFE_FILTDIRV;
            break;
    }
    ife_setMode(IFE_FILTERING_D2D, &gIfe_prv_param, IFE_NOWAIT);
#endif    
}

