/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       Debug.h
    @ingroup    mISYSDebug

    @brief      Header file of debug module
                Header file of debug module

*/

/** \addtogroup mISYSDebug */
//@{

#ifndef _DEBUG_H
#define _DEBUG_H

#include "Type.h"

// Set to ENABLE to enable debug error time stamp log macro
// Set to DISABLE to disable debug error time stamp log macro
#define DEBUG_LOG_TIME                      ENABLE
// Set to ENABLE to append debug level of each output message. Ex: "ERR: Your message"
// Set to DISABLE to disable this function
#define DEBUG_LOG_LEVEL                     ENABLE

#if (DEBUG_LOG_TIME == ENABLE)
#include "RTC.h"
#endif

#define DEBUG_LEVEL_NONE                    0
#define DEBUG_LEVEL_INDICATION              1
#define DEBUG_LEVEL_WARNING                 2
#define DEBUG_LEVEL_ERROR                   3

// If DEBUG_LEVEL == DEBUG_LEVEL_NONE,
// there is no debug message will show to UART
//
// If DEBUG_LEVEL == DEBUG_LEVEL_INDICATION,
// debug_ind(), debug_wrn() and debug_err() message will show to UART
//
// If DEBUG_LEVEL == DEBUG_LEVEL_WARNING
// only debug_wrn() and debug_err() message will show to UART
//
// If DEBUG_LEVEL == DEBUG_LEVEL_ERROR
// only debug_err() message will show to UART
//
// If no DEBUG_LEVEL is defined, default level to error level
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL                         DEBUG_LEVEL_ERROR
#endif

//#NT#2010/02/08#Niven Cho -begin
//#NT#Added., Debug Marco Utility
#define CHKPNT    debug_msg("\033[37mCHK: %d, %s\033[0m\r\n",__LINE__,__func__) //!< Show a color sting of line count and function name in your insert codes
#define DBGD(x)   debug_msg("\033[0;35m%s=%d\033[0m\r\n",#x,x)                  //!< Show a color sting of variable name and variable deciaml value
#define DBGH(x)   debug_msg("\033[0;35m%s=0x%08X\033[0m\r\n",#x,x)              //!< Show a color sting of variable name and variable hexadecimal value
#define DBGTAG(x) {ID iTsk;get_tid(&iTsk);debug_msg("%s,T:%02d %s\r\n",__func__,iTsk,(x)?"-end":"-begin");} //Show function caller task with begin 'DBGTAG(0)' and end 'DBGTAG(1)'
#define ASSERT_LINE    debug_msg("^R%s %d\r\n",__func__,__LINE__)
/*
//Example:
void TestFunction(void)
{
    INT32 iCmd = 30;

    DBGTAG(0);

    iCmd++;
    DBGD(iCmd);
    CHKPNT;
    DBGH(iCmd);

    DBGTAG(1);

}

Get Result as Following:

TestFunction,T:41 -begin             //!< DBGTAG(0);
iCmd=31                              //!< DBGD(iCmd);
CHK: 216, TestFunction               //!< CHKPNT;
iCmd=0x0000001F                      //!< DBGH(iCmd);
TestFunction,T:41 -end               //!< DBGTAG(1);

*/

//#NT#2010/02/08#Niven Cho -end

#if (DEBUG_LOG_TIME == ENABLE)
/**
    Debug log structure definition, this is to be used by debug_errlog and debug_assertlog
*/
struct DebugErrorLogTag
{
    unsigned long errcode;       ///< error code for this log entry
    char *filename;              ///< the file name issues this error log
    long line;                   ///< the source line in the file that issues this error entry
    RTC_DATE date;               ///< the date stamp when this error log is issued
    RTC_TIME time;               ///< the time stamp when this error log is issued
};

/// Time stamp snapshot for error log
extern struct DebugErrorLogTag DebugErrorLog;
#endif

// Real debug funcation declaration, this is for the defined macros below, do not call this function directly
#if (DEBUG_LOG_LEVEL == ENABLE)
extern void debug_indmsg(char *fmtstr, ...);
extern void debug_wrnmsg(char *fmtstr, ...);
extern void debug_errmsg(char *fmtstr, ...);
extern void debug_assertmsg(char *fmtstr, ...);
#endif

// Real debug funcation declaration, this is for the defined macros below, do not call this function directly
extern void debug_msg(char *fmtstr, ...);

/*! \def debug_errlog(ecode,msg)
    \brief Output error message to indicate that an error condition is occured.
                  A log to internal data structure DebugErrorLog will be done.
                  Notice that this will increase code size notably.
    \param ecode Error code of type (unsigned long) to identify the error
    \param msg The text string of type (char *) to be outputed to debug console
*/
/*! \def debug_assertlog(exp,ecode,msg)
    \brief Output message to indicate that the expected condition is not correct
                  (exp=FALSE).
                  A log to internal data structure DebugErrorLog will be done.
                  Notice that this will increase code size notably.
    \param exp Boolean expression to determine whether the debug message is output or not
    \param ecode Error code of type (unsigned long) to identify the error
    \param msg The text string of type (char *) to be outputed to debug console
*/
/*! \def debug_assert(exp,msg)
    \brief Output message to indicate that the expected condition is not correct
                  (exp=FALSE).
                  Notice that this will increase code size notably.
    \param exp Boolean expression to determine whether the debug message is output or not
    \param msg The text string of type (char *) to be outputed to debug console
*/
/*! \def debug_err(msg)
    \brief Output error message to indicate that an error condition is occured.
    \param msg The text string of type (char *) to be outputed to debug console
*/
/*! \def debug_wrn(msg)
    \brief Output warning message in case that there are potential errors but
                  can be ignored without affecting the result.
    \param msg The text string of type (char *) to be outputed to debug console
*/
/*! \def debug_ind(msg)
    \brief Output message for general purpose.  This is mostly for debug purpose.
    \param msg The text string of type (char *) to be outputed to debug console
*/

#if DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
  #if (DEBUG_LOG_TIME == ENABLE)
    #define debug_errlog(ecode,msg)                 \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_errmsg msg;                   \
            }
    #define debug_assertlog(exp,ecode,msg)          \
            if (!exp)                               \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_assertmsg msg;                \
            }
  #else
    #define debug_errlog(ecode,msg)           debug_msg msg
    #define debug_assertlog(exp,ecode,msg)    debug_msg msg
  #endif
    #if (DEBUG_LOG_LEVEL == ENABLE)
        #define debug_err(msg)              debug_errmsg msg
    #else
        #define debug_err(msg)              debug_msg msg
    #endif
        #define debug_wrn(msg)
        #define debug_ind(msg)
        #define debug_assert(exp,msg)
#elif DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
  #if (DEBUG_LOG_TIME == ENABLE)
    #define debug_errlog(ecode,msg)                 \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_errmsg msg;                   \
            }
    #define debug_assertlog(exp,ecode,msg)          \
            if (!exp)                               \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_assertmsg msg;                \
            }
  #else
    #define debug_errlog(ecode,msg)           debug_msg msg
    #define debug_assertlog(exp,ecode,msg)    debug_msg msg
  #endif
    #if (DEBUG_LOG_LEVEL == ENABLE)
        #define debug_err(msg)              debug_errmsg msg
        #define debug_wrn(msg)              debug_wrnmsg msg
        #define debug_assert(exp,msg)                   \
                if (!exp)                               \
                {                                       \
                    debug_assertmsg msg;                \
                }
    #else
        #define debug_err(msg)              debug_msg msg
        #define debug_wrn(msg)              debug_msg msg
        #define debug_assert(exp,msg)                   \
                if (!exp)                               \
                {                                       \
                    debug_msg msg;                      \
                }
    #endif
        #define debug_ind(msg)
#elif DEBUG_LEVEL >= DEBUG_LEVEL_INDICATION
  #if (DEBUG_LOG_TIME == ENABLE)
    #define debug_errlog(ecode,msg)                 \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_errmsg msg;                   \
            }
    #define debug_assertlog(exp,ecode,msg)          \
            if (!exp)                               \
            {                                       \
                DebugErrorLog.errcode = (ecode);    \
                DebugErrorLog.filename = __MODULE__;\
                DebugErrorLog.line = __LINE__;      \
                DebugErrorLog.date = rtc_getDate(); \
                DebugErrorLog.time = rtc_getTime(); \
                debug_assertmsg msg;                \
            }
  #else
    #define debug_errlog(ecode,msg)           debug_msg msg
    #define debug_assertlog(exp,ecode,msg)    debug_msg msg
  #endif
    #if (DEBUG_LOG_LEVEL == ENABLE)
        #define debug_err(msg)              debug_errmsg msg
        #define debug_wrn(msg)              debug_wrnmsg msg
        #define debug_ind(msg)              debug_indmsg msg
        #define debug_assert(exp,msg)                   \
                if (!exp)                               \
                {                                       \
                    debug_assertmsg msg;                \
                }
    #else
        #define debug_err(msg)              debug_msg msg
        #define debug_wrn(msg)              debug_msg msg
        #define debug_ind(msg)              debug_msg msg
        #define debug_assert(exp,msg)                   \
                if (!exp)                               \
                {                                       \
                    debug_msg msg;                      \
                }
    #endif
#else
    #define debug_errlog(ecode,msg)
    #define debug_assertlog(exp,ecode,msg)
    #define debug_assert(exp,msg)
    #define debug_err(msg)
    #define debug_wrn(msg)
    #define debug_ind(msg)
#endif

extern void debug_dumpKernelStatus(void);
extern void debug_dumpKernelTaskInfo(void);
extern BOOL debug_dumpKernelStackInfo(UINT32 TaskID);
//#NT#2009/08/12#Elvis Chuang -begin
// transfer the put string function to pFunc
typedef ER (*FPDBGPUTSTR)(CHAR*);
extern void debug_register_putstr(FPDBGPUTSTR pFunc);
/* Example
ER New_Debug_Putstr(CHAR* DBGSTRING)
{
         ......
         return E_OK;
}

   ..
   ....
   debug_register_putstr(New_Debug_Putstr);
   ..
   ..
*/
//#NT#2009/08/12#Elvis Chuang -end
//#NT#2009/09/15#Elvis Chuang -Begin
//#NT This Macro is only for Debuging,In release Mode shoulde remove it.
//Usage: This Macro must be put in the preceding procedure code
//For example:
//INT32 AVIPlayParseIndexTbl(USHORT *pdata, UINT32 aviStartAddr)
//{
//     /*--------------------------
//       variables declartion area
//     --------------------------*/
//
//     -->ShowCallerAddr();<--
//
//     /*--------------------------
//      procedure code body
//     --------------------------*/
//}
//In Console App will show as follow string
//==>AVIPlayParseIndexTbl() is called @0x00076BD4 in AviPlayManagerTsk()
//
//According to the 0x00076BD4 to find the which is called at 0x00076BD4 inner function(caller)
//How transform bin file into disassembling
//In Project level directory
//.\Project\DemoKit>make AXF_DIS="ON" release
//This command wiil create the
//.\Project\DemoKit\450Demo_Data\Release\outfile.lst
//outfile.lst is disassembly file
//
#define ShowCallerAddr()    {\
                            extern CHAR* tasknametable[];\
                            static UINT32 CallerAddress= 0;\
                            static ID CurrentTskID = 0;\
                            __asm{MOV CallerAddress,lr    }\
                            get_tid(&CurrentTskID);\
                            debug_msg("^Y%s() is called @0x%08X in %s\r\n",\
                            __func__,CallerAddress-4,tasknametable[CurrentTskID]);\
                            }
//#NT#2009/09/15#Elvis Chuang -End

/*Don't modify and Reference the version info*/
#define DEB_MAJOR_VER       1
#define DEB_MINOR_VER       0
#define DEB_SUBMINOR_VER    0
#define DEBUG_MODULE_VERSION (DEB_MAJOR_VER<<20 | DEB_MINOR_VER<<12 | DEB_SUBMINOR_VER)
extern CHAR* debug_getVersion(UINT32 ModelVersion_h);
#endif

//@}
