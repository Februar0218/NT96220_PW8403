/**
    Public header file for IPP flow control

    This file is the header file that define the API and data type for IPP flow control.

    @file       IPPCtrl.h
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPPCTRL_H
#define _IPPCTRL_H

#include "Type.h"
#include "IPPBuffer.h"
/**
     IPPCtrl status

     none
     @note (Optional)
*/
typedef enum _IPPCTRL_OPMODE
{
    IPP_CLOSE       = 0,       ///< Description
    IPP_OPEN        = 1,       ///< Description
    IPP_START       = 2,       ///< Description
    IPP_STOP        = 3,       ///< stop pipe line
    IPP_CAP_STOP    = 4,       ///< pipe line from prv to cap
    IPP_MaxOpmode   = 5,       ///< Description
    ENUM_DUMMY4WORD(IPPCTRL_OPMODE)
}IPPCTRL_OPMODE;
//exten op mode
#define IPP_CHGIOSIZE_DZOOM (IPP_MaxOpmode)

/**
     IPP flows supported by IPP lib

     none
     @note (Optional)
*/
typedef enum _IPPCTRL_FLOW
{
    IPP_FrameMode_HW = 0,       ///< sie + ipeime frame mode + hw hand shake
    IPP_FrameMode_FW = 1,       ///< sie + ipeime frame mode + sw trigger
    IPP_SliceMode_S2F= 2,      ///< sie slice mode + ipeime frame mode
    IPP_SliceMode_S2S= 3,      ///< sie slice mode + ipeime slice mode
    IPP_SliceMode_F2S= 4,
    IPP_MaxFlowdefine= 5,
    ENUM_DUMMY4WORD(IPPCTRL_FLOW)
}IPPCTRL_FLOW;

/**
     Brief description.

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPCTRL_RST
{
    UINT32              SieMode;       ///< ref. enum:SIE_OutputModeEnum(sie_lib.h)
    UINT32              IpeMode;       ///< ref. enum:IPE_SripeModeEnum(ipe_lib.h)
    UINT32              ImeMode;       ///< ref. enum:_IME_MODE_SEL(ime_lib.h)
    UINT32              siebufnum;     ///< ref. enum:SIE_OUTBUFNUM(sie_lib.h)
    UINT32              imebufnum[MAXPathNum];///< ref. enum:_IME_PPB_NUM_SEL(ime_lib.h)
    UINT32              uiImeSliceH;
    IPPBUF_FMT          ImeFmt[MAXPathNum];///< Description
}IPPCTRL_RST,*PIPPCTRL_RST;

/**
     IPP control information

     @note
*/
typedef struct _IPPCTRL_INFO
{
    UINT32  uiDzoomStep;                     ///< dzoomidx
    UINT32  uiSieBitDepth;                   ///< 8-bit ->8, 10-bit -> 10 ...etc.
    UINT32  uiPath1OutLineoffset;            ///< Y lineoft
    UINT32  uiPath1OutCCLineoffset;          ///< CC lineoft
    UINT32  uiPath1OutsizeH;                 ///< Description
    UINT32  uiPath1OutsizeV;                 ///< Description
    BOOL    ImePath2Enable;                  ///< Description
    UINT32  uiPath2OutLineoffset;            ///< Y lineoft
    UINT32  uiPath2OutCCLineoffset;          ///< CC lineoft
    UINT32  uiPath2OutsizeH;                 ///< Description
    UINT32  uiPath2OutsizeV;                 ///< Description
    UINT32  uiImgFmt;                         ///< fmt for path1 and path2.
}IPPCTRL_INFO,*PIPPCTRL_INFO;

/**
     IPP ctrol obj

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPCTRL_OBJ
{
    IPPCTRL_OPMODE  opmode;                  ///< Description
    IPPCTRL_FLOW (*fpStrongFlow)(void);      ///< requested IPP flow
    IPPCTRL_INFO IPPctrl_info;             ///< ipp control info
    IPPCTRL_RST* pIPPctrl_rst;               ///< ipp control result
    void (*fpIppFlowApi)(PIPPCTRL_INFO, PIPPCTRL_RST);          ///< ipp flow api
    //void (*isr)(IPPISR_CB*);                  ///< ipp isr callback
}IPPCTRL_OBJ,*PIPPCTRL_OBJ;

/**
     ipp isr cb func.

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPISR_CB
{
    void (*FP_SIEISR_CB)(UINT32); ///< ipp sie isr cb
    void (*FP_IPEISR_CB)(UINT32); ///< ipp ipe isr cb
    void (*FP_IMEISR_CB)(UINT32); ///< ipp ime isr cb
    void (*FP_IFEISR_CB)(UINT32); ///< ipp ife isr cb
}IPPISR_CB,*PIPPISR_CB;

/**
     set IPP op mode

     Detailed description.
     @note (Optional)

     @param[in] IPPCtrl_obj       Description of argument arg1.
     @return Description of data returned.
         - @b LIST1:   Description of list1 (List of valid return value)
         - @b LIST2:   Description of list2 (List of valid return value)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPPCTRL_SetCmd(IPPCTRL_OBJ IPPCtrl_obj);

/**
     get ipp current flow
*/
extern IPPCTRL_FLOW IPPCTRL_GetCurFlow(void);

/**
     Get IPL stop status
*/
extern UINT32 GetIPLStopStatus(void);

/**
     Get trigger IPL stop status
*/
extern UINT32 GetIPLTriggerStop(void);

/**
     Trigger IPL stop
*/
extern void IPLTriggerStop(void);
#endif