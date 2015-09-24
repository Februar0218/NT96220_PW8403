
/*
    Gsensor module driver

    This file is the driver of gsensor module

    @file       Gsensor.c
    @ingroup    mISYSGsensor
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#include "OSCommon.h"
#include "Gsensor.h"
#include "I2C.h"
#include "top.h"


static BOOL   bGsensorOpened = FALSE;

static GSENSOR_FUNC     GsensorFunc = {
    NULL, // GsensorInit
    NULL, // GetGsensorData
    NULL, // GetGsensorStatus
    NULL, // OpenGsensorInterface
    NULL, // CloseGsensorInterface
};


BOOL gsensor_open(void)
{

   if (bGsensorOpened==TRUE)
   {
     debug_msg("Gsensor open already\r\n");
     return TRUE;
   }

   bGsensorOpened=TRUE;

   // hook Gsensor functions
   gsensor_getFP(&GsensorFunc);

   // open Gsensor interface
   gsensor_OpenInterface();

   // Gsensor init
   gsensor_Init();

   return TRUE;
}

BOOL gsensor_close(void)
{

    if (bGsensorOpened==FALSE)
    {
        debug_msg("I2C close already\r\n");
        return TRUE;
    }

    // close Gsensor interface
    gsensor_CloseInterface();

    bGsensorOpened=FALSE;
    return TRUE;
}

BOOL gsensor_Init(void)
{

    if (GsensorFunc.GsensorInit != NULL)
    {
        return GsensorFunc.GsensorInit();
    }
}

void gsensor_GetData(Gsensor_Data *GS_Data)
{

    if (GsensorFunc.GetGsensorData != NULL)
    {
        GsensorFunc.GetGsensorData(GS_Data);
    }
}

void gsensor_GetStatus(UINT32 status,UINT32 *uiData)
{

    if (GsensorFunc.GetGsensorStatus!= NULL)
    {
        GsensorFunc.GetGsensorStatus(status,uiData);
    }
}

void gsensor_OpenInterface(void)
{
    if (GsensorFunc.GsensorOpenInterface != NULL)
    {
        GsensorFunc.GsensorOpenInterface();
    }
}

void gsensor_CloseInterface(void)
{
    if (GsensorFunc.GsensorCloseInterface != NULL)
    {
        GsensorFunc.GsensorCloseInterface();
    } else {
        debug_msg("%s is NULL\r\n",__func__);
    }
}

