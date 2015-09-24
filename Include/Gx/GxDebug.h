//    Debug

/*
Please config these symbol before you include "GxDebug.h"
1.DBG_CLASS (class flag)
2.DBG_CLASS_NAME (class string)

Usage:
//////////////////////////////////////////
//  config debug system
//////////////////////////////////////////
#define DBG_CLASS           DBG_CLASS_DSP
#define DBG_CLASS_NAME      "[Dsp]"
#include "GxDebug.h"
//////////////////////////////////////////
*/

/*
void GX_TRACE(const char *fmtstr, ...);
void GX_TRACE_EX(UINT32 uiLevel, const char *fmtstr, ...);
void GX_ASSERT(BOOL bExpression);

UINT32 GX_TRACE_GETLEVEL(void);
void GX_TRACE_SETLEVEL(UINT32 uiLevel);
UINT32 GX_TRACE_GETCLASS(void);
void GX_TRACE_ADDCLASS(UINT32 uiAddClass);
void GX_TRACE_REMOVECLASS(UINT32 uiRemoveClass);
*/

#ifndef _GX_DEBUG_H_
#define _GX_DEBUG_H_

#include "GxCommon.h"

//debug level
#define DBG_LEVEL_ALL               0x00000000
#define DBG_LEVEL_INFO              0x00000010 //info : information
#define DBG_LEVEL_NOTE              0x00000040 //note : note information
#define DBG_LEVEL_WARN              0x00000080 //warning : function is not work normally
#define DBG_LEVEL_ERROR             0x000000e0 //error : function is not supported or not work, and flow is bypassed
#define DBG_LEVEL_FATAL             0x000000ff //fatal error : function is not supported or failed, and flow is aborted
#define DBG_LEVEL_NONE              0x000000ff

#define DBG_LEVEL_DEFAULT           DBG_LEVEL_NOTE


//debug class
#define DBG_CLASS_NONE              0x00000000
#define DBG_CLASS_SYS               0x00000001 // GxSystem
#define DBG_CLASS_DISP              0x00000002 // GxDisplay
#define DBG_CLASS_GFX               0x00000004 // GxGfx
#define DBG_CLASS_ALL               0xffffffff

#define DBG_CLASS_DEFAULT           DBG_CLASS_ALL





#ifdef _DEBUG

#define DBG_DEFINE_WARNING  0

#ifndef __NAME__
#undef DBG_DEFINE_WARNING
#define DBG_DEFINE_WARNING  1
#endif //__NAME__

#ifndef __CLASS__
#undef DBG_DEFINE_WARNING
#define DBG_DEFINE_WARNING  1
#endif //__CLASS__

#if (DBG_DEFINE_WARNING)
#pragma message("ERROR: Please define your __NAME__ and __CLASS__ before include GxDebug.h.")
#pragma message("Ex:")
#pragma message("#define __NAME__     MY_CODE")
#pragma message("#define __CLASS__    0x10000000")
#pragma message("#include \"GxDebug.h\"")
#error
#endif //DBG_DEFINE_WARNING

#define _TOKEN_STR(a)  # a
#define TOKEN_STR(a)  _TOKEN_STR(a)
#define _TOKEN_BIND(a, b)  a ## b
#define TOKEN_BIND(a, b)  _TOKEN_BIND(a, b)

#define __FILELINE__    __FILE__ "(" _TOKEN_STR(__LINE__) ") : "

#include <stdarg.h>
extern UINT32 _dbg_level_;
extern UINT32 _dbg_class_;
int GxDebug_Output(const char *str);
int GxDebug_Printf(const char *fmtstr, ...);
int GxDebug_Vprintf(const char *fmtstr, va_list marker);

#ifdef _WIN32
void __stdcall PreDebugBreakAnyway();
#define GxDebug_Break()           \
    PreDebugBreakAnyway();    \
    DebugBreak();
#else
#include <assert.h>
#define GxDebug_Break()    \
    assert(0);
#endif

#define GX_TRACE                    GxDebug_Printf

#ifdef __NAME__
#ifdef __CLASS__
__inline int TOKEN_BIND(GxDebug_, __NAME__)(unsigned int level, const char *fmtstr, ...)
{
    int r;
    va_list marker;
    //filter by class
    if(!(__CLASS__ & _dbg_class_) && (level < DBG_LEVEL_FATAL))return 0;
    //filter by level
    if((level < _dbg_level_) && (level < DBG_LEVEL_FATAL))return 0;
    va_start(marker, fmtstr);
    r = GxDebug_Vprintf(fmtstr, marker);
    va_end(marker);
    if(level == DBG_LEVEL_FATAL)
    {
    GxDebug_Printf("[FATAL ERROR and program abort.]\r\n");
    GxDebug_Break();
    }
    return r;
}

#define GX_TRACE_EX                 TOKEN_BIND(GxDebug_, __NAME__)
#define GX_ASSERT(exp)  \
do {                    \
    if(!(exp))          \
    {                   \
    GxDebug_Printf("%s(%d) : ASSERT! expression=(%s)\r\n", __FILE__, __LINE__, TOKEN_STR(exp)); \
    GxDebug_Break();        \
    }                   \
} while (0)

#endif
#endif

#define GX_TRACE_SETLEVEL(v)        {_dbg_level_ = (UINT32)(v);}
#define GX_TRACE_GETLEVEL()         (_dbg_level_)
#define GX_TRACE_ADDCLASS(v)        {_dbg_class_ |= (UINT32)(v);}
#define GX_TRACE_REMOVECLASS(v)     {_dbg_class_ &= (UINT32)~(v);}
#define GX_TRACE_GETCLASS()         (_dbg_class_)


#else

#define GX_TRACE
#define GX_TRACE_EX
#define GX_ASSERT

#define GX_TRACE_SETLEVEL(a)        (0)
#define GX_TRACE_GETLEVEL()         (DBG_LEVEL_DEFAULT)
#define GX_TRACE_ADDCLASS(a)        (0)
#define GX_TRACE_REMOVECLASS(a)     (0)
#define GX_TRACE_GETCLASS()         (DBG_CLASS_DEFAULT)


#endif


#ifdef _DEBUG
#define GX_TRACE_API(api)           GX_TRACE_EX(DBG_LEVEL_INFO, _TOKEN_STR(api) "()\r\n");
#define GX_TRACE_API_2(api)         GX_TRACE_EX(DBG_LEVEL_INFO+1, _TOKEN_STR(api) "()\r\n");
#else
#define GX_TRACE_API(api)
#define GX_TRACE_API_2(api)
#endif

//#NT#2011/07/11#Elvis Chuang -begin
//#NT# Add version info and build date functions
extern CHAR *Gx_GetVerInfo_Debug(void);
extern CHAR *Gx_GetBuildDate_Debug(void);
//#NT#2011/07/11#Elvis Chuang -end

#endif//_GX_DEBUG_H_