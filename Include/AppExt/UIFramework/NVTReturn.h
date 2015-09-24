/**
    @file       NVTReturn.h
    @ingroup    mIAPPExtUIFrmwk

    @brief      NVT UI framework return values
                Declare return values of UI framework


    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/
#ifndef NVT_ERR_H
#define NVT_ERR_H

/**
@addtogroup mIAPPExtUIFrmwk
@{
@name Return Values
@{
*/

/**
    @brief Return value data type
*/
typedef UINT32 NVTRET;

/**
    @brief Return values
*/
enum //NVTRET
{
    NVTRET_OK       = 0,                ///< OK
    NVTRET_ERROR,                       ///< ERROR
    NVTRET_CLOSE_PARENT_WND,            ///< Close parent window
    NVTRET_CLOSE_PARENT_WITH_PARAM,     ///< Close parent window with parameters
    NVTRET_TOTAL_RET
};

//@}
//@}
#endif