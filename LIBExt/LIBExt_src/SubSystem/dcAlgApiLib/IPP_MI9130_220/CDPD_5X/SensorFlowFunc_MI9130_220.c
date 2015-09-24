#include "type.h"
#include "ae_api.h"
#include "awb_api.h"
#include "flashlight_api.h"
#include "ErrorNo.h"
#include "Pll.h"
#if MI9130_220
#include "IPP_MI9130_220_int.h"
#include "IQSetting_MI9130_220.h"
//Local Define
#define NORMAL_CC       0
#define INDENITY_CC     1
#define IPE_CC_TYPE     NORMAL_CC
//Local Var.
IME_IQ_FLOW_INFO Ime_IQ = {0};
STR_IPE_EDGE_ENHMAP_PARA IQ_IPE_EdgeParam;
IPE_COLOR_EFFECT_PARAM IQ_ColorEffectTable;
SenCBInfo gSenCBInfo = {0, &IPPMgrReq, &SIE_PRE_Para_cap, &SIE_PRE_Para_prv, &IPEModeInfo,&IMEParam,&Ife_Param, IPL_SLEEP_LEVEL_0};
static UINT32 m_uiISO = 0,m_uiEXPOT = 0,m_uiISOIDX = ISONUM0;

//#NT#2011/11/17#Ethan Lau -begin
//#NT#cause 220 ife have no rounding,always have truncation error.
#define IfeCCOft(iso)    (IFE_Func[iso][IFE_YCC]==ENABLE ? IFE_param_ycc[iso][0].repeat:0)
//#NT#2011/11/17#Ethan Lau -end

void SetAEInfo(UINT32 ISO,UINT32 ExpoT)
{
    m_uiISO = ISO;
    m_uiEXPOT = ExpoT;
    if(m_uiISO <= 60)
        m_uiISOIDX = ISONUM0;
    else if(m_uiISO < 100)
        m_uiISOIDX = ISONUM1;
    else if(m_uiISO < 200)
        m_uiISOIDX = ISONUM2;
    else if(m_uiISO < 400)
        m_uiISOIDX = ISONUM3;
}

static UINT32 GetISOIdx(void)
{
    return m_uiISOIDX;
}

ER ipe_IQparam(UINT32 ParamMode, IPE_IQINFO *IQInfo)
{
    UINT32 ISO_Value = GetISOIdx();
    //#NT#2011/11/17#Ethan Lau -begin
    //#NT#cause 220 ife have no rounding,always have truncation error
    memcpy(&IQ_ColorEffectTable, &(ColorEffectTable[gImageAlgInfo.ImageEffect]), sizeof(IPE_COLOR_EFFECT_PARAM));
    IQ_ColorEffectTable.CbOfs += IfeCCOft(ISO_Value);
    IQ_ColorEffectTable.CrOfs += IfeCCOft(ISO_Value);
    //#NT#2011/11/17#Ethan Lau -end
    switch(ParamMode)
    {
        case IPE_PRV_SETTING :
        case IPE_CAP_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;

            IQInfo->FuncSel =   IPE_NR_EN |
                                IPE_RLPF_EN |
                                IPE_GLPF_EN |
                                IPE_BLPF_EN |
                                IPE_CCR_EN |
                                IPE_CTRL_EN |
                                IPE_HADJ_EN |
                                IPE_CADJ_EN |
                                IPE_CADJ_YENH_EN |
                                IPE_CADJ_YCON_EN |
                                IPE_CADJ_CCON_EN |
                                IPE_CADJ_CLPF_EN;

            IQInfo->ParamUpdateSel =  IPE_SET_NR |
                                      IPE_SET_EDGE |
                                      IPE_SET_COLOR_GAIN |
                                      IPE_SET_COLOR |
                                      IPE_SET_COLOREFFECT
                                      ;

            memcpy(&IQ_IPE_EdgeParam, IPE_ISOParam[ISO_Value].EdgeParamSel, sizeof(STR_IPE_EDGE_ENHMAP_PARA));

            IQInfo->EDGE_PARAM = &IQ_IPE_EdgeParam;
            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;
            IQInfo->EdgeKerDiv = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;
            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &IQ_ColorEffectTable;//&ColorEffectTable[gImageAlgInfo.ImageEffect];

            IQInfo->HueTab = IPE_hueTab;
            IQInfo->SatTab = IPE_satTab;
            IQInfo->IntTab = IPE_intTab;
            IQInfo->DDSTab = IPE_ddsTab;

            IQInfo->COLOR_PARAM.ColorGain.ChR = 256;
            IQInfo->COLOR_PARAM.ColorGain.ChGr = 256;
            IQInfo->COLOR_PARAM.ColorGain.ChGb = 256;
            IQInfo->COLOR_PARAM.ColorGain.ChB = 256;

            #if (IPE_CC_TYPE == INDENITY_CC)
            IQInfo->COLOR_PARAM.ColorCorrect=Indenity_CC;
            #else
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            #endif
            IQInfo->COLOR_PARAM.ColorSupWeight=22;

            IQInfo->EDGE_PARAM->EdgEnhanceP = (IQInfo->EDGE_PARAM->EdgEnhanceP * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
            IQInfo->EDGE_PARAM->EdgEnhanceN = (IQInfo->EDGE_PARAM->EdgEnhanceN * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
            IQInfo->COLOR_EFFECT_PARAM->SatOfs+= Saturation[ISO_Value][gImageAlgInfo.Saturation];

#if 0
            //Image Effect Capture IPE parameter
            if ( IE_GetCapStatus() == TRUE )
            {
                switch ( IE_GetCapIndex() )
                {
                    case IE_INDEX_ARF_PopArt:
                    {
                        IQInfo->FuncSel |= (IPE_CADJ_EN | IPE_CADJ_CCON_EN);
                        IQInfo->COLOR_EFFECT_PARAM = &IPE_ColorEffect_PopArt[ISO_Value];
                        IQInfo->HueTab = NULL;
                        IQInfo->SatTab = NULL;
                        IQInfo->IntTab = NULL;
                        IQInfo->DDSTab = NULL;
                    }
                    break;

                    case IE_INDEX_ARF_Sketch:
                    {
                        //IQInfo->FuncSel |= (IPE_CADJ_EN | IPE_YCTH_EN | IPE_CADJ_YENH_EN);

                        //IQInfo->YCFIX_EFFECT_PARAM = &IPE_YCFixEffect_Sketch[ISO_Value];
                        //IQInfo->EDGE_EFFECT_PARAM = &IPE_EdgeEffect_Sketch[ISO_Value];
                        //IQInfo->EDGE_PARAM = &IPE_EdgeParam_Sketch[ISO_Value];
                    }
                    break;

                    case IE_INDEX_ARF_ToyPhoto:
                    case IE_INDEX_ARF_FishEye:
                    case IE_INDEX_ARF_FantasticFocus:
                    case IE_INDEX_ARF_RoughMonoChorme:
                    break;
                }
            }
#endif
        break;
        case IPE_PRV_AE_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;
            //IQInfo->ParamUpdateSel = IPE_SET_NR | IPE_SET_EDGE | IPE_SET_COLOR;
            IQInfo->ParamUpdateSel = IPE_SET_NR | IPE_SET_EDGE | IPE_SET_COLOR|IPE_SET_COLOREFFECT;

            memcpy(&IQ_IPE_EdgeParam, IPE_ISOParam[ISO_Value].EdgeParamSel, sizeof(STR_IPE_EDGE_ENHMAP_PARA));

            IQInfo->EDGE_PARAM = &IQ_IPE_EdgeParam;
            IQInfo->NR_PARAM = IPE_ISOParam[ISO_Value].NRParamSel;

            //IQInfo->EDGE_PARAM = IPE_ISOParam[ISO_Value].EdgeParamSel;
            IQInfo->EdgeKerDiv = IPE_ISOParam[ISO_Value].EdgeKerDivSel;
            IQInfo->Estab = IPE_ISOParam[ISO_Value].EstabSel;
            IQInfo->FStab = IPE_ISOParam[ISO_Value].FStabSel;
            IQInfo->FDtab = IPE_ISOParam[ISO_Value].FDtabSel;

            IQInfo->COLOR_EFFECT_PARAM = &IQ_ColorEffectTable; //&ColorEffectTable[gImageAlgInfo.ImageEffect];
            #if (IPE_CC_TYPE == INDENITY_CC)
            IQInfo->COLOR_PARAM.ColorCorrect=Indenity_CC;
            #else
            IQInfo->COLOR_PARAM.ColorCorrect=Normal_CC;
            #endif
            IQInfo->COLOR_PARAM.ColorSupWeight=22;

            IQInfo->EDGE_PARAM->EdgEnhanceP = (IQInfo->EDGE_PARAM->EdgEnhanceP * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
            IQInfo->EDGE_PARAM->EdgEnhanceN = (IQInfo->EDGE_PARAM->EdgEnhanceN * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
            IQInfo->COLOR_EFFECT_PARAM->SatOfs+= Saturation[ISO_Value][gImageAlgInfo.Saturation];

        break;
        case IPE_PRV_AF_SETTING :
            IQInfo->FuncUpdateSel = IPE_FUNC_SET;
            IQInfo->FuncSel |= IPE_VACC_EN;

            IQInfo->ParamUpdateSel = IPE_SET_EDGE | IPE_SET_VA;

            IQInfo->EdgeKerDiv = IPE_AFedgeKernelDiv;
        break;
        case IPE_COLOREFFECT:
            if ( Is_AELock() == TRUE )
            {
                IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;
                IQInfo->ParamUpdateSel = IPE_SET_COLOREFFECT;
                //#NT#2011/09/08#Ethan Lau -begin
                //#NT#avoid to re-config  ColorEffectTable
                IQInfo->COLOR_EFFECT_PARAM = &IQ_ColorEffectTable;
                //#NT#2011/09/08#Ethan Lau -end
                IQInfo->HueTab = NULL;
                IQInfo->SatTab = NULL;
                IQInfo->IntTab = NULL;
                IQInfo->DDSTab = NULL;
            }
            break;
        case IPE_SHARPNESS:
            if ( Is_AELock() == TRUE )
            {
                IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;
                IQInfo->ParamUpdateSel = IPE_SET_EDGE;
                //#NT#2011/09/08#Ethan Lau -begin
                //#NT#avoid to config dram addr : 0x0
                memcpy(&IQ_IPE_EdgeParam, IPE_ISOParam[ISO_Value].EdgeParamSel, sizeof(STR_IPE_EDGE_ENHMAP_PARA));
                IQInfo->EDGE_PARAM = &IQ_IPE_EdgeParam;
                //#NT#2011/09/08#Ethan Lau -end
                IQInfo->EDGE_PARAM->EdgEnhanceP = (IQInfo->EDGE_PARAM->EdgEnhanceP * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
                IQInfo->EDGE_PARAM->EdgEnhanceN = (IQInfo->EDGE_PARAM->EdgEnhanceN * Sharpness[ISO_Value][gImageAlgInfo.Sharpness])>>6;
                IQInfo->EdgeKerDiv = NULL;
                IQInfo->Estab = NULL;
            }
            break;
        case IPE_SATURATION:
            if ( Is_AELock() == TRUE )
            {
                IQInfo->FuncUpdateSel = IPE_FUNC_NOUPDATE;
                IQInfo->ParamUpdateSel = IPE_SET_COLOREFFECT;
                //#NT#2011/09/08#Ethan Lau -begin
                //#NT#avoid to re-config the satofs configed by ColorEffectTable
                IQInfo->COLOR_EFFECT_PARAM = &IQ_ColorEffectTable;
                IQInfo->COLOR_EFFECT_PARAM->SatOfs += Saturation[ISO_Value][gImageAlgInfo.Saturation];
                //#NT#2011/09/08#Ethan Lau -end
                IQInfo->HueTab = NULL;
                IQInfo->SatTab = NULL;
                IQInfo->IntTab = NULL;
                IQInfo->DDSTab = NULL;
            }
            break;
        default :
            debug_err(("ipe_IQparam() param mode(%d) selection error!\r\n",ParamMode));
            return E_SYS;
        break;
    }
    return E_OK;
}

ER ime_IQparam(UINT32 ParamMode, void* _ptr)
{
    pIME_MODE_PRAM _pime_param = {0};
    IME_CST_INFO* _cst = {0};
    IME_CHROMA_FILTER_INFO* _chromaF = {0};
    IME_CURVE_MAPPING_INFO* _curveM = {0};
    IME_FILM_GRAIN_INFO* _filmG = {0};

    switch(ParamMode)
    {
        case IME_IQ_Init:
            _pime_param = (pIME_MODE_PRAM)_ptr;
            Ime_IQ.CurveMappingInfo.uCrvMapLUT = NULL;
            _pime_param->imeIqInfo = &Ime_IQ;
            //debug_msg("IMEParam.imeIqInfo : 0x%x\r\n",IMEParam.imeIqInfo);
            break;
        case IME_PRV_ChromaF:
            _chromaF = (IME_CHROMA_FILTER_INFO*)_ptr;
            _chromaF->HavgEN = IME_HAVG_DISABLE;
            _chromaF->HavgSPLEN = IME_HAVG_SPL_DISABLE;
            _chromaF->HavgCoef = 0;
            _chromaF->HavgTh = 0;
            _chromaF->HavgSPLTap = 0;
            _chromaF->HavgPWSel = IME_AVG_PXLWIDTH_2;
            ime_chgParam((void*)_chromaF,IME_CHG_CHROMA_FILTER);
            break;
        case IME_PRV_CurveM:
            _curveM = (IME_CURVE_MAPPING_INFO*)_ptr;
            _curveM->CrvEn = DISABLE;
            _curveM->CenterCb = 0x80;
            _curveM->CenterCr = 0x80;
            _curveM->StepSel = IME_CRV_STEP_1;
            _curveM->uCrvMapLUT = NULL;
            ime_chgParam((void*)_curveM,IME_CHG_CURVE_MAPPING);
            break;
        case IME_PRV_CST:
            _cst= (IME_CST_INFO*)_ptr;
            _cst->cstEN = IME_CST_DISABLE;
            _cst->cstSel = IME_CST_RGB2YCC;
            ime_chgParam((void*)_cst,IME_CHG_CST);
            break;
        case IME_PRV_FilmG:
            _filmG = (IME_FILM_GRAIN_INFO*)_ptr;
            _filmG->fgEn = DISABLE;
            _filmG->fgVal = 0;
            _filmG->fgW0 = 0;
            _filmG->fgW1 = 0;
            ime_chgParam((void*)_filmG,IME_CHG_FILM_GRAIN);
            break;
        case IME_PRV_ScaleF:
            break;
        case IME_CAP_ChromaF:
            _chromaF = (IME_CHROMA_FILTER_INFO*)_ptr;
            _chromaF->HavgEN = IME_HAVG_DISABLE;
            _chromaF->HavgSPLEN = IME_HAVG_SPL_DISABLE;
            _chromaF->HavgCoef = 0;
            _chromaF->HavgTh = 0;
            _chromaF->HavgSPLTap = 0;
            _chromaF->HavgPWSel = IME_AVG_PXLWIDTH_2;
            ime_chgParam((void*)_chromaF,IME_CHG_CHROMA_FILTER);
            break;
        case IME_CAP_CurveM:
            _curveM = (IME_CURVE_MAPPING_INFO*)_ptr;
            _curveM->CrvEn = DISABLE;
            _curveM->CenterCb = 0x80;
            _curveM->CenterCr = 0x80;
            _curveM->StepSel = IME_CRV_STEP_1;
            _curveM->uCrvMapLUT = NULL;
            ime_chgParam((void*)_curveM,IME_CHG_CURVE_MAPPING);
            break;
        case IME_CAP_CST:
            _cst= (IME_CST_INFO*)_ptr;
            _cst->cstEN = IME_CST_DISABLE;
            _cst->cstSel = IME_CST_RGB2YCC;
            ime_chgParam((void*)_cst,IME_CHG_CST);
            break;
        case IME_CAP_FilmG:
            _filmG = (IME_FILM_GRAIN_INFO*)_ptr;
            _filmG->fgEn = DISABLE;
            _filmG->fgVal = 0;
            _filmG->fgW0 = 0;
            _filmG->fgW1 = 0;
            ime_chgParam((void*)_filmG,IME_CHG_FILM_GRAIN);
            break;
        case IME_CAP_ScaleF:
            break;
        default :
            debug_err(("ime iq setting out of range!,ime msg id = %d\r\n",ParamMode));
        break;
    }

    return E_OK;
}

ER ife_IQparam(UINT32 ParamMode, pIFE_ENG_SETTING pIfe)
{
    UINT32 ISO_Value = GetISOIdx();
    switch(ParamMode)
    {
        case IFE_PRV_AE_SETTING:
            pIfe->IFE_filter = IFE_param_embeded[ISO_Value];
            break;
        case IFE_PRV_SETTING:
        case IFE_VIDREC_SETTING:
        case IFE_CAP_SETTING:
            pIfe->IFE_filter = IFE_param_embeded[ISO_Value];
            break;
        default:
            debug_err(("ife_IQparam: mode error(%d)\r\n",ParamMode));
            return E_PAR;
            break;
    }
    return E_OK;
}

void ife_ChgParam(pIFE_ENG_SETTING pIfe_Param, JobTypeEnum JobType)
{
    IPPMgrReq.pIfeParam = pIfe_Param;
    if ( IFE_AllocatResourceIPP(&IPPMgrReq, JobType) != IPP_OK)
        debug_err(("IFE_AllocatResourceIPP() error \r\n"));
    if ( IFE_ExecIPP(&IPPMgrReq, JobType) != IPP_OK)
        debug_err(("IFE_ExecIPP() error \r\n"));
}

UINT32 Get_IfeInfo(IFE_MODE_SEL sel, UINT32 idx)
{
    switch(sel)
    {
        case IFE_RAW:
            return (UINT32)IFE_param_raw[idx];

        case IFE_YCC:
            return (UINT32)IFE_param_ycc[idx];

        case IFE_Y:
            return (UINT32)IFE_param_y[idx];

        default:
            return (UINT32)IFE_param_ycc[idx];
    }
}

//#NT#2009/10/08#Bowen Li -begin
//#NT#add IPL sleep & wake up command
void Sen_EnterSleepMode(IPL_SLEEP_LEVEL Level)
{
#if 0
    StopPreviewDramData(NULL);
    sie_close();
    Sensor_Sleep(0);
    ipe_close();
    ime_close();
#endif
}

void Sen_LeaveSleepMode(IPL_SLEEP_LEVEL Level)
{
#if 0
    //IPL_OBJ IplObj;
    SIE_OPENOBJ SIEOpenObj;
    IPE_OPENOBJ IPEOpenObj;
    IME_OPENOBJ IMEOpenObj;

    SIEOpenObj.FP_SIEISR_CB = sie_isrCB;


    IPEOpenObj.FP_IPEISR_CB = ipe_isrCB;
    IPEOpenObj.IPE_CLOCKSEL = PLL_CLKSEL_IPE_80;
    IMEOpenObj.FP_IMEISR_CB = ime_isrCB;
    IMEOpenObj.IME_CLOCKSEL = PLL_CLKSEL_IME_120;
    Sensor_WakeUp(0);
    sie_open(&SIEOpenObj);

    ipe_open(&IPEOpenObj);
    ime_open(&IMEOpenObj);

    //#NT#2010/04/15#Connie Yu -begin
    //#NT#Not in use
    /*
    if(gImageAlgInfo.DISMode == _DISMode_On)
    {
        dis_updateDisplaySize();
    }
    */
    //#NT#2010/04/15#Connie Yu -end
    //IplObj.uiCmd = NULL;
    //Set2Preview(&IplObj);
#endif
}
//#NT#2009/10/08#Bowen Li -end

void Set_SenInfo(UINT32 uiMode)
{
    gSenCBInfo.IPPImgMode = uiMode;
}

void SenFlowFunc(SEN_FLOW_MSG MsgID)
{
    PImageAlgInfo pimginfo;
    pimginfo = UI_GetParameter();
    switch(MsgID)
    {
        case AE_PRV_INIT:
            AE_Init();
            break;

        case AE_PRV_SETTING:
            AE_Cap2Prv();
            break;

        case AE_CAP_SETTING:
            //ethan_20110804_ToGetRightISO_EXPT
            {
                UINT32 capiso,capext,iris;
                AE_GetCapAEArg(&capiso,&capext,&iris);
                SetAEInfo(capiso,capext);
            }
            SetIPPFuncInfor(CAP_ISO,  GetISOIdx());
            break;

        case AWB_PRV_INIT:
            //aaa_InitRBgain();
            //aaa_InitAWB();
            AWB_Init();
            break;

        case AWB_PRV_SETTING:
            //AwbCap2Prv();
            break;

        case AWB_CAP_SETTING:
            //AwbPrv2Cap();
            break;

        case PREFLASH_FLOW:
            flash_PreFlashFlow();
            break;
        case IPE_CAP_SETTING:
        case IPE_PRV_SETTING:
        case IPE_PRV_AE_SETTING:
        case IPE_PRV_AF_SETTING:
        //#NT#2011/06/28#Ethan Lau -begin
        case IPE_COLOREFFECT:
        case IPE_SHARPNESS:
        case IPE_SATURATION:
        //#NT#2011/06/28#Ethan Lau -end
            ipe_IQparam(MsgID, &gSenCBInfo.IPEModeInfo->IQINFO);
            ipe_changeParam(&gSenCBInfo.IPEModeInfo->IQINFO);
            break;

        case SENSOR_INIT:
            break;

        case I2C_INIT:
            break;

        case SIE_PRV_SETTING:
            sie_PrvParameter(gSenCBInfo.IppMgrInfo);
            break;

        case SIE_CAP_SETTING:
            sie_CapParameter(gSenCBInfo.IppMgrInfo);
            break;

        case SIE_D2D_SETTING:
            sie_D2DParameter(gSenCBInfo.IppMgrInfo);
            break;

        case SIE_PRE_GET_CAP_RAW:
            siepre_GetCapRaw(gSenCBInfo.SiePreCapInfo);
            break;

        case SENSOR_MODE_SETTING:
            Sensor_ChgMode(gSenCBInfo.SiePrePrvInfo->ModeSel);
            break;

        case IPPFUNC_Switch:
            IPPFunc_OnOffSwitch();
            break;

        case AAFD_Switch:
            AAFD_LockSwitch(gSenCBInfo.IPPImgMode);
            break;

        case IFE_FUNC_SETTING:
            SetIPPFuncInfor(IFE_FUNC_TABLE_ADDR, (UINT32)IFE_Func[GetIPPFuncInfor(CAP_ISO)]);
            break;

        case IFE_RAW_SETTING:
            SetIPPFuncInfor(IFE_RAW_TABLE_ADDR, Get_IfeInfo(IFE_RAW, GetIPPFuncInfor(CAP_ISO)));
            break;

        case IFE_YCC_SETTING:
            if (GetIPPFuncInfor(FQV_INPROCESS))
            {
                SetIPPFuncInfor(IFE_YCC_TABLE_ADDR, Get_IfeInfo(IFE_YCC, GetIPPFuncInfor(CAP_ISO)));
            }
            else if (GetIPPFuncInfor(THUMBIMG_INPROCESS))
            {
                SetIPPFuncInfor(IFE_YCC_TABLE_ADDR, Get_IfeInfo(IFE_YCC, GetIPPFuncInfor(CAP_ISO)));
            }
            else if (GetIPPFuncInfor(PRIIMG_INPROCESS))
            {
                SetIPPFuncInfor(IFE_YCC_TABLE_ADDR, Get_IfeInfo(IFE_YCC, GetIPPFuncInfor(CAP_ISO)));
            }
            break;

        //#NT#2010/07/09#Linkin Huang -begin
        //#NT# IFE for Y channel
        case IFE_Y_SETTING:
            if (GetIPPFuncInfor(FQV_INPROCESS))
            {
                SetIPPFuncInfor(IFE_Y_TABLE_ADDR, Get_IfeInfo(IFE_Y, GetIPPFuncInfor(CAP_ISO)));
            }
            else if (GetIPPFuncInfor(THUMBIMG_INPROCESS))
            {
                SetIPPFuncInfor(IFE_Y_TABLE_ADDR, Get_IfeInfo(IFE_Y, GetIPPFuncInfor(CAP_ISO)));
            }
            else if (GetIPPFuncInfor(PRIIMG_INPROCESS))
            {
                SetIPPFuncInfor(IFE_Y_TABLE_ADDR, Get_IfeInfo(IFE_Y, GetIPPFuncInfor(CAP_ISO)));
            }
            break;
        //#NT#2010/07/09#Linkin Huang -end
        case IPL_SLEEP:
            Sen_EnterSleepMode(gSenCBInfo.IplSleepLevel);
            break;
        case IPL_WAKEUP:
            Sen_LeaveSleepMode(gSenCBInfo.IplSleepLevel);
            break;
        case GET_CAP_RAW_END:
            break;
        case IME_IQ_Init:
            ime_IQparam(MsgID, (void*)gSenCBInfo.IMEModeInfo);
            break;
        case IME_PRV_SETTING:
            ime_IQparam(IME_PRV_ChromaF, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ChromaFilterInfo);
            ime_IQparam(IME_PRV_CurveM, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->CurveMappingInfo);
            ime_IQparam(IME_PRV_CST, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ColorSpaceTrans);
            ime_IQparam(IME_PRV_FilmG, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->FileGrainInfo);
            ime_IQparam(IME_PRV_ScaleF,(void*)&gSenCBInfo.IMEModeInfo->p1_out_scale_filter);
            break;
        case IME_PRV_ChromaF:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ChromaFilterInfo);
            break;
        case IME_PRV_CurveM:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->CurveMappingInfo);
            break;
        case IME_PRV_CST:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ColorSpaceTrans);
            break;
        case IME_PRV_FilmG:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->FileGrainInfo);
            break;
        case IME_PRV_ScaleF:
            ime_IQparam(IME_PRV_ScaleF,(void*)&gSenCBInfo.IMEModeInfo->p1_out_scale_filter);
            break;
        case IME_CAP_SETTING:
            ime_IQparam(IME_CAP_ChromaF, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ChromaFilterInfo);
            ime_IQparam(IME_CAP_CurveM, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->CurveMappingInfo);
            ime_IQparam(IME_CAP_CST, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ColorSpaceTrans);
            ime_IQparam(IME_CAP_FilmG, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->FileGrainInfo);
            ime_IQparam(IME_CAP_ScaleF,(void*)&gSenCBInfo.IMEModeInfo->p1_out_scale_filter);
            break;
        case IME_CAP_ChromaF:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ChromaFilterInfo);
            break;
        case IME_CAP_CurveM:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->CurveMappingInfo);
            break;
        case IME_CAP_CST:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->ColorSpaceTrans);
            break;
        case IME_CAP_FilmG:
            ime_IQparam(MsgID, (void*)&gSenCBInfo.IMEModeInfo->imeIqInfo->FileGrainInfo);
            break;
        case IME_CAP_ScaleF:
            ime_IQparam(IME_CAP_ScaleF,(void*)&gSenCBInfo.IMEModeInfo->p1_out_scale_filter);
            break;
        case IFE_PRV_AE_SETTING:
            ife_IQparam(MsgID, gSenCBInfo.IFEModeInfo);
            ife_ChgParam(gSenCBInfo.IFEModeInfo, IFE_RTCHG);
            break;
        case IFE_PRV_SETTING:
            ife_IQparam(MsgID, gSenCBInfo.IFEModeInfo);
            ife_ChgParam(gSenCBInfo.IFEModeInfo, PREVIEW_JOB);
            break;
        case IFE_VIDREC_SETTING:
            ife_IQparam(MsgID, gSenCBInfo.IFEModeInfo);
            ife_ChgParam(gSenCBInfo.IFEModeInfo, VIDEO_JOB);
            break;
        case IFE_CAP_SETTING:
            ife_IQparam(MsgID, gSenCBInfo.IFEModeInfo);
            ife_ChgParam(gSenCBInfo.IFEModeInfo, CAPTURE_JOB);
            break;
        default:
            debug_err(("-E- MsgID(%d) > MaxID(%d)\r\n", MsgID, SEN_FLOW_MSG_NUM));
            break;
    }
}
#endif
