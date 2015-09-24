/*
    IPL command API

    This file is the API of the IPL control.

    @file       Alg_IPLMode.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#include "ErrorNo.h"
#include "pll.h"
#include "top.h"
#include "dcout.h"
#include "sen_isr.h"
#include "utility.h"
#if NT99140_220
#include "IPP_NT99140_220_int.h"
/** \addtogroup mISYSAlg */
//@{


typedef ER (*FP_ChgMode) (IPL_OBJ *Obj);
ER IPL_ChgModeOffTo(IPL_OBJ *Obj);
ER IPL_ChgModeIdleTo(IPL_OBJ *Obj);
ER IPL_ChgModePrvTo(IPL_OBJ *Obj);
ER IPL_ChgModeVidTo(IPL_OBJ *Obj);
ER IPL_ChgModeRecTo(IPL_OBJ *Obj);
ER IPL_ChgModePccTo(IPL_OBJ *Obj);
ER IPL_ChgModeCapTo(IPL_OBJ *Obj);

static IPL_OBJ gIPLCurObj = {0};
//#NT#20090929#Bowen Li -begin
//#NT#add new function for change clock
static IPL_MODE gIPLNextMode = IPL_MODE_OFF;
//#NT#20090929#Bowen Li -end
static FP_ChgMode ChgModeAct[IPL_MODE_NUM] =
{
    IPL_ChgModeOffTo,
    IPL_ChgModeIdleTo,
    IPL_ChgModePrvTo,
    IPL_ChgModeVidTo,
    IPL_ChgModeRecTo,
    IPL_ChgModePccTo,
    IPL_ChgModeCapTo,
};
//ex. for ipp control when video rec. resolution and Disp. resolution have matching issue
static __inline void IPL_EngMode(UINT32 RecH,UINT32 RecV,UINT32 DispH,UINT32 DispV)
{
    if((RecH >= (DispH<<1)) || (RecV >= (DispV<<1)))
    {
        SetIPPFuncInfor(JPG_DCOUT_EN, ENABLE);
    }
    else
        SetIPPFuncInfor(JPG_DCOUT_EN, DISABLE);
}

//ex. for ippctrl strong flow
static __inline IPPCTRL_FLOW IPL_IPPctrlflow(void)
{
    return IPP_SliceMode_S2S;
}

static __inline IPPCTRL_FLOW IPL_IPPctrlflow2(void)
{
    return IPP_SliceMode_S2F;
}

/**
    IPL get mode.

    IPL get current mode.

    @return return current mode.
*/
IPL_MODE IPL_GetMode(void)
{
    return gIPLCurObj.Mode;
}


//#NT#20090929#Bowen Li -begin
//#NT#add new function for change clock
IPL_MODE IPL_GetNextMode(void)
{
    return gIPLNextMode;
}



/**
    IPL change mode

    IPL change mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
//#NT#20090929#Bowen Li -end
ER IPL_ChgMode(IPL_OBJ *Obj)
{
    ER rt;
    if (ChgModeAct[gIPLCurObj.Mode] == NULL)
    {
        debug_err(("IPL chg mode error (%d)\r\n", Obj->Mode));
        return E_NOSPT;
    }
    //#NT#20090929#Bowen Li -begin
    //#NT#add new function for change clock
    gIPLNextMode = Obj->Mode;
    //#NT#20090929#Bowen Li -end
    //#NT#2011/03/28#Ethan Lau -begin
    //#NT#chk path1 outputsize
    if((Obj->uiImeOutWidth1 == 0)||(Obj->uiImeOutHeight1 == 0)||(Obj->uiImeOutYoft1 == 0))
    {
        Obj->uiImeOutWidth1 = gIPLCurObj.uiImeOutWidth1;
        Obj->uiImeOutHeight1 = gIPLCurObj.uiImeOutHeight1;
        Obj->uiImeOutYoft1 = gIPLCurObj.uiImeOutWidth1;
        debug_err(("%d %s :force chging to previous path1 resolution(%dx%d)\r\n",__LINE__,__func__,gIPLCurObj.uiImeOutWidth1,gIPLCurObj.uiImeOutHeight1));
    }
    //#NT#2011/03/28#Ethan Lau -end
    rt = ChgModeAct[gIPLCurObj.Mode](Obj);

    if (rt == E_OK)
    {
        debug_msg("IPL chg mode from %d to %d\r\n", gIPLCurObj.Mode, Obj->Mode);

        //capture only keep mode information
        if(Obj->Mode != IPL_MODE_CAP )
            memcpy(&gIPLCurObj,Obj,sizeof(IPL_OBJ));
        else
            gIPLCurObj.Mode = Obj->Mode;
    }
    else
    {
        debug_err(("IPL chg mode fail (%d mode to %d)\r\n",gIPLCurObj.Mode, Obj->Mode));
    }
    return rt;
}

//#NT#2009/12/11#Bowen Li -begin
//#NT#add Semaphore for IPL
ER IPL_Lock(void)
{
    ER erReturn;

    erReturn = wai_sem(SEMID_IPL);
    if (erReturn != E_OK)
        return erReturn;
    return E_OK;
}

ER IPL_Unlock(void)
{
    ER erReturn;

    erReturn = sig_sem(SEMID_IPL);
    if (erReturn != E_OK)
        return erReturn;
    return E_OK;
}

/**
    IPL set command

    IPL set command.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_SetCmd(IPL_OBJ *Obj)
{
//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
    ER erReturn, Status;
    IPPCTRL_OBJ IPPCtrl_obj = {0};
    static IPL_MODE EnterSleepMode = IPL_MODE_OFF;

    erReturn = IPL_Lock();
    if (erReturn != E_OK)
    {
        debug_err(("^R IPL_Lock fail\r\n"));
        return erReturn;
    }

    if ((Obj->uiCmd & IPL_CMD_CHG_MODE) || (Obj->uiCmd & IPL_CMD_WAKE_UP) || (Obj->uiCmd & IPL_CMD_SLEEP))
    {
        if (Obj->uiCmd & IPL_CMD_SLEEP)
        {
            if (gIPLCurObj.Mode!= IPL_MODE_IDLE)
            {
                EnterSleepMode = gIPLCurObj.Mode;
            }
            Obj->Mode = IPL_MODE_IDLE;
        }
        else if (Obj->uiCmd & IPL_CMD_WAKE_UP)
        {
            if (gIPLCurObj.Mode != IPL_MODE_IDLE)
            {
                debug_err(("^R IPL command fail\r\n"));
                Status = E_SYS;
                goto IPL_END;
            }
            Obj->Mode = EnterSleepMode;
        }
        Status = IPL_ChgMode(Obj);
        goto IPL_END;
    }
//#NT#2009/10/08#Bowen Li -end
    else if (Obj->uiCmd != NULL)
    {
        if ((gIPLCurObj.Mode == IPL_MODE_OFF) || (gIPLCurObj.Mode == IPL_MODE_IDLE) || (gIPLCurObj.Mode == IPL_MODE_CAP))
        {
            debug_err(("IPL command fail\r\n"));
            Status= E_SYS;
            goto IPL_END;
        }

        if (Obj->uiCmd & IPL_CMD_SET_DZOOM)
        {
            IPPCtrl_obj.opmode = IPP_CHGIOSIZE_DZOOM;
            IPPCtrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            IPPCTRL_SetCmd(IPPCtrl_obj);
            Status= E_OK;
        }
        //#NT#2009/12/11#Niven Cho -begin
        //#NT#Added., Unknown command handler
        else
        {
            Status = E_PAR;
            debug_err(("unknown Obj->uiCmd=%d\r\n",Obj->uiCmd));
        }
        //#NT#2009/12/11#Niven Cho -end
    }
    else
    {
        Status= E_NOSPT;
        debug_err(("IPL command error (%d)\r\n", Obj->uiCmd));
        goto IPL_END;
    }

IPL_END:

    erReturn = IPL_Unlock();
    if (erReturn != E_OK)
    {
        debug_err(("IPL_Unlock fail\r\n"));
        return erReturn;
    }
    return Status;
}
//#NT#2009/12/11#Bowen Li -end

/**
    IPL stop to dram.

    stop IPL output to dram.

    @return ER IPL status.
*/
//#NT#2009/10/29#Bowen Li -begin
//#NT#rearrange judgement flow
//#NT#2009/09/29#Bowen Li -begin
//#NT#add new function for change clock
ER IPL_Stop2Dram(void)
{
    IPL_MODE CurMode, NextMode;

    CurMode = IPL_GetMode();
    NextMode = IPL_GetNextMode();
    debug_ind(("IPL_Stop2Dram %d %d\r\n", CurMode, NextMode));
    if (((CurMode == IPL_MODE_PREVIEW) && (NextMode == IPL_MODE_CAP)) ||
        ((CurMode == IPL_MODE_PREVIEW) && (NextMode == IPL_MODE_PCC)) ||
        ((CurMode == IPL_MODE_VIDEO) && (NextMode == IPL_MODE_PCC)) ||
        ((CurMode == IPL_MODE_VIDEO) && (NextMode == IPL_MODE_VIDEOREC)) ||
        ((CurMode == IPL_MODE_PCC) && (NextMode == IPL_MODE_PREVIEW)) ||
        ((CurMode == IPL_MODE_PCC) && (NextMode == IPL_MODE_VIDEO)) ||
        ((CurMode == IPL_MODE_VIDEOREC) && (NextMode == IPL_MODE_VIDEO)))
    {
        StopPreviewDramData(NULL);
    }
    return E_OK;
}
//#NT#2009/09/29#Bowen Li -end
//#NT#2009/10/29#Bowen Li -end

/**
    IPL change off mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeOffTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    ISR_CB Isr_cb = {NULL};
    ER er_code;
    pimginfo = UI_GetParameter();

    switch(Obj->Mode)
    {
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
            //config sensor
            Sensor_WakeUp(0);
            Sensor_ChgMode(SENSOR_MODE_FULL);
            //hook ipp isrcb func
            Isr_cb.SIEISR_CB = NT99140_sie_isrCB;
            Isr_cb.IPEISR_CB = NT99140_ipe_isrCB;
            Isr_cb.IMEISR_CB = NT99140_ime_isrCB;
            Isr_cb.IFEISR_CB = NULL;
            IsrCBReg(&Isr_cb);
            //config Image pipeline
            ippctrl_obj.opmode = IPP_OPEN;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.fpIppFlowApi = Set2Preview;
            ippctrl_obj.fpStrongFlow = NULL;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutYoft1/2;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = DISABLE;
            ippctrl_obj.IPPctrl_info.uiImgFmt = Obj->uiImgFmt;
            ippctrl_obj.opmode = IPP_START;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);

            return er_code;
            //#NT#2011/03/28#Ethan Lau -begin
            //#NT#fpga test
        case IPL_MODE_PCC:
            //config sensor
            Sensor_WakeUp(0);
            Sensor_ChgMode(SENSOR_MODE_FULL);

            IsrCBReg(NULL);

            //config Image pipeline
            ippctrl_obj.opmode = IPP_OPEN;
            IPPCTRL_SetCmd(ippctrl_obj);
            ippctrl_obj.fpIppFlowApi = Set2Preview;
            ippctrl_obj.fpStrongFlow = IPL_IPPctrlflow2;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutYoft1/2;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = DISABLE;
            ippctrl_obj.IPPctrl_info.uiImgFmt = Obj->uiImgFmt;
            ippctrl_obj.opmode = IPP_START;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            return er_code;
            //#NT#2011/03/28#Ethan Lau -end
        case IPL_MODE_OFF:
            return E_OK;
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_IDLE:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change idle mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeIdleTo(IPL_OBJ *Obj)
{
    //IPPCTRL_OBJ ippctrl_obj;
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            return E_OK;
        case IPL_MODE_IDLE:
            return E_OK;

        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
        case IPL_MODE_PCC:
            //wake up sensor
            Sensor_WakeUp(0);
            //start ipp
            /*
            //config Image pipeline
            ippctrl_obj.opmode = IPP_OPEN;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.fpStrongFlow = NULL;
            ippctrl_obj.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.uiPath1OutLineoffset = Obj->uiImeOutWidth1;
            ippctrl_obj.ImePath2Enable = DISABLE;
            ippctrl_obj.opmode = IPP_START;
            IPPCTRL_SetCmd(ippctrl_obj);
            */
            return E_OK;
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change preview mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModePrvTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    IME_OUTFMT uiCapFmt = IMEOUT2_FMT420_2;
    ER er_code;
    pimginfo = UI_GetParameter();
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);

            //config sensor into sleep/pause mode
            Sensor_Sleep(0);
            return E_OK;

        case IPL_MODE_CAP:
            //#NT#20090929#Bowen Li -begin
            //#NT#add new function for change clock
            if(g_fpAlgExport)
            {
                g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
            }
            //#NT#20090929#Bowen Li -end
            ippctrl_obj.opmode = IPP_CAP_STOP;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieCapBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutUVoft1;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = ENABLE;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeH = Obj->uiImeOutWidth2;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeV = Obj->uiImeOutHeight2;
            ippctrl_obj.IPPctrl_info.uiPath2OutLineoffset = Obj->uiImeOutYoft2;
            ippctrl_obj.IPPctrl_info.uiPath2OutCCLineoffset = Obj->uiImeOutUVoft2;
            ippctrl_obj.IPPctrl_info.uiImgFmt = uiCapFmt;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            return er_code;
        case IPL_MODE_IDLE:
            //ex
            //IPP stop & close
            /*
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);
            */
            //stop/sleep sensor
            Sensor_Sleep(0);
            return E_OK;
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
            //#NT#2011/09/05#Ethan Lau -begin
            //#NT# inorder to prevent proj. layer close and then open phototsk
            Set_SenInfo(IPPImg_Preview);
            SenFlowFunc(AAFD_Switch);
            //#NT#2011/09/05#Ethan Lau -end
            return E_OK;
        case IPL_MODE_PCC:
        case IPL_MODE_VIDEOREC:
        default:
            return E_PAR;
    }
}

/**
    IPL change video mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeVidTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    DCOUT_OBJ jpgobj = {0};
    DcoutSize jpgDcoutsize;//2011/05/13 Meg
    ISR_CB Isr_cb = {NULL};
    UINT32 uiPath1H,uiPath1V,uiPath1Yoft,uiPath1CCoft;
    ER er_code;
    UINT32 uiJpgStatus=0;
    pimginfo = UI_GetParameter();
    jpgobj.pDcoutSize = (DcoutSize *)&jpgDcoutsize;//2011/05/13 Meg
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);
            Sensor_Sleep(0);
            return E_OK;
        case IPL_MODE_VIDEOREC:
            //enable jpg eng when video rec.
            //referenced by ippbuffer when ipp buf initialize
            SetIPPFuncInfor(JPG_EN, ENABLE);

            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);
            //config sensor
            if (Obj->uiImeOutWidth2 == 1920 && Obj->uiImeOutHeight2 == 1080)
                Sensor_ChgMode(SENSOR_MODE_1920X1080);
            else
                Sensor_ChgMode(SENSOR_MODE_FULL);

            IPL_EngMode(Obj->uiImeOutWidth2,Obj->uiImeOutHeight2,Obj->uiImeOutWidth1,Obj->uiImeOutHeight1);

            //jpg bit resolution
            jpgobj.uiInSizeH = Obj->uiImeOutWidth2;
            jpgobj.uiInSizeV = Obj->uiImeOutHeight2;
            switch(Obj->uiImgFmt)
            {
                case IMEOUT2_FMT444:
                    jpgobj.Fmt = JPGFormatYUV444;
                    break;
                case IMEOUT2_FMT422_1:
                case IMEOUT2_FMT422_2:
                    jpgobj.Fmt = JPGFormatYUV422;
                    break;
                case IMEOUT2_FMT420_1:
                case IMEOUT2_FMT420_2:
                    jpgobj.Fmt = JPGFormatYUV420;
                    break;
                default:
                    jpgobj.Fmt = JPGFormatYUV420;
                    break;
            }
            if(GetIPPFuncInfor(JPG_DCOUT_EN))
            {
                //config jpg dcoutinfo
                jpgobj.Op = DCOUT_OP_SIZEINFO;
                jpgobj.uiPanelSizeH = Obj->uiImeOutWidth1;
                jpgobj.uiPanelSizeV = Obj->uiImeOutHeight1;
                Dcout_Setcmd(&jpgobj);

                uiPath1H = jpgobj.pDcoutSize->uiSizeH;//dcout size
                uiPath1V = jpgobj.pDcoutSize->uiSizeV;//dcout size
                uiPath1Yoft = jpgobj.pDcoutSize->uiLineoffsetY;//dcout size
                uiPath1CCoft = jpgobj.pDcoutSize->uiLineoffsetUV;
            }
            else
            {
                //simulated path1 buf do not need
                uiPath1H = Obj->uiImeOutWidth1;
                uiPath1V = Obj->uiImeOutHeight1;
                uiPath1Yoft = Obj->uiImeOutYoft1;
                uiPath1CCoft = Obj->uiImeOutYoft1>>1;
            }

            //config jpg triggeridx
            jpgobj.Op = DCOUT_OP_TRIGGERIDX;
            jpgobj.TriggerIdx = 3;
            Dcout_Setcmd(&jpgobj);
            //config Image pipeline
            ippctrl_obj.fpIppFlowApi = Set2Video;
            if(GetIPPFuncInfor(JPG_DCOUT_EN))
                ippctrl_obj.fpStrongFlow = IPL_IPPctrlflow;
            else
                ippctrl_obj.fpStrongFlow = IPL_IPPctrlflow2;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = uiPath1H;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = uiPath1V;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = uiPath1Yoft;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = uiPath1CCoft;

            ippctrl_obj.IPPctrl_info.ImePath2Enable = ENABLE;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeH = Obj->uiImeOutWidth2;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeV = Obj->uiImeOutHeight2;
            ippctrl_obj.IPPctrl_info.uiPath2OutLineoffset = Obj->uiImeOutYoft2;
            ippctrl_obj.IPPctrl_info.uiPath2OutCCLineoffset = Obj->uiImeOutYoft2/2;
            ippctrl_obj.IPPctrl_info.uiImgFmt = Obj->uiImgFmt;
            ippctrl_obj.opmode = IPP_START;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            if(er_code != E_OK)
                return er_code;
            //config jpg start
            switch(IPPCTRL_GetCurFlow())
            {
                case IPP_SliceMode_S2S:
                //case IPP_SliceMode_F2S:
                    jpgobj.Op = DCOUT_OP_SOPEN;
                    break;
                default:
                    jpgobj.Op = DCOUT_OP_FOPEN;
                    break;
            }
            switch(IME_D2_Para.out_format)
            {
                case IME_OUTPUT_YCC_420_COS:
                    jpgobj.Fmt = JPGFormatYUV420;
                    break;
                default:
                    debug_err(("%d %s\r\n",__LINE__,__func__));
                case IME_OUTPUT_YCC_422_COS:
                    jpgobj.Fmt = JPGFormatYUV422;
                    break;
            }
            Dcout_Setcmd(&jpgobj);
            //hook ipp isrcb func
            //ipp proj. dep.
            Isr_cb.SIEISR_CB = NT99140_sie_isrCB;
            Isr_cb.IPEISR_CB = NT99140_ipe_isrCB;
            Isr_cb.IMEISR_CB = jpgobj.fpImeIsrCB;//must be when use jpg encode dcout
            IsrCBReg(&Isr_cb);
            if(GetIPPFuncInfor(JPG_DCOUT_EN))//ethan_20110725_fixVidFrameModCrash
            {
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
                        if(g_fpAlgExport)
                            g_fpAlgExport(ALGMSG_PREVIEWSTABLE, &gAlgMsg);
                        break;
                    }
                }
            }
            return E_OK;
        case IPL_MODE_IDLE:
            //ex
            //IPP stop & close
            /*
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);
            */
            //stop/sleep sensor
            /*
            */
            return E_OK;

        case IPL_MODE_VIDEO:
        case IPL_MODE_PREVIEW:
            //#NT#2011/09/05#Ethan Lau -begin
            //#NT# inorder to prevent proj. layer close and then open phototsk
            Set_SenInfo(IPPImg_Preview);
            SenFlowFunc(AAFD_Switch);
            //#NT#2011/09/05#Ethan Lau -end
            return E_OK;

        case IPL_MODE_PCC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}
/**
    IPL change video recording mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeRecTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    DCOUT_OBJ jpgobj;
    ISR_CB Isr_cb = {NULL};
    ER er_code;
    pimginfo = UI_GetParameter();
    switch(Obj->Mode)
    {
        case IPL_MODE_VIDEO:
            SetIPPFuncInfor(JPG_EN, DISABLE);
            //stop pipeline
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);
            //hook ipp isrcb func
            Isr_cb.SIEISR_CB = NT99140_sie_isrCB;
            Isr_cb.IPEISR_CB = NT99140_ipe_isrCB;
            Isr_cb.IMEISR_CB = NT99140_ime_isrCB;
            Isr_cb.IFEISR_CB = NULL;
            IsrCBReg(&Isr_cb);
            //close jpg
            jpgobj.Op = DCOUT_OP_CLOSE;
            Dcout_Setcmd(&jpgobj);
            //config sensor
            Sensor_ChgMode(SENSOR_MODE_FULL);
            //config Image pipeline
            ippctrl_obj.fpIppFlowApi = Set2Preview;
            ippctrl_obj.fpStrongFlow = NULL;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutYoft1/2;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = DISABLE;
            ippctrl_obj.IPPctrl_info.uiImgFmt = Obj->uiImgFmt;
            ippctrl_obj.opmode = IPP_START;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            return er_code;

        case IPL_MODE_OFF:
        case IPL_MODE_IDLE:
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEOREC:
        case IPL_MODE_PCC:
        case IPL_MODE_CAP:
        default:
            return E_PAR;
    }
}

/**
    IPL change pcc mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModePccTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    ER er_code;
    pimginfo = UI_GetParameter();
    switch(Obj->Mode)
    {
        case IPL_MODE_OFF:
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);

            //config sensor into sleep/pause mode
            Sensor_Sleep(0);
            return E_OK;

        case IPL_MODE_IDLE:
            //ex
            //IPP stop & close
            /*
            ippctrl_obj.opmode = IPP_STOP;
            IPPCTRL_SetCmd(ippctrl_obj);

            ippctrl_obj.opmode = IPP_CLOSE;
            IPPCTRL_SetCmd(ippctrl_obj);
            */
            //stop/sleep sensor
            /*
            */
            return E_OK;

        case IPL_MODE_PCC:
            //#NT#2011/09/05#Ethan Lau -begin
            //#NT# inorder to prevent proj. layer close and then open phototsk
            Set_SenInfo(IPPImg_Preview);
            SenFlowFunc(AAFD_Switch);
            //#NT#2011/09/05#Ethan Lau -end
            return E_OK;
        case IPL_MODE_CAP:
            //#NT#20090929#Bowen Li -begin
            //#NT#add new function for change clock
            if(g_fpAlgExport)
            {
                g_fpAlgExport(ALGMSG_FIRSTRAW, &gAlgMsg);
            }
            //#NT#20090929#Bowen Li -end
            ippctrl_obj.opmode = IPP_CAP_STOP;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieCapBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutUVoft1;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = ENABLE;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeH = Obj->uiImeOutWidth2;
            ippctrl_obj.IPPctrl_info.uiPath2OutsizeV = Obj->uiImeOutHeight2;
            ippctrl_obj.IPPctrl_info.uiPath2OutLineoffset = Obj->uiImeOutYoft2;
            ippctrl_obj.IPPctrl_info.uiPath2OutCCLineoffset = Obj->uiImeOutUVoft2;
            ippctrl_obj.IPPctrl_info.uiImgFmt = IMEOUT2_FMT422_2;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            return er_code;
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
        case IPL_MODE_VIDEOREC:
        default:
            return E_PAR;
    }
}

/**
    IPL change capture mode to other mode.

    @param Obj IPL paramters, including command, mode, path1&2 size, and digital zoom step.

    @return ER IPL change mode status.
*/
ER IPL_ChgModeCapTo(IPL_OBJ *Obj)
{
    IPPCTRL_OBJ ippctrl_obj = {0};
    PImageAlgInfo pimginfo;
    ER er_code;
    pimginfo = UI_GetParameter();
    switch(Obj->Mode)
    {
        case IPL_MODE_PREVIEW:
        case IPL_MODE_PCC:
            /**
            //following operation is in sensor_cap2prv
            //config sensor
            //don't need,sensor config op. move 2 sensor_cap2prv
            */
            //config Image pipeline
            ippctrl_obj.fpIppFlowApi = Set2Preview;
            ippctrl_obj.fpStrongFlow = NULL;
            ippctrl_obj.IPPctrl_info.uiDzoomStep = Obj->uiDzoomStep;
            ippctrl_obj.IPPctrl_info.uiSieBitDepth = pimginfo->SieBitDepth;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeH = Obj->uiImeOutWidth1;
            ippctrl_obj.IPPctrl_info.uiPath1OutsizeV = Obj->uiImeOutHeight1;
            ippctrl_obj.IPPctrl_info.uiPath1OutLineoffset = Obj->uiImeOutYoft1;
            ippctrl_obj.IPPctrl_info.uiPath1OutCCLineoffset = Obj->uiImeOutYoft1/2;
            ippctrl_obj.IPPctrl_info.ImePath2Enable = DISABLE;
            ippctrl_obj.IPPctrl_info.uiImgFmt = Obj->uiImgFmt;
            ippctrl_obj.opmode = IPP_START;
            er_code = IPPCTRL_SetCmd(ippctrl_obj);
            return er_code;

        case IPL_MODE_CAP:
            return E_OK;

        case IPL_MODE_OFF:
        case IPL_MODE_IDLE:
        case IPL_MODE_VIDEO:
        case IPL_MODE_VIDEOREC:
        default:
            return E_PAR;
    }
}
#endif
