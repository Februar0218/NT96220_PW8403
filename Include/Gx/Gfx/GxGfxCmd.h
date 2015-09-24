
//    Command

#ifndef _GX_COMMAND_H_
#define _GX_COMMAND_H_

#include "GxCommon.h"
#include "GxDC.h"
#include "GxRect.h"

#ifdef __cplusplus
extern "C" {
#endif

#if 0
//--------------------------------------------------------------------------------------
//      Command
//--------------------------------------------------------------------------------------

//<type>

typedef UINT32 DC_CMD;

//<enum>

//CMD
#define CMD_END                     0x00

#define CMD_STATE_OP                0x10    
#define CMD_SETBR                   0x10
#define CMD_MOVETO                  0x11
#define CMD_MOVEBACK                0x12
#define CMD_SETORIGIN               0x13
#define CMD_SETWINDOW               0x14

#define CMD_PRIM_OP                 0x20    
#define CMD_CLEAR                   0x20
#define CMD_POINT                   0x21
#define CMD_LINETO                  0x22
#define CMD_LINE                    0x23
#define CMD_FILLRECT                0x24
#define CMD_FRAMERECT               0x25
#define CMD_INVERTRECT              0x26
#define CMD_RECTANGLE               0x27
#define CMD_ROUNDRECT               0x28
#define CMD_ELLIPSE                 0x29

#define CMD_IMAGE_OP                0x40    
#define CMD_IMAGE                   0x40
#define CMD_IMAGERECT               0x41  

#define CMD_TEXT_OP                 0x80    
#define CMD_TEXT                    0x80
#define CMD_TEXTRECT                0x81


//<function>

#define CMD_HEADER(iCmd, nParam, nFlag)         ((((UINT32)(iCmd)) & 0x000000FF) << 24) | \
                                                ((((UINT32)(nParam)) & 0x000000FF) << 16) | \
                                                (((UINT32)(nFlag)) & 0x0000FFFF)
#define CMD_PARAM(vParam)                       ((UINT32)(vParam))

#define GET_ECMD_iCmd(cmd)                      ((((UINT32)(cmd)) & 0xFF000000) >> 24)
#define GET_ECMD_nParam(cmd)                    ((((UINT32)(cmd)) & 0x00FF0000) >> 16)
#define GET_ECMD_nFlag(cmd)                     (((UINT32)(cmd)) & 0x0000FFFF)


//format: [cmd]|[n]|[flag], [param-1], [param-2], ..., [param-n]

//nFlag : 16 bits to noted type of [param-i], 0=constant type, 1=variable type
//        there are total 16 registers to support variable value
//        if [param-i] is vairable type, the value of param = index of register

#define ECMD_End()                              CMD_HEADER(CMD_END, 0, 0)
#define ECMD_SetBrush(p, v)                     CMD_HEADER(CMD_SETBR, 2, 0), CMD_PARAM(p), CMD_PARAM(v)
#define ECMD_SetOrigin(x1, y1)                  CMD_HEADER(CMD_SETORIGIN, 2, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_SetWindow(x1, y1, x2, y2)          CMD_HEADER(CMD_SETWINDOW, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_MoveTo(x1, y1)                     CMD_HEADER(CMD_MOVETO, 2, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_MoveBack()                         CMD_HEADER(CMD_MOVEBACK, 0, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_Clear()                            CMD_HEADER(CMD_CLEAR, 0, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_Point(x1, y1)                      CMD_HEADER(CMD_POINT, 2, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_LineTo(x1, y1)                     CMD_HEADER(CMD_LINETO, 2, 0), CMD_PARAM(x1), CMD_PARAM(y1)
#define ECMD_Line(x1, y1, x2, y2)               CMD_HEADER(CMD_LINE, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_FillRect(x1, y1, x2, y2)           CMD_HEADER(CMD_FILLRECT, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_FrameRect(x1, y1, x2, y2)          CMD_HEADER(CMD_FRAMERECT, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_InvertRect(x1, y1, x2, y2)         CMD_HEADER(CMD_INVERTRECT, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_Rectangle(x1, y1, x2, y2)          CMD_HEADER(CMD_RECTANGLE, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_RoundRect(x1, y1, x2, y2, rw, rh)  CMD_HEADER(CMD_ROUNDRECT, 6, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2), CMD_PARAM(cw), CMD_PARAM(ch)
#define ECMD_Ellipse(x1, y1, x2, y2)            CMD_HEADER(CMD_ELLIPSE, 4, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2)
#define ECMD_Image(x1, y1, id)                  CMD_HEADER(CMD_IMAGE, 3, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(id)
#define ECMD_ImageRect(x1, y1, x2, y2, id)      CMD_HEADER(CMD_IMAGERECT, 5, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2), CMD_PARAM(id)
#define ECMD_Text(x1, y1, id)                   CMD_HEADER(CMD_TEXT, 3, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(id)
#define ECMD_TextRect(x1, y1, x2, y2, id)       CMD_HEADER(CMD_TEXTRECT, 5, 0), CMD_PARAM(x1), CMD_PARAM(y1), CMD_PARAM(x2), CMD_PARAM(y2), CMD_PARAM(id)

RESULT GxGfx_Execute(DC* pDestDC, const DC_CMD* pCmd);
#endif

#ifdef __cplusplus
} //extern "C"
#endif

#endif//_GX_COMMAND_H_
