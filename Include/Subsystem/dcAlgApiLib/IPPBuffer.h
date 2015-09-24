/***************************************************************************
* Copyright  Novatek Microelectronics Corp. 2008.  All rights reserved.    *
*--------------------------------------------------------------------------*
* Name: IPPBuffer Header                                                   *
* Description: This file should be duplicated in ImageAlgInfor.h           *
* Author: JJ                                                               *
****************************************************************************/
#ifndef _IPPBUFFER_H
#define _IPPBUFFER_H

#include "Type.h"

/**
    IPP_BUFFER
    Manage all the buffers for Image-Pipeline
    Add ones if necessary
*/
typedef enum {
    //Total Buffers, for all IPP Usage (PRV and Cap are assigned in the same time)
    IPPBUF_TOTAL_START=0,       //start-ptr of memory resources
    IPPBUF_TOTAL_END,           //end-ptr of memory resources
    IPPBUF_TOTAL_OVLAP_START,   //start-ptr of the capture mode
    IPPBUF_TOTAL_CURREND,       //the end of current assignment, "END" - "CURREND" = remain-memory-size
    //Preview Buffers, for all live-view mode (ex: PRV, VDO, PCC .etc)
    IPPBUF_PRV_START,           //start-ptr of memory resources
    IPPBUF_PRV_END,             //end-ptr of memory resources
    IPPBUF_PRV_CURREND,         //the end of current assignment, "END" - "CURREND" = remain-memory-size
    //Preview Buffers, for all live-view mode (ex: PRV, VDO, PCC .etc)
    IPPBUF_CAP_START,           //start-ptr of memory resources
    IPPBUF_CAP_END,             //end-ptr of memory resources
    IPPBUF_CAP_CURREND,         //the end of current assignment, "END" - "CURREND" = remain-memory-size
    IPPBUF_PRV_SIERAW_1,    //for Sensor(SIE) output raw image
    IPPBUF_PRV_SIERAW_2,
    IPPBUF_PRV_SIERAW_3,    //for sie slice mode
    IPPBUF_CA,          //sie preview ca out
    IPPBUF_PRV_IME_P1,      //for IME YCC image output
    IPPBUF_PRV_IME_P1_2,    //for IME YCC pingpong 2
    IPPBUF_PRV_IME_P1_3,    //for IME YCC pingpong 3
    IPPBUF_PRV_IME_P2,
    IPPBUF_PRV_IME_P2_2,    //for IME YCC pingpong 2
    IPPBUF_PRV_IME_P2_3,    //for IME YCC pingpong 3
    IPPBUF_PRV_IME_P2_4,    //            pingpong 4
    IPPBUF_PRV_IME_P2_5,    //            pingpong 5
    IPPBUF_PRV_FD_BUF,      //for Face-Detection buffers
    IPPBUF_PRV_SD_BUF,      //for Smile-Detection buffers
    IPPBUF_PRV_IE_BUF,      //for Image effect
    IPPBUF_PRV_IEDISPLAY_BUF,   //for Image effect display buffer
    IPPBUF_PRV_DIS_BUF,     //for Digital Image Stabilizer
    IPPBUF_JPGADDR,         //for userjpg buf start addr
    IPPBUF_CAP_RAW_1,       //for Sensor Input (PRE output)
    IPPBUF_CAP_RAW_2,
    IPPBUF_CAP_RAW_3,
    IPPBUF_CAP_RAW_4,
    IPPBUF_CAP_RAW_5,
    IPPBUF_CAP_RAW_6,
    IPPBUF_CAP_RAW_GEOMETRIC,   //buffer for Geometric-distortion-recovery
    IPPBUF_CAP_RAW_DARKFRAME,   //buffer for Dark-frame-substraction
    IPPBUF_ECS,                 //buffer for ECS,Prv and Capture share the same buffer
    IPPBUF_CAP_HEADER,          //for screen/thumbnail header
    IPPBUF_CAP_SLC_BUF,         //for Slice-mode process buffers (IME-path1 output)
    IPPBUF_CAP_FRAME_Y,
    IPPBUF_CAP_FRAME_CB,
    IPPBUF_CAP_FRAME_CR,
    IPPBUF_CAP_IE_SLC,          //image effect buffer for Slice-mode process buffers (IME-path1 output)
    IPPBUF_CAP_IE_QV_PRE,       //image effect buffer for 1st-stage QV buffer (IME-path2 output)
    IPPBUF_CAP_IE_SHADING,       //image effect buffer for shading image
    IPPBUF_CAP_QV_PRE,          //for 1st-stage QV buffer (>= quick view in frame mode,jpg dcout must use this)
    IPPBUF_CAP_QV_SCREEN,
    IPPBUF_CAP_QV_DISPLAY,      //for 2nd-stage QV buffer (for quick view)
    IPPBUF_CAP_QV_THUMBNAIL,    //for 3rd-stage QV buffer (for thumbnail)
    IPPBUF_CAP_CHROMA_FILTER,   //for chroma-filter buffer
    IPPBUF_CAP_DATA_STAMP,      //for data-stamp(date-imprint) buffer
    IPPBUF_VID_DATA_STAMP,      //for video data-stamp buffer
    IPPBUF_CAP_JPG_1,           //for jpeg header/bitstream
    IPPBUF_CAP_JPG_2,
    IPPBUF_CAP_JPG_3,
    IPPBUF_CAP_JPG_4,
    IPPBUF_CAP_JPG_5,
    IPPBUF_CAP_JPG_6,
    IPPBUF_CAP_JPG_BTSSIZE,     //for jpg bitstream size
    IPPBUF_CAP_COUPLESHOTPRV,
    IPPBUF_CAP_PRESHOTPRV,
    IPPBUF_CAP_IFESLICE,        //for slice mode ife d2d buf.
    IPPBUF_CAP_IFESLICE_SRC_Y,  //for slice mode ife d2d upside boundry clean src
    IPPBUF_CAP_IFESLICE_SRC_CC, //for slice mode ife d2d upside boundry clean src
    IPPBUF_MAX_INDEX
} IPPBUF_BUFFER;

typedef enum {
    IPPBUF_RES_PRV = 0,
    IPPBUF_RES_CAP,
    IPPBUF_RES_MAX_INDEX
} IPPBUF_RESOURCE;


/**
    IPPBUF_MODE
    Mode selection for IPP-Buffer-Allocation
    Add ones if necessary
*/
typedef enum {
    //Preview modes, for all live-view/running-frames modes
    IPPBUF_MODE_F2F = 0,        // for Preview/Live-view mode
    IPPBUF_MODE_S2F,            // for Preview/Live-view mode
    IPPBUF_MODE_S2S,            // for Preview/Live-view mode
    //Capture modes, for all still-image modes
    IPPBUF_MODE_CAP_NORMAL,     // for normal/single-shot capture
    IPPBUF_MODE_CAP_ALBUMSHOT,  // for album-shot mode
    IPPBUF_MODE_CAP_COUPLESHOT,
    IPPBUF_MODE_CAP_PRESHOT,
    IPPBUF_MODE_CAP_NORMAL_FRAME,
    IPPBUF_MODE_MAX_INDEX
} IPPBUF_MODE;


/**
    IPPBuffer Error Message
*/
#define IPPBUF_E_OK                (0)         /* Normal termination */
#define IPPBUF_E_NOINDEX           (1)         /* Index Out of range */
#define IPPBUF_E_OUTOFBUFF         (2)         /* Out of buffer */

typedef enum
{
    FMT444 = 1,
    FMT422 = 2,
    FMT420 = 4,
    ENUM_DUMMY4WORD(IPPBUF_FMT)
}IPPBUF_FMT;

typedef enum
{
    ImePath1    = 0,
    ImePath2    = 1,
    MAXPathNum  = 2,
    ENUM_DUMMY4WORD(IMEOUTPUTCHANEL)
}IMEOUTPUTCHANEL;

typedef struct _ENG2DRAMINFO
{
    BOOL        ImePath2Enable;
    IPPBUF_FMT  Ime_Fmt[MAXPathNum];
    UINT32      ImeBufferNum[MAXPathNum];       ///<ref. enum:_IME_PPB_NUM_SEL(ime_lib.h)
    UINT32      ImeSliceH;                      ///<the valid slice H is 32 ~ 2048,plz ref. ime_lib.h
    UINT32      ImeOutV[MAXPathNum];
    UINT32      ImeOutLineOffset[MAXPathNum];   ///<Y lineoft
    UINT32      ImeOutCCLineOffset[MAXPathNum]; ///<CC lineoft
    UINT32      SieBufferNum;                   ///<ref. enum:SIE_OUTBUFNUM(sie_lib.h)
    UINT32      SieSliceH;
    UINT32      EcsMapSize;                     ///<ref. enum:SIE_EcsMapSizeEnum(sie_lib.h)
    UINT32      SieBitDepth;                    ///<8-bit -> 8,10-bit -> 10 ...etc,
    UINT32      CropH;                          ///<sie to dram size
    UINT32      CropV;                          ///<sie to dram size
}IPPBUF_ENG2DRAMINFO,*pIPPBUF_ENG2DRAMINFO;

/**
     IPPBUF Assigner result

     Detailed description.
     @note (Optional)
*/
typedef struct _IPPBUF_RST
{
    ER     er_code;         ///< Description
    UINT32 uibuffer;        ///< required buffer size
}IPPBUF_RST,*PIPPBUF_RST;

typedef IPPBUF_RST (*FPIPPBUFASSIGNER)(UINT32, UINT32, pIPPBUF_ENG2DRAMINFO);

/**
    Assigner Function Type

    Set IPP buffers for certain mode

    @param[in-1] UINT32     BufAddr, memory resource start-ptr
    @param[in-2] UINT32     BufSize, memory resource size

    @return      UINT32     BufferSize in Need

*/
extern UINT32 ippbuf_SetBuf(IPPBUF_BUFFER BufferIdx, UINT32 setValue);
extern UINT32 ippbuf_GetBuf(IPPBUF_BUFFER BufferIdx);
extern IPPBUF_RST ippbuf_ExecuteAssigner(IPPBUF_MODE modeIdx,pIPPBUF_ENG2DRAMINFO pInfo);
extern UINT32 ippbuf_SetResource(IPPBUF_RESOURCE ResIdx, UINT32 BufAddr, UINT32 BufSize);
extern UINT32 ippbuf_GetResource(IPPBUF_RESOURCE ResIdx, UINT32 *BufAddr, UINT32 *BufSize);
extern UINT32 ippbuf_RegisterAssigner(IPPBUF_MODE modeIdx, FPIPPBUFASSIGNER pIPPBufAssigner);
extern void   ippbuf_SetResetFlag(BOOL Flag);
//#NT#2012/08/09#KS Hung -begin
//#NT#Set the JPG&RAW Buffer overlap or not
extern void   ippbuf_SetJpgRawOverlap(BOOL Flag);
//#NT#2012/08/09#KS Hung -end
#endif// _IPPBUFFER_H
