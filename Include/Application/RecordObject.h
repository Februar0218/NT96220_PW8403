#ifndef _RECORDOBJECT_H
#define _RECORDOBJECT_H

#include "type.h"

typedef enum
{
    RECOBJ_CLOSE_NOWAIT = 0x1,
    RECOBJ_CLOSE_WAITEND= 0x2,
    ENUM_DUMMY4WORD(RECOBJ_CLOSE_TYPE)
} RECOBJ_CLOSE_TYPE;

typedef enum
{
    RECOBJ_GETINFO_HEADERSIZE = 0x1,   ///< param1: headersize per second(out)
    ENUM_DUMMY4WORD(RECOBJ_GETINFO_TYPE)
} RECOBJ_GETINFO_TYPE;

typedef enum
{
    RECOBJ_SETINFO_HEADERSIZE = 0x1,   ///< param1: headersize per second(out)
    RECOBJ_SETINFO_MED_ENDTYPE =0x81,  ///< param1: MEDIAREC_ENDTYPE_NORMAL or others
    ENUM_DUMMY4WORD(RECOBJ_SETINFO_TYPE)
} RECOBJ_SETINFO_TYPE;


typedef struct
{
    ER (*Open)(void *pObj);             ///< open
    ER (*Close)(RECOBJ_CLOSE_TYPE type);           ///<
    ER (*RecordStart)(void *pobj);      ///<
    ER (*Stop)(UINT32 type);            ///<
    ER (*GetInfo)(RECOBJ_GETINFO_TYPE type, UINT32 *pparam1, UINT32 *pparam2, UINT32 *pparam3);
    ER (*SetInfo)(RECOBJ_SETINFO_TYPE type, UINT32 param1, UINT32 param2, UINT32 param3);
    ER (*CustomizeFunc)(UINT32 type, void *pobj);    ///<
    UINT32 uiCheckID;                     ///< checkID
} RECORDOJBECT, *PRECORDOJBECT;


#endif
