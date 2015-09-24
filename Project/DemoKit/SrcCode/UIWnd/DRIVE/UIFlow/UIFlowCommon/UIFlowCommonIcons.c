#include <stdio.h>
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#include "JpgEnc.h"
#include "Exif.h"
#include "Utility.h"
#include "audio.h"

static CHAR    g_DateStr[20];
static CHAR    g_TimeStr[20];


void FlowCommon_DrawDateIcon(VControl *pCtrl)
{
    RTC_DATE    Date;

    Date = rtc_getDate();
    sprintf(g_DateStr,"%02d/%02d/%04d",Date.s.month,Date.s.day,Date.s.year);

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_DateStr));
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowCommon_HideDateIcon(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}


void FlowCommon_DrawTimeIcon(VControl *pCtrl)
{
    RTC_TIME    Time;

    Time = rtc_getTime();
    if ((Time.s.hour == 0) && (Time.s.minute == 0) && (Time.s.second==0) && (SysGetFlag(FL_TIME_FORMAT)==TIME_FORMAT_PM))
    {
        sprintf(g_TimeStr,"AM %02d:%02d:%02d",Time.s.hour+12,Time.s.minute,Time.s.second);
        SysSetFlag(FL_TIME_FORMAT, TIME_FORMAT_AM);
    }
    else if((Time.s.hour > 12)&& (SysGetFlag(FL_TIME_FORMAT)==TIME_FORMAT_PM))
    {
        sprintf(g_TimeStr,"PM %02d:%02d:%02d",Time.s.hour-12,Time.s.minute,Time.s.second);
        SysSetFlag(FL_TIME_FORMAT, TIME_FORMAT_PM);
    }
    else  if((Time.s.hour == 12)&& (Time.s.minute == 0) && (Time.s.second==0)&& (SysGetFlag(FL_TIME_FORMAT)==TIME_FORMAT_AM))
    {
        sprintf(g_TimeStr,"PM %02d:%02d:%02d",Time.s.hour,Time.s.minute,Time.s.second);
        SysSetFlag(FL_TIME_FORMAT, TIME_FORMAT_PM);

     }
    else  if((Time.s.hour > 12)&& (SysGetFlag(FL_TIME_FORMAT)==TIME_FORMAT_AM))
    {
        sprintf(g_TimeStr,"AM %02d:%02d:%02d",Time.s.hour-12,Time.s.minute,Time.s.second);
        SysSetFlag(FL_TIME_FORMAT, TIME_FORMAT_AM);
    }
    else
    {
        if (SysGetFlag(FL_TIME_FORMAT) == TIME_FORMAT_AM)
        {
            if(Time.s.hour == 0)
                sprintf(g_TimeStr,"AM %02d:%02d:%02d",Time.s.hour+12,Time.s.minute,Time.s.second);
            else
                sprintf(g_TimeStr,"AM %02d:%02d:%02d",Time.s.hour,Time.s.minute,Time.s.second);
        }
        else
        {
             sprintf(g_TimeStr,"PM %02d:%02d:%02d",Time.s.hour,Time.s.minute,Time.s.second);

        }
    }

    UxStatic_SetData(pCtrl,STATIC_VALUE,Txt_Pointer(g_TimeStr));
    UxCtrl_SetShow(pCtrl,TRUE);
}

void FlowCommon_HideTimeIcon(VControl *pCtrl)
{
    UxCtrl_SetShow(pCtrl,FALSE);
}



