/**
    @file       NVTSysCommand.h
    @ingroup    mIAPPExtUIFrmwk

    @brief      NVT UI framework system events
                Declare system events of UI framework.

    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.
*/

#ifndef NVT_SYS_COMMAND_H
#define NVT_SYS_COMMAND_H

/**
@addtogroup mIAPPExtUIFrmwk
@{

@name System Commands
@{
*/

/**
    @brief NVT event data type
*/
//typedef UINT32 NVTEVT;
typedef UINT32 NVTEVT;

/**
    @brief NVT event values
*/
//enum NVTEVT
enum
{
    NVTEVT_NULL             = 0,   ///< NULL COMMAND
    NVTEVT_OPEN_WINDOW,            ///< OPEN WINDOW COMMAND
    NVTEVT_CLOSE_WINDOW,           ///< CLOSE WINDOW COMMAND
    NVTEVT_CHILD_CLOSE,            ///< CHILD WINDOW CLOSE COMMAND
    NVTEVT_REDRAW,                 ///< Ux_Redraw COMMAND

    NVTEVT_KEY_PRESS,
    NVTEVT_KEY_RELEASE,
    NVTEVT_KEY_PRESS_HOLD,

    NVTEVT_BEGIN_CTRL,             ///< BEGIN CONTROL COMMAND
    NVTEVT_END_CTRL,               ///< END CONTROL COMMAND
    NVTEVT_BACKGROUND_DONE,        ///< BACKGROUND DONE COMMAND
    NVTEVT_OUT_RANGE,    
    NVTEVT_SYSCMD_MAX              ///< Max values of system event. NVTUserCommand adds new event after this value.
    //MAX = 10
};

//@}
//@}
#endif
