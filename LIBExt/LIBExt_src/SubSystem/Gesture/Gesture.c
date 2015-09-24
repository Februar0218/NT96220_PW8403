/*
    Detect touch panel gesture

    Detect touch panel gesture

    @file       Gesture.c
    @ingroup    mISYSGesture
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Kernel.h"
#include "Type.h"
#include "Debug.h"
#include "Utility.h"
#include "TouchPanel.h"
#include "Gesture_Public.h"
#include "Gesture_Private.h"
#define Ao 0
#define Bo 1
#define Co 2
#define Do 3
#define Eo 4
#define Fo 5
#define LX 0
#define LY 1
#define RX 0
#define RY 1
//-------------------------------------------------------------------------------------------------
static GESTURE_INFO_STRUCT Gesture;
static GESTURE_LOCATION_STRUCT Correction;
static INT32 KX = 0;
static INT32 KY = 0;
//-------------------------------------------------------------------------------------------------
void Gesture_AddEvent(GESTURE_EVENT_ENUM  EventId)
{
    GESTURE_MSG(("GESTURE_M:AddEvent:Count=%d,EventId=%d\r\n",Gesture.EventQueue.Count,EventId));

    if(Gesture.EventQueue.Count < GESTURE_EVENT_QUEUE_AMOUNT)
    {
        Gesture.EventQueue.Event[Gesture.EventQueue.Write]=EventId;

        Gesture.EventQueue.Write++;
        if(Gesture.EventQueue.Write == GESTURE_EVENT_QUEUE_AMOUNT)
        {
           Gesture.EventQueue.Write = 0;
        }

        Gesture.EventQueue.Count++;
    }
}
//-------------------------------------------------------------------------------------------------
GESTURE_EVENT_ENUM Gesture_GetEvent(void)
{
    GESTURE_EVENT_ENUM TempEvent = GESTURE_EVENT_IDLE;

    if(Gesture.EventQueue.Count > 0)
    {
        TempEvent = Gesture.EventQueue.Event[Gesture.EventQueue.Read];

        Gesture.EventQueue.Read++;
        if(Gesture.EventQueue.Read == GESTURE_EVENT_QUEUE_AMOUNT)
        {
           Gesture.EventQueue.Read = 0;
        }

        Gesture.EventQueue.Count--;
    }
    GESTURE_MSG(("GESTURE_M:GetEvent:Count=%d,EventId=%d\r\n",Gesture.EventQueue.Count,TempEvent));

    return TempEvent;
}
//-------------------------------------------------------------------------------------------------
void Gesture_MappingLCD(
    UINT32*     pPosX,
    UINT32*     pPosY)
{
    INT32 Dx  = 0;
    INT32 Dy  = 0;
    INT32 Kx  = 0;
    INT32 Ky  = 0;
    //INT32 ARy = 0;
    //INT32 ARx = 0;

    GESTURE_MSG(("GESTURE_M:MappingLCD:PosX=%d,PosY=%d,RX=%d,RY=%d,LCDW=%d,LCDH=%d,MinX=%d,MinY=%d,MaxX=%d,MaxY=%d\r\n",(*pPosX),(*pPosY),Gesture.CalData.ReverseX,Gesture.CalData.ReverseY,Gesture.LcdInfo.Width,Gesture.LcdInfo.Height,Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));


    Kx = Gesture.CalData.KX;
    Ky = Gesture.CalData.KY;

    if((*pPosX) > Gesture.CalData.MaxX)
    {
        (*pPosX) = Gesture.CalData.MaxX;
    }
    else
    if((*pPosX) < Gesture.CalData.MinX)
    {
        (*pPosX) = Gesture.CalData.MinX;
    }

    if((*pPosY) > Gesture.CalData.MaxY)
    {
        (*pPosY) = Gesture.CalData.MaxY;
    }
    else
    if((*pPosY) < Gesture.CalData.MinY)
    {
        (*pPosY) = Gesture.CalData.MinY;
    }

    if(1)//(!Gesture.CalData.Cal_Test)
    {
        if(Gesture.CalData.ReverseX)
        {
            (*pPosX) = Gesture.LcdInfo.Width - (Gesture.LcdInfo.Width * ((*pPosX) - Gesture.CalData.MinX)/(Gesture.CalData.MaxX - Gesture.CalData.MinX));
        }
        else
        {
            (*pPosX) = Gesture.LcdInfo.Width * ((*pPosX) - Gesture.CalData.MinX)/(Gesture.CalData.MaxX - Gesture.CalData.MinX);
        }

        if(Gesture.CalData.ReverseY)
        {
            (*pPosY) = Gesture.LcdInfo.Height - (Gesture.LcdInfo.Height * ((*pPosY) - Gesture.CalData.MinY)/(Gesture.CalData.MaxY - Gesture.CalData.MinY));
        }
        else
        {
            (*pPosY) = Gesture.LcdInfo.Height * ((*pPosY) - Gesture.CalData.MinY)/(Gesture.CalData.MaxY - Gesture.CalData.MinY);
        }
    }
    else
    {
        Dx = (*pPosX)-Gesture.CalData.MinX ;
        Dy = (*pPosY)-Gesture.CalData.MaxY ;
        (*pPosX) = (Dx/(Gesture.CalData.KX));
        (*pPosY) = (Dy/(Gesture.CalData.KY));
    }

    if((*pPosX) > 0)
    {
        (*pPosX) -= 1;
    }

    if((*pPosY) > 0)
    {
        (*pPosY) -= 1;
    }

    //if(Gesture.CalData.Cal_Test)
    //debug_err(("CAL_MappingLCD:ALx:%d ALy:%d X=%d, Y=%d \r\n",ARx,ARy,(*pPosX),(*pPosY)));
    //else
    //debug_err(("GESTURE_M:MappingLCD:Out:X=%d, Y=%d\r\n",(*pPosX),(*pPosY)));

    GESTURE_MSG(("GESTURE_M:MappingLCD:Out:X=%d, Y=%d\r\n",(*pPosX),(*pPosY)));
}
//-------------------------------------------------------------------------------------------------
GESTURE_EVENT_ENUM Gesture_GetGestureSlide(void)
{
    UINT32  MoveDistanceX, MoveDistanceY;
    GESTURE_EVENT_ENUM   Event = GESTURE_EVENT_IDLE;

    MoveDistanceX = abs(Gesture.Move.EndPosX-Gesture.Move.StartPosX);
    MoveDistanceY = abs(Gesture.Move.EndPosY-Gesture.Move.StartPosY);

    if(MoveDistanceX > Gesture.Threshold.SlideRange)
    {
        if(Gesture.Move.EndPosX >= Gesture.Move.StartPosX)
        {
            Event = GESTURE_EVENT_SLIDE_RIGHT;
        }
        else
        {
            Event = GESTURE_EVENT_SLIDE_LEFT;
        }

        if(MoveDistanceX < MoveDistanceY)
        {
            if(Gesture.Move.EndPosY >= Gesture.Move.StartPosY)
            {
                Event = GESTURE_EVENT_SLIDE_DOWN;
            }
            else
            {
                Event = GESTURE_EVENT_SLIDE_UP;
            }
        }
    }
    else
    if(MoveDistanceY > Gesture.Threshold.SlideRange)
    {
        if(Gesture.Move.EndPosY >= Gesture.Move.StartPosY)
        {
            Event = GESTURE_EVENT_SLIDE_DOWN;
        }
        else
        {
            Event = GESTURE_EVENT_SLIDE_UP;
        }

        if(MoveDistanceY < MoveDistanceX)
        {
            if(Gesture.Move.EndPosX >= Gesture.Move.StartPosX)
            {
                Event = GESTURE_EVENT_SLIDE_RIGHT;
            }
            else
            {
                Event = GESTURE_EVENT_SLIDE_LEFT;
            }
        }
    }

    GESTURE_MSG(("GESTURE_M:GetGestureMove:DisX=%d, DisY=%d, SX=%d, SY=%d, EX=%d, EY=%d, Event=%d\r\n",MoveDistanceX,MoveDistanceY,Gesture.Move.StartPosX,Gesture.Move.StartPosY,Gesture.Move.EndPosX,Gesture.Move.EndPosY,Event));

    if(Event != GESTURE_EVENT_IDLE)
    {
        Gesture_AddEvent(Event);
    }

    return Event;
}
//-------------------------------------------------------------------------------------------------
void Gesture_GetGesture(
    BOOL        IsPress,
    UINT32      PosX,
    UINT32      PosY)
{
    Gesture.Time.Count++;

    if(IsPress)
    {
        if(!Gesture.Status.Press)
        {
            // First Touch panel press
            Gesture.Status.Press = IsPress;
            Gesture.Time.LastPress = Gesture.Time.Count;
            Gesture.Move.StartPosX = PosX;
            Gesture.Move.StartPosY = PosY;
            Gesture.Move.EndPosX = PosX;
            Gesture.Move.EndPosY = PosY;

            Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
        }
        else
        {
            if( !(Gesture.Status.Hold) &&
                abs(Gesture.Move.StartPosX-PosX < Gesture.Threshold.HoldRange) &&
                abs(Gesture.Move.StartPosY-PosY < Gesture.Threshold.HoldRange))
            {
                if((Gesture.Time.Count-Gesture.Time.LastPress) > Gesture.Threshold.HoldTime)
                {
                    Gesture.Status.Hold = TRUE;
                    Gesture.Gesture.Event = GESTURE_EVENT_HOLD;
                }
                else
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
                }
            }
            else
            {
                if( Gesture.Move.EndPosX != PosX ||
                    Gesture.Move.EndPosY != PosY)
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_MOVE;
                }
                else
                {
                    Gesture.Gesture.Event = GESTURE_EVENT_PRESS;
                }
            }

            Gesture.Move.EndPosX = PosX;
            Gesture.Move.EndPosY = PosY;
        }

        Gesture.Gesture.PosX = PosX;
        Gesture.Gesture.PosY = PosY;
    }
    else
    {
        if(Gesture.Status.Press)
        {
            // First touch panel release
            Gesture.Status.Press = IsPress;

            if(Gesture_GetGestureSlide() == GESTURE_EVENT_IDLE)
            {
                if(!(Gesture.Status.Hold))
                {
                    Gesture_AddEvent(GESTURE_EVENT_CLICK);
                }

                if((Gesture.Time.Count-Gesture.Time.LastRelease) < Gesture.Threshold.DoubleClickTime)
                {
                    Gesture_AddEvent(GESTURE_EVENT_DOUBLE_CLICK);
                }
            }

            Gesture.Status.Hold = FALSE;
            Gesture.Gesture.Event = GESTURE_EVENT_RELEASE;
            Gesture.Time.LastRelease = Gesture.Time.Count;
        }
        else
        {
            Gesture.Gesture.Event = Gesture_GetEvent();
        }
    }

    GESTURE_MSG(("GESTURE_M:GetGesture:IsPress=%d, X=%d, Y=%d, Event=%d\r\n",IsPress,PosX,PosY,Gesture.Gesture.Event));
}
//-------------------------------------------------------------------------------------------------
void Gesture_DetGesture(GESTURE_GESTURE_STRUCT* pGesture)
{
    BOOL    IsPress;
    UINT32  PosX, PosY;

    GESTURE_MSG(("GESTURE_M:DetGesture\r\n"));

    //#PIC#2012/03/31/david -begin
    if (Gesture.CalData.Rotate)
    {
        IsPress = tp_getPos(&PosY, &PosX);
    }
    else
    {
        IsPress = tp_getPos(&PosX, &PosY);
    }
    //#PIC#2012/03/31/david -end
    Gesture_MappingLCD(&PosX, &PosY);
    Gesture_GetGesture(IsPress, PosX, PosY);
    GESTURE_MSG(("GESTURE_M:GetGesture:Event:%d, X=%d, Y=%d\r\n",Gesture.Gesture.Event,Gesture.Gesture.PosX,Gesture.Gesture.PosY));
    memcpy((void*)pGesture,(void*)&(Gesture.Gesture), sizeof(GESTURE_GESTURE_STRUCT));
}
//-------------------------------------------------------------------------------------------------
BOOL Gesture_Cal(void)
{

    UINT16  XY_Line,XY_Ponit;
    UINT32  X1_ERR,Y1_ERR,X2_ERR,Y2_ERR;
    INT32  Logic_Loc[2][5]={0};
    INT32  Aro_Loc[2][5]={0};
    INT32  LXE_TEST,LYE_TEST;
    INT32  ERR_Value=70;
    FLOAT  ky,kx;

//Get logic and astro location.
    for(XY_Line=0;XY_Line<2;XY_Line++)
    {
        for(XY_Ponit=0;XY_Ponit<5;XY_Ponit++)
        {
          Logic_Loc[XY_Line][XY_Ponit]= (INT32)Correction.Logic_Pos[XY_Line][XY_Ponit];
          Aro_Loc[XY_Line][XY_Ponit]= (INT32)Correction.Astro_Pos[XY_Line][XY_Ponit];
          //debug_err(("Gesture_Cal:LXY=%d RXY=%d\r\n",Logic_Loc[XY_Line][XY_Ponit],Aro_Loc[XY_Line][XY_Ponit]));
        }
    }
 //Set Cal_test inital value is false
    if(!Gesture.CalData.Cal_Test)
    Gesture.CalData.Cal_Test = FALSE;

 //Calculate inaccuracy value.if value was over 70 ,it will be back to get logic value again.
    X1_ERR = abs(Logic_Loc[LX][Bo]-Logic_Loc[LX][Ao]);
      //debug_err(("Gesture_Cal:GetGesture:X1ERR=%d\r\n",X1_ERR));
    X2_ERR = abs(Logic_Loc[LX][Co]-Logic_Loc[LX][Do]);
      //debug_err(("Gesture_Cal:GetGesture:X2ERR=%d\r\n",X2_ERR));
    Y1_ERR = abs(Logic_Loc[LY][Do]-Logic_Loc[LY][Ao]);
      //debug_err(("Gesture_Cal:GetGesture:Y1ERR=%d\r\n",Y1_ERR));
    Y2_ERR = abs(Logic_Loc[LY][Co]-Logic_Loc[LY][Bo]);
      //debug_err(("Gesture_Cal:GetGesture:Y2ERR=%d\r\n",Y2_ERR));

    if(X1_ERR >=ERR_Value || X2_ERR >=ERR_Value)
       return FALSE;

    if(Y1_ERR >=ERR_Value || Y2_ERR >=ERR_Value)
       return FALSE;

 // Calculate KX and KY value.// ps. LX"a" is frist ponit (X,Y) location and LX"b" is sencond.
 //KX= ((LXd-LXa)/(RXd-RXa)+(LXc-LXb)/(RXc-RXb))/2.
// KY= ((LYb-LYa)/(RYb-RYa)+(LYc-LYd)/(RYc-RYd))/2.
    KX = ((Logic_Loc[LX][Do] - Logic_Loc[LX][Ao])/(Aro_Loc[RX][Do] - Aro_Loc[RX][Ao])
            + (Logic_Loc[LX][Co] - Logic_Loc[LX][Bo])/(Aro_Loc[RX][Co] - Aro_Loc[RX][Bo]))/2;
    kx = ((Logic_Loc[LX][Do] - Logic_Loc[LX][Ao])/(Aro_Loc[RX][Do] - Aro_Loc[RX][Ao])
            + (Logic_Loc[LX][Co] - Logic_Loc[LX][Bo])/(Aro_Loc[RX][Co] - Aro_Loc[RX][Bo]))%2;

    KY = ((Logic_Loc[LY][Bo] - Logic_Loc[LY][Ao])/(Aro_Loc[RY][Bo] - Aro_Loc[RY][Ao])
            + (Logic_Loc[LY][Co] - Logic_Loc[LY][Do])/(Aro_Loc[RY][Co] - Aro_Loc[RY][Do]))/2;

    ky=  ((Logic_Loc[LY][Bo] - Logic_Loc[LY][Ao])/(Aro_Loc[RY][Bo] - Aro_Loc[RY][Ao])
            + (Logic_Loc[LY][Co] - Logic_Loc[LY][Do])/(Aro_Loc[RY][Co] - Aro_Loc[RY][Do]))%2;
    debug_err(("Gesture_Cal:GetGesture:KX=%d, KY=%d kx:%f ky:%f \r\n",KX,KY,kx,ky));
    KX=(INT32)(KX+kx);
    KY=(INT32)(KY+ky);
   debug_err(("Gesture_Cal:GetGesture:KX=%d, KY=%d\r\n",KX,KY));

// Used formula to calculate "E" ponit logic location.
    LXE_TEST = Logic_Loc[LX][Ao]+(KX)*(Aro_Loc[RX][Eo] - Aro_Loc[RX][Ao]);

    LYE_TEST = Logic_Loc[LY][Ao]+(KY)*(Aro_Loc[RY][Eo] - Aro_Loc[RY][Ao]);

    debug_err(("Gesture_Cal_Center:LXE_TEST=%d,LYE_TEST=%d, Logic_LocX=%d, Logic_LocY=%d \r\n",LXE_TEST,LYE_TEST,Logic_Loc[LX][Eo],Logic_Loc[LY][Eo]));
    //debug_err(("Gesture_Cal:GetGesture:LXE_TEST=%d, LXE_TEST=%d\r\n",LXE_TEST,LYE_TEST));

//Compare Test "E" ponit with touch driver of logic value.If X or Y was over ERR_Value 50,return false
    X1_ERR = abs(LXE_TEST-Logic_Loc[LX][Eo]);
    Y1_ERR = abs(LYE_TEST-Logic_Loc[LY][Eo]);
    if( X1_ERR >ERR_Value || Y1_ERR >ERR_Value)
       return FALSE;

//Calculate (MinX,MinY) and (MaxX,MaxY) logic location .
    Gesture.CalData.TopLeftX = Logic_Loc[LX][Ao]-(KX)*(Aro_Loc[RX][Ao]-0);
    Gesture.CalData.TopLeftY = Logic_Loc[LY][Ao]-(KY)*(Aro_Loc[RY][Ao]-0);
    Gesture.CalData.BottomRightX = Logic_Loc[LX][Ao]+(KX)*( Gesture.LcdInfo.Width - Aro_Loc[RX][Ao]);
    Gesture.CalData.BottomRightY = Logic_Loc[LY][Ao]+(KY)*( Gesture.LcdInfo.Height - Aro_Loc[RY][Ao]);

    //debug_err(("Gesture_Cal:GetGesture:LT=%d,RT=%d,LB=%d,RB=%d\r\n",Gesture.CalData.TopLeftX,Gesture.CalData.TopLeftY,Gesture.CalData.BottomRightX,Gesture.CalData.BottomRightY));

    if(Gesture.CalData.TopLeftX == Gesture.CalData.BottomRightX)
    {
        GESTURE_ERR(("GESTURE_E:Cal:X is the same!:%d\r\n",Gesture.CalData.TopLeftX));
        return FALSE;
    }
    else
    if(Gesture.CalData.TopLeftX < Gesture.CalData.BottomRightX)
    {
        Gesture.CalData.ReverseX = FALSE;
        Gesture.CalData.MinX = Gesture.CalData.TopLeftX;
        Gesture.CalData.MaxX = Gesture.CalData.BottomRightX;
        Gesture.CalData.KX = KX;
    }
    else
    {
        Gesture.CalData.ReverseX = TRUE;
        Gesture.CalData.MinX = Gesture.CalData.BottomRightX;
        Gesture.CalData.MaxX = Gesture.CalData.TopLeftX;
        Gesture.CalData.KX = KX;
    }

    if(Gesture.CalData.TopLeftY == Gesture.CalData.BottomRightY)
    {
        GESTURE_ERR(("GESTURE_E:Cal:Y is the same!:%d\r\n",Gesture.CalData.TopLeftY));
        return FALSE;
    }
    else
    if(Gesture.CalData.TopLeftY < Gesture.CalData.BottomRightY)
    {
        Gesture.CalData.ReverseY = FALSE;
        Gesture.CalData.MinY = Gesture.CalData.TopLeftY;
        Gesture.CalData.MaxY = Gesture.CalData.BottomRightY;
        Gesture.CalData.KY = KY;
    }
    else
    {
        Gesture.CalData.ReverseY = TRUE;
        Gesture.CalData.MinY = Gesture.CalData.BottomRightY;
        Gesture.CalData.MaxY = Gesture.CalData.TopLeftY;
        Gesture.CalData.KY = KY;
    }

    GESTURE_MSG(("GESTURE_M:Cal:RX=%d, RY=%d\r\n",Gesture.CalData.ReverseX,Gesture.CalData.ReverseY));
    GESTURE_MSG(("GESTURE_M:Cal:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",Gesture.CalData.TopLeftX,Gesture.CalData.TopLeftY,Gesture.CalData.BottomRightX,Gesture.CalData.BottomRightY));
    GESTURE_MSG(("GESTURE_M:Cal:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));
    GESTURE_MSG(("GESTURE_M:Cal:KX=%d, KY=%d\r\n",Gesture.CalData.KX,Gesture.CalData.KY));
    Gesture.CalData.Cal_Test = TRUE;
    return TRUE;
}
//-------------------------------------------------------------------------------------------------
void Gesture_GetCalData(GESTURE_CALI_DATA_STRUCT* pCalData)
{
    GESTURE_MSG(("GESTURE_M:GetCalData:RX=%d, RY=%d\r\n",Gesture.CalData.ReverseX,Gesture.CalData.ReverseY));
    GESTURE_MSG(("GESTURE_M:GetCalData:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",Gesture.CalData.TopLeftX,Gesture.CalData.TopLeftY,Gesture.CalData.BottomRightX,Gesture.CalData.BottomRightY));
    GESTURE_MSG(("GESTURE_M:GetCalData:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));
    GESTURE_MSG(("GESTURE_M:GetCalData:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",Gesture.CalData.MinX,Gesture.CalData.MinY,Gesture.CalData.MaxX,Gesture.CalData.MaxY));
    GESTURE_MSG(("GESTURE_M:GetCalData:KX=%d, KY=%d \r\n",Gesture.CalData.KX,Gesture.CalData.KY));
    memcpy((void*)pCalData, (void*)&(Gesture.CalData), sizeof(GESTURE_CALI_DATA_STRUCT));
}
//-------------------------------------------------------------------------------------------------
void Gesture_SetCalData(GESTURE_CALI_DATA_STRUCT* pCalData)
{
    GESTURE_MSG(("GESTURE_M:SetCalData:RX=%d, RY=%d\r\n",pCalData->ReverseX,pCalData->ReverseY));
    GESTURE_MSG(("GESTURE_M:SetCalData:TLX=%d, TLY=%d , BRX=%d, BRY=%d\r\n",pCalData->TopLeftX,pCalData->TopLeftY,pCalData->BottomRightX,pCalData->BottomRightY));
    GESTURE_MSG(("GESTURE_M:SetCalData:MinX=%d, MinY=%d, MaxX=%d, MaxY=%d\r\n",pCalData->MinX,pCalData->MinY,pCalData->MaxX,pCalData->MaxY));
    GESTURE_MSG(("GESTURE_M:GetCalData:KX=%d, KY=%d \r\n",pCalData->KX,pCalData->KY));
    memcpy((void*)&(Gesture.CalData), (void*)pCalData, sizeof(GESTURE_CALI_DATA_STRUCT));
}
//-------------------------------------------------------------------------------------------------
BOOL Gesture_DetCalData(GESTURE_CAL_TYPE_ENUM Cal)
{
    UINT32  PosX, PosY;
    BOOL    IsPress;
    GESTURE_MSG(("GESTURE_M:DetCalData:%d\r\n",Cal));
    if (Gesture.CalData.Rotate)
    {
        IsPress = tp_getPos(&PosY, &PosX);
    }
    else
    {
        IsPress = tp_getPos(&PosX, &PosY);
    }

   if(IsPress)
   {
       if (Gesture.LcdInfo.Width < 320)  //for FDHN 176x220 Lcd panel
       {
           switch(Cal)
           {
               case GESTURE_CAL_TYPE_LEFT_TOP:
               {
                   Correction.Logic_Pos[LX][Ao] = PosX;
                   Correction.Logic_Pos[LY][Ao] = PosY;
                   debug_err(("LEFT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("LEFT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Ao] = PosX;
                   Correction.Astro_Pos[RY][Ao] = PosY;
                   if((Correction.Astro_Pos[RX][Ao]<20) || (Correction.Astro_Pos[RX][Ao]>30)\
                      || (Correction.Astro_Pos[RY][Ao]<10) || (Correction.Astro_Pos[RY][Ao]>20))
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_LEFT_BOTTOM:
               {
                   Correction.Logic_Pos[LX][Bo] = PosX;
                   Correction.Logic_Pos[LY][Bo] = PosY;
                   debug_err(("LEFT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("LEFT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Bo] = PosX;
                   Correction.Astro_Pos[RY][Bo] = PosY;
                   if((Correction.Astro_Pos[RX][Bo]<20) || (Correction.Astro_Pos[RX][Bo]>30) \
                     || (Correction.Astro_Pos[RY][Bo]<190) || (Correction.Astro_Pos[RY][Bo]>210))
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_RIGHT_BOTTOM:
               {
                   Correction.Logic_Pos[LX][Co] = PosX;
                   Correction.Logic_Pos[LY][Co] = PosY;
                   debug_err(("RIGHT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("RIGHT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Co] = PosX;
                   Correction.Astro_Pos[RY][Co] = PosY;
                   if(Correction.Astro_Pos[RX][Co]<150 || Correction.Astro_Pos[RX][Co]>165
                     || Correction.Astro_Pos[RY][Co]<190 || Correction.Astro_Pos[RY][Co]>210)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_RIGHT_TOP:
               {
                   Correction.Logic_Pos[LX][Do] = PosX;
                   Correction.Logic_Pos[LY][Do] = PosY;
                   debug_err(("RIGHT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("GESTURE_LCD:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Do] = PosX;
                   Correction.Astro_Pos[RY][Do] = PosY;
                   if(Correction.Astro_Pos[RX][Do]<150 || Correction.Astro_Pos[RX][Do]>165
                      || Correction.Astro_Pos[RY][Do]<10|| Correction.Astro_Pos[RY][Do]>20)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_CENTER:
               {
                   Correction.Logic_Pos[LX][Eo] = PosX;
                   Correction.Logic_Pos[LY][Eo] = PosY;
                   debug_err(("CENTER:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("GESTURE_LCD:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Eo] = PosX;
                   Correction.Astro_Pos[RY][Eo] = PosY;
                   if(Correction.Astro_Pos[RX][Eo]<85 || Correction.Astro_Pos[RX][Eo]>95
                      || Correction.Astro_Pos[RY][Eo]<35 || Correction.Astro_Pos[RY][Eo]>45)
                       return FALSE;
                   else
                       return IsPress;
               }

               default:
               {
                   GESTURE_ERR(("GESTURE_E:GetCalData:Unknow Cal:%d\r\n",Cal));
                   return FALSE;
               }
           }
       }
       else
       {
           switch(Cal)
           {
               case GESTURE_CAL_TYPE_LEFT_TOP:
               {
                   Correction.Logic_Pos[LX][Ao] = PosX;
                   Correction.Logic_Pos[LY][Ao] = PosY;
                   debug_err(("LEFT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("LEFT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Ao] = PosX;
                   Correction.Astro_Pos[RY][Ao] = PosY;
                   if(Correction.Astro_Pos[RX][Ao]<35 || Correction.Astro_Pos[RX][Ao]>55
                      || Correction.Astro_Pos[RY][Ao]<35 || Correction.Astro_Pos[RY][Ao]>55)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_LEFT_BOTTOM:
               {
                   Correction.Logic_Pos[LX][Bo] = PosX;
                   Correction.Logic_Pos[LY][Bo] = PosY;
                   debug_err(("LEFT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("LEFT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Bo] = PosX;
                   Correction.Astro_Pos[RY][Bo] = PosY;
                   if(Correction.Astro_Pos[RX][Bo]<35 || Correction.Astro_Pos[RX][Bo]>55
                     || Correction.Astro_Pos[RY][Bo]<180 || Correction.Astro_Pos[RY][Bo]>200)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_RIGHT_BOTTOM:
               {
                   Correction.Logic_Pos[LX][Co] = PosX;
                   Correction.Logic_Pos[LY][Co] = PosY;
                   debug_err(("RIGHT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("RIGHT_BOTTOM:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Co] = PosX;
                   Correction.Astro_Pos[RY][Co] = PosY;
                   if(Correction.Astro_Pos[RX][Co]<275 || Correction.Astro_Pos[RX][Co]>295
                     || Correction.Astro_Pos[RY][Co]<180 || Correction.Astro_Pos[RY][Co]>200)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_RIGHT_TOP:
               {
                   Correction.Logic_Pos[LX][Do] = PosX;
                   Correction.Logic_Pos[LY][Do] = PosY;
                   debug_err(("RIGHT_TOP:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("GESTURE_LCD:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Do] = PosX;
                   Correction.Astro_Pos[RY][Do] = PosY;
                   if(Correction.Astro_Pos[RX][Do]<275 || Correction.Astro_Pos[RX][Do]>295
                      || Correction.Astro_Pos[RY][Do]<35 || Correction.Astro_Pos[RY][Do]>55)
                       return FALSE;
                   else
                       return IsPress;
               }

               case GESTURE_CAL_TYPE_CENTER:
               {
                   Correction.Logic_Pos[LX][Eo] = PosX;
                   Correction.Logic_Pos[LY][Eo] = PosY;
                   debug_err(("CENTER:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Gesture_MappingLCD(&PosX,&PosY);
                   //debug_err(("GESTURE_LCD:GetGesture:X=%d, Y=%d\r\n",PosX,PosY));
                   Correction.Astro_Pos[RX][Eo] = PosX;
                   Correction.Astro_Pos[RY][Eo] = PosY;
                   if(Correction.Astro_Pos[RX][Eo]<155 || Correction.Astro_Pos[RX][Eo]>175
                      || Correction.Astro_Pos[RY][Eo]<55 || Correction.Astro_Pos[RY][Eo]>75)
                       return FALSE;
                   else
                       return IsPress;
               }
               default:
               {
                   GESTURE_ERR(("GESTURE_E:GetCalData:Unknow Cal:%d\r\n",Cal));
                   return FALSE;
               }
           }
       }
   }
   else
     return FALSE;
}
//-------------------------------------------------------------------------------------------------
void Gesture_Init(GESTURE_INIT_STRUCT* pInitPara)
{
    GESTURE_MSG(("GESTURE_M:Init:LCD W=%d, H=%d\r\n",pInitPara->LcdInfo.Width,pInitPara->LcdInfo.Height));

    Gesture.Status.Press = FALSE;
    Gesture.Status.Hold = FALSE;
    Gesture.Time.Count = 0;
    Gesture.Time.LastPress = 0;
    Gesture.Time.LastRelease = 0;

    memcpy((void*)&(Gesture.Threshold), (void*)&(pInitPara->Threshold), sizeof(GESTURE_THRESHOLD_STRUCT));
    memcpy((void*)&(Gesture.LcdInfo), (void*)&(pInitPara->LcdInfo), sizeof(GESTURE_LCD_STRUCT));
}
//-------------------------------------------------------------------------------------------------

