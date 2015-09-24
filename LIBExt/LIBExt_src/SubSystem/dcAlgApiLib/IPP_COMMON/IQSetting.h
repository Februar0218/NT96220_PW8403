#ifndef _IQSETTING_H
#define _IQSETTING_H
typedef struct
{
    STR_IPE_NOVANR_PARA *NRParamSel;
    UINT8 *FStabSel;
    UINT8 *FDtabSel;
    STR_IPE_EDGE_ENHMAP_PARA *EdgeParamSel;
    INT16 *EdgeKerDivSel;
    UINT8 *EstabSel;
} IPE_ISO_PARAM, *PIPE_ISO_PARAM;

//#NT#2011/08/22#Bowen Li -begin
//#NT#add image filter
typedef struct
{
    IPE_YCFIX_EFFECT_PARAM *YCFIX;
    IPE_EDGE_EFFECT_PARAM *EDGE;
    IPE_COLOR_EFFECT_PARAM *COLOR;
    IPE_INVERSE_EFFECT_PARAM *INVERSE;
    STR_COLOR_INFOR *COLOR_INFOR;
    UINT32 *GNoise;
    STR_IPE_COLOR_PARA_EXT *IPE_ColorExt;
    UINT32 *Gamma;
    STR_IPE_EDGE_ENHMAP_PARA *EdgeMapParam;
    UINT32 FuncEn;
} IMAGE_FILTER;
//#NT#2011/08/22#Bowen Li -end
#endif // EOF
