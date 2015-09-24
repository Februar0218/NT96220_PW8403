#ifndef GESTURE_PRIVATE_H
#define GESTURE_PRIVATE_H
//-------------------------------------------------------------------------------------------------
#define GESTURE_DEBUG_MSG    0
#define GESTURE_DEBUG_ERR    1
//-------------------------------------------------------------------------------------------------
#if (GESTURE_DEBUG_MSG&DEBUG_LEVEL)
#define GESTURE_MSG(msg)     debug_msg msg
#else
#define GESTURE_MSG(msg)
#endif

#if (GESTURE_DEBUG_ERR&DEBUG_LEVEL)
#define GESTURE_ERR(msg)     debug_msg msg
#else
#define GESTURE_ERR(msg)
#endif
//-------------------------------------------------------------------------------------------------
#define GESTURE_EVENT_QUEUE_AMOUNT   5
//-------------------------------------------------------------------------------------------------
typedef struct
{
    UINT32  StartPosX;
    UINT32  StartPosY;
    UINT32  EndPosX;
    UINT32  EndPosY;
}GESTURE_MOVE_STRUCT;

typedef struct
{
    UINT32  Count;
    UINT32  LastPress;
    UINT32  LastRelease;
}GESTURE_TIME_STRUCT;

typedef struct
{
    BOOL    Press;
    BOOL    Hold;
}GESTURE_STATUS_STRUCT;

typedef struct
{
    UINT32              Count;
    UINT32              Read;
    UINT32              Write;
    GESTURE_EVENT_ENUM  Event[GESTURE_EVENT_QUEUE_AMOUNT];
}GESTURE_EVENT_QUEUE_STRUCT;

typedef struct
{
    volatile GESTURE_STATUS_STRUCT  Status;
    volatile GESTURE_TIME_STRUCT    Time;
    GESTURE_GESTURE_STRUCT          Gesture;
    GESTURE_EVENT_QUEUE_STRUCT      EventQueue;
    GESTURE_CALI_DATA_STRUCT        CalData;
    GESTURE_MOVE_STRUCT             Move;
    GESTURE_LCD_STRUCT              LcdInfo;
    GESTURE_THRESHOLD_STRUCT        Threshold;
    GESTURE_LOCATION_STRUCT         Location; 
}GESTURE_INFO_STRUCT;
//-------------------------------------------------------------------------------------------------
#endif

