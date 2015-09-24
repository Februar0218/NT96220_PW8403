/**
    @file       MessageElement.h
    @ingroup    mIAPPExtUIFrmwk

    @brief      NVT Message Element
                Declare data type and function prototype of message element


    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H
#include "Type.h"
#include "NVTSysCommand.h"

/**
@addtogroup mIAPPExtUIFrmwk
@{
@name Message Element Data Structures
@{
*/

/**
    @brief Define the maximum number of parameters carried with a event
*/
#define MAX_MESSAGE_PARAM_NUM      3

/**
    @brief Message Content
*/
typedef struct _NVTMSG
{
    NVTEVT event;                             ///< Command
    UINT32 wParam[MAX_MESSAGE_PARAM_NUM];       ///< Parameter
    UINT32 paramNum;
} NVTMSG;

//@}
//@}

extern void MessageElementInit(void);
extern UINT8 IsMessageElementInit(void);
extern ER Get_NVTMSG_Element(NVTMSG **p_msg);
extern void Free_NVTMSG_Element(NVTMSG *p_msg);
extern void Message_DumpStatus(void);
#endif
