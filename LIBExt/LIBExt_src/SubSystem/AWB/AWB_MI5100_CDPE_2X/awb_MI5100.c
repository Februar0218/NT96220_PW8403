/**
    Auto white balance flow.

    ae flow.

    @file       awb_MI5100.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "type.h"
#include "debug.h"
#include "Awb_alg.h"
#include "Ae_alg.h"
#include "Awb_MI5100_int.h"
#include "Awb_Api.h"
#include "ImageAlgInfor.h"
#include "aaa_lib.h"
#include "calibration.h"

#define AWB_DEBUG_MODE DISABLE
#if (AWB_DEBUG_MODE == ENABLE)
#define AWB_IND(...)      debug_msg("AWB: "__VA_ARGS__)
#define AWB_ERR(...)      debug_msg("^RAWB: "__VA_ARGS__)
#else
#define AWB_IND(...)
#define AWB_ERR(...)      debug_msg("^RAWB: "__VA_ARGS__)
#endif

static AWBALG_INFO AwbInfo = {0};

static UINT32 AWBEnter = 1;
static UINT32 OldRGain, OldGGain, OldBGain;
static UINT32 RGain, GGain, BGain;


void AWB_Init(void)
{
    CAL_DATA CalInfo;
    Cal_GetCalData(&CalInfo);
    
    AwbInfo.KRgain = AWB_DEF_KRGAIN;
    AwbInfo.KBgain = AWB_DEF_KBGAIN;
    if( (CalInfo.CalStatus_AWBGS == _AWB_Status) && (CalInfo.CalStatus_AWB == _AWB_Status))
    {
        AwbInfo.KRgain = CalInfo.Cal_AWBGain.Rgain*100/CalInfo.Cal_AWBGSGain.Rgain;
        AwbInfo.KBgain = CalInfo.Cal_AWBGain.Bgain*100/CalInfo.Cal_AWBGSGain.Bgain;
    }
///////////////////???//////////////////////
if ( AwbInfo.KRgain > 101) AwbInfo.KRgain = 101;
if ( AwbInfo.KBgain > 101) AwbInfo.KBgain = 101;
if ( AwbInfo.KRgain < 99) AwbInfo.KRgain = 99;
if ( AwbInfo.KBgain < 99) AwbInfo.KBgain = 99;
////////////////////////////////    
    AwbInfo.OutlR = AWB_OUTL_R;
    AwbInfo.OutlB = AWB_OUTL_B;
    AwbInfo.WThreshold = AWB_W_THRES;

    AWBEnter = 1;

    RGain = OldRGain = AWB_DEF_RGAIN;
    GGain = OldGGain = AWB_DEF_GGAIN;
    BGain = OldBGain = AWB_DEF_BGAIN;

    aaa_SetAwbRate(AWB_RATE);
    //ethan_20110803_remove,config by sie
    //AWB_setGain(AWB_DEF_RGAIN, AWB_DEF_GGAIN, AWB_DEF_BGAIN);
}


void AWB_Process(void)
{
    ImageAlgInfo *ImgInfo;
    AWB_MODE Mode;
    UINT32 i;
    ImgInfo = UI_GetParameter();
    Mode = AWB_getMode(ImgInfo->WBMode);

    if(Mode == AWB_MDOE_AUTO)
    {
        AwbInfo.CurEV = AEAlg_GetPrvEVValue();
        AwbInfo.Tab = AwbTAB;
        AwbInfo.TabCnt = 2;
        AWB_getCA(&AwbInfo.RAcc[0], &AwbInfo.GAcc[0], &AwbInfo.BAcc[0], AWBALG_WIN_NUM);
        for(i=0;i<AWBALG_WIN_NUM;i++)
        {
            AwbInfo.RAcc[i]=(AwbInfo.RAcc[i]*AwbInfo.KRgain)/100;
            AwbInfo.GAcc[i]=AwbInfo.GAcc[i];
            AwbInfo.BAcc[i]=(AwbInfo.BAcc[i]*AwbInfo.KBgain)/100;       
        }
        AWB_autoProc(&AwbInfo);
    }
    else
    {
        AWB_manualProc(Mode);
    }
}

void AWB_GetPrvGain(UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain)
{
    *Rgain = RGain;
    *Ggain = GGain;
    *Bgain = BGain;
}

#if 0
#endif
//------------------------------------------------------------------------------
void AWB_autoProc(AWBALG_INFO *Info)
{
    UINT32 NewRGain, NewGGain, NewBGain;

    NewGGain = AWB_DEF_GGAIN;
    if(AWBALG_ChkImg(Info, &NewRGain, &NewBGain) == AWBALG_OK)
    {
        if (AWBEnter == 1)
        {
            OldRGain = NewRGain;
            OldGGain = NewGGain;
            OldBGain = NewBGain;
            AWBEnter = 0;
        }
        else
        {
            OldRGain = RGain;
            OldGGain = GGain;
            OldBGain = BGain;
        }

        RGain = (NewRGain * 3 + OldRGain) / 4;
        GGain = (NewGGain * 3 + OldGGain) / 4;
        BGain = (NewBGain * 3 + OldBGain) / 4;



        // set limit for A light [NTK#RandyHo]<2012/06/19>
        // Rg        Bg -- face on kodak gray card in light box
        // 216      509 --F/A
        // 380      316 --D65
        // 393      300 -- D75
        // 325      448 -- CWF
/*
        if((AEAlg_GetPrvEVValue()<((1 << 9) * 10)) && (10*BGain/RGain >  16)) // check if indoor & F/A light
        {
            if(RGain < 260)
            {
                RGain = 260;
            }
            if(BGain > 470)
            {
                BGain = 470;
            }
        }*/
    }
    else
    {
        if (Info->CurEV > Info->Tab->EvH)
        {
            RGain = (MwbTAB[AWB_MODE_DAYLIGHT][0] * 3 + OldRGain) / 4;
            GGain = AWB_DEF_GGAIN;
            BGain = (MwbTAB[AWB_MODE_DAYLIGHT][1] * 3 + OldBGain) / 4;
            OldRGain = RGain;
            OldGGain = GGain;
            OldBGain = BGain;
        }
    }
    AWB_setGain(RGain, GGain, BGain);
}


void AWB_manualProc(AWB_MODE Idx)
{
    RGain = MwbTAB[Idx][0]*100/AwbInfo.KRgain;
    GGain = AWB_DEF_GGAIN;
    BGain = MwbTAB[Idx][1]*100/AwbInfo.KRgain;
    AWB_setGain(RGain, GGain, BGain);
}

void AWB_setGain(UINT32 RGain, UINT32 GGain, UINT32 BGain)
{
    Sen_SetColorGain(RGain, BGain, GGain);
}

void AWB_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    if((RGain==0)||(GGain==0)||(BGain==0))
    {
        *Rg = AWB_DEF_RGAIN;
        *Gg = AWB_DEF_GGAIN;
        *Bg = AWB_DEF_BGAIN;
    }
    else
    {
        *Rg = RGain;//*AwbInfo.KRgain/100;
        *Gg = GGain;
        *Bg = BGain;//*AwbInfo.KBgain/100;
    }
}

void AWB_getCA(UINT32 *R, UINT32 *G, UINT32 *B, UINT32 Size)
{
    Sen_GetSubOutRGBinfo(R,G,B,Size);
}


AWB_MODE AWB_getMode(UINT32 Idx)
{
    if (Idx == _IMGWBMode_Auto) return AWB_MDOE_AUTO;
    if (Idx == _IMGWBMode_Daylight) return AWB_MODE_DAYLIGHT;
    if (Idx == _IMGWBMode_Cloudy) return AWB_MODE_CLOUDY;
    if (Idx == _IMGWBMode_Tungsten) return AWB_MODE_TUNGSTEN;
    if (Idx == _IMGWBMode_Fluorescent1) return AWB_MODE_FLUORESCENT1;
    if (Idx == _IMGWBMode_Fluorescent2) return AWB_MODE_FLUORESCENT2;
    if (Idx == _IMGWBMode_Sunset) return AWB_MODE_SUNSET;
    if (Idx == _IMGWBMode_BirthdayParty) return AWB_MODE_BIRTHDAYPARTY;
    if (Idx == _IMGWBMode_Customer1) return AWB_MODE_CUSTOMER1;
    if (Idx == _IMGWBMode_Customer2) return AWB_MODE_CUSTOMER2;
    if (Idx == _IMGWBMode_Customer3) return AWB_MODE_CUSTOMER3;
    return AWB_MDOE_AUTO;
}

UINT32 AWB_WriteDebugInfo(UINT32 Addr)
{
    UINT32 *TabAdr, TabIdx, i, j;


    Addr += sprintf((char *)Addr, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", RGain, GGain, BGain);
    Addr += sprintf((char *)Addr, "Weight Table:\r\n");

    TabAdr = (UINT32 *)AWBALG_GetWeightInfo(&TabIdx);

    for(j = 0; j < AWBALG_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"W  \t: ");

        for (i = 0; i < AWBALG_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld\t", TabAdr[i + (j * AWBALG_WIN_X)]);
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }

    Addr += sprintf((char *)Addr,"\t");
    for(j = 0; j < 80; j++)
        Addr += sprintf((char *)Addr,"-");
    Addr += sprintf((char *)Addr,"\r\n");

    for(j = 0; j < AWBALG_WIN_Y; j++)
    {
        Addr += sprintf((char *)Addr,"RGB  \t: ");

        for (i = 0; i < AWBALG_WIN_X; i++)
        {
            Addr += sprintf((char *)Addr,"%3ld %3ld %3ld|\t",AwbInfo.RAcc[j*AWBALG_WIN_Y+i],AwbInfo.GAcc[j*AWBALG_WIN_Y+i],AwbInfo.BAcc[j*AWBALG_WIN_Y+i] );
        }

        Addr += sprintf((char *)Addr,"\r\n");
    }
    return Addr;
}

extern void AWB_ReCalColorGain(UINT32 addr,UINT32 Lineoffset,UINT32 Hsize,UINT32 Vsize,UINT32 DataBit,UINT32 SubRate,UINT32 CFA)
{
}