/**
    Implement Jpg encode dc-out

    designed for frame/slice mode streaming image jpg encode dcout

    @file       Dcout.h
    @ingroup    Predefined_group_name
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _DCOUT_H
#define _DCOUT_H

#include "Type.h"
#include "jpgenc.h"
#include "jpegcommon.h"
#include "jpeg.h"
/**
     DCOUT operation

     Detailed description.
     @note (Optional)
*/
typedef enum _DCOUT_OP
{
    DCOUT_OP_UNKNOWN    = 0,     ///< Description
    DCOUT_OP_SIZEINFO   = 1,     ///< config/return dcout resolution
    DCOUT_OP_TRIGGERIDX = 2,     ///< jpg start to encode at this frame
    DCOUT_OP_FOPEN      = 3,     ///< jpg frame mode config/open,then auto trig
    DCOUT_OP_SOPEN      = 4,     ///< jpg slice mode config/open,then auto trig
    DCOUT_OP_CLOSE      = 5,     ///< close jpg
    DCOUT_OP_Max        = 6,
    ENUM_DUMMY4WORD(DCOUT_OP)
}DCOUT_OP;

/**
     Engine Trigger request

     @note (Optional)
*/
typedef enum _DCOUT_ISR
{
    DCOUT_ISR_UNKNOWN,    ///< Description
    DCOUT_ISR_IME,     ///< IME trigger request
    DCOUT_ISR_JPG,     ///< JPG trigger request
    ENUM_DUMMY4WORD(DCOUT_ISR)
}DCOUT_ISR;

/**
     Dcout return information

     Detailed description.
     @note (Optional)
*/
typedef struct _DcoutSize
{
    UINT32 uiSizeH;       ///< Description
    UINT32 uiSizeV;
    UINT32 uiLineoffsetY;
    UINT32 uiLineoffsetUV;
}DcoutSize,*PDcoutSize;

/**
     DCOUT cmd info

     Detailed description.
     @note (Optional)
*/
typedef struct _DCOUT_OBJ
{
    DCOUT_OP Op;        ///< Description

    //UINT32  uiBufaddr;        ///< BitstreamBufferaddr
    //UINT32  uiBufsize;        ///< BitstreamBiffersize
    UINT32  uiInSizeH;          ///< Bitstream inputsize
    UINT32  uiInSizeV;          ///< Bitstream inputsize
    JPG_YUV_FORMAT  Fmt;        ///< Bitstream fmt
    void (*fpImeIsrCB)(UINT32 uistatus); ///< isr callback function

    UINT32  uiPanelSizeH;       ///< panel resolution
    UINT32  uiPanelSizeV;       ///< panel resolution
    PDcoutSize pDcoutSize;      ///< Return Dcout real size//2011/05/13 Meg

    UINT32  TriggerIdx;         ///< Jpg start to trigger at idx frame
}DCOUT_OBJ,*PDCOUT_OBJ;

/**
     excute DCOUT operation

     excute dcout function base on input operation
     @note (Optional)

     @param[in] arg1       Description of argument arg1.
     @param[out] arg2      Description of argument arg2.
     @param[in,out] arg3   Description of argument arg3.
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
extern ER Dcout_Setcmd(PDCOUT_OBJ pObj);

/**
     get DCOUT current op

     return dcout current operation mode
     @note (Optional)

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
extern DCOUT_OP Dcout_GetCurOp(void);

/**
     config Jpg Q value

     for brc config jpg q value
     @note (Optional)

     @param[in] JpgQ       config jpg q value
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void Dcout_SetJpgQ(UINT32 JpgQ);

/**
     engine trigger request

     engine trigger request when use dcout for ide
     @note (Optional)

     @param[in] ISR        Trigger request from which Engine
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void Dcout_TrigCtrl(DCOUT_ISR ISR);


//#NT#2011/12/05#Ethan Lau -begin
//#NT#for image pipeline lib to get jpg interrupt status under streaming condition
extern UINT32 GetJpgStatus(void);
//#NT#2011/12/05#Ethan Lau -end
#endif