#ifndef GESTURE_PUBLIC_H
#define GESTURE_PUBLIC_H
//-------------------------------------------------------------------------------------------------
typedef enum
{
    GESTURE_EVENT_IDLE,
    GESTURE_EVENT_PRESS,
    GESTURE_EVENT_RELEASE,
    GESTURE_EVENT_MOVE,
    GESTURE_EVENT_HOLD,
    GESTURE_EVENT_CLICK,
    GESTURE_EVENT_DOUBLE_CLICK,
    GESTURE_EVENT_SLIDE_UP,
    GESTURE_EVENT_SLIDE_DOWN,
    GESTURE_EVENT_SLIDE_LEFT,
    GESTURE_EVENT_SLIDE_RIGHT
}GESTURE_EVENT_ENUM;

typedef enum
{
    GESTURE_CAL_TYPE_LEFT_TOP,
    GESTURE_CAL_TYPE_LEFT_BOTTOM,
    GESTURE_CAL_TYPE_RIGHT_TOP,
    GESTURE_CAL_TYPE_RIGHT_BOTTOM,
    GESTURE_CAL_TYPE_CENTER
}GESTURE_CAL_TYPE_ENUM;

typedef struct
{
    BOOL    ReverseX;
    BOOL    ReverseY;
    BOOL    Rotate;
    BOOL    Cal_Test;
    UINT32  TopLeftX;
    UINT32  TopLeftY;
    UINT32  BottomRightX;
    UINT32  BottomRightY;
    UINT32  MinX;
    UINT32  MinY;
    UINT32  MaxX;
    UINT32  MaxY;
    INT32   KX;
    INT32   KY;
}GESTURE_CALI_DATA_STRUCT;

typedef struct
{
    GESTURE_EVENT_ENUM  Event;
    UINT32              PosX;
    UINT32              PosY;
}GESTURE_GESTURE_STRUCT;

typedef struct
{
    UINT32       Logic_Pos[2][5];
    UINT32       Astro_Pos[2][5];//astrophysicist location
}GESTURE_LOCATION_STRUCT;

typedef struct
{
    UINT32      Width;
    UINT32      Height;
}GESTURE_LCD_STRUCT;

typedef struct
{
    UINT32      SlideRange;         //Pixel
    UINT32      HoldRange;          //Pixel
    UINT32      HoldTime;           //Timer count
    UINT32      DoubleClickTime;    //Timer count
}GESTURE_THRESHOLD_STRUCT;

typedef struct
{
    GESTURE_THRESHOLD_STRUCT    Threshold;
    GESTURE_LCD_STRUCT          LcdInfo;
}GESTURE_INIT_STRUCT;
//-------------------------------------------------------------------------------------------------
extern void Gesture_DetGesture(GESTURE_GESTURE_STRUCT* pGesture);
extern BOOL Gesture_Cal(void);
extern void Gesture_GetCalData(GESTURE_CALI_DATA_STRUCT* pCalData);
extern void Gesture_SetCalData(GESTURE_CALI_DATA_STRUCT* pCalData);
extern BOOL Gesture_DetCalData(GESTURE_CAL_TYPE_ENUM CalType);
extern void Gesture_Init(GESTURE_INIT_STRUCT* pInitPara);
//-------------------------------------------------------------------------------------------------
#endif

