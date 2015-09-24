/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       LensCmd.c
    @ingroup    mISYSLens

    @brief      Lens Test command functions

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA4
    @date       2009/01/06
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lens_int.h"
#include "LensCmd.h"
#include "LensAPI.h"
#include "Utility.h"
#include "gpio.h"
#include "pad.h"
#include "sif.h"

// ----------------------------------------------------------------------------
// Globals & Definitions
// ----------------------------------------------------------------------------
// Lens model id list
#define LENSMID_FF              0
#define LENSMID_ST8003          1
#define LENSMID_ST8005          2
#define LENSMID_DZ5311          3
#define LENSMID_ST2089          4
#define LENSMID_UNKNOWN         0xFF

// Motor model id list
#define MOTORMID_PSEUDO         0
#define MOTORMID_WT7026         1
#define MOTORMID_R2A30423       2
#define MOTORMID_UNKNOWN        0xFF

// Lens cmd user current using lens/motor model id
// If user do not need lens commands, please set as unknown state,
// to save image size after lib linking!
#define USER_LENSMID            LENSMID_ST2089
#define USER_MOTORMID           MOTORMID_WT7026

// ----------------------------------------------------------------------------
// Static Variables & Functions
// ----------------------------------------------------------------------------
static LENS_CMD_LENSMODEL gLensModelTbl[] =
{
    { LENSMID_FF,               "FF"            },
    { LENSMID_ST8003,           "ST8003"        },
    { LENSMID_ST8005,           "ST8005"        },
    { LENSMID_DZ5311,           "DZ5311"        },
    { LENSMID_ST2089,           "ST2089"        },

    { LENSMID_UNKNOWN,          "UNKNOWN"       }
};

static LENS_CMD_MOTORMODEL gMotorModelTbl[] =
{
    { MOTORMID_PSEUDO,          "PSEUDO"        },
    { MOTORMID_WT7026,          "WT7026"        },
    { MOTORMID_R2A30423,        "R2A30423"      },

    { MOTORMID_UNKNOWN,         "UNKNOWN"       }
};

static CHAR *Lens_Cmd_GetLensModelString(UINT32 uiModelID);
static CHAR *Lens_Cmd_GetMotorModelString(UINT32 uiModelID);

// ----------------------------------------------------------------------------
// Lens Command APIs
// ----------------------------------------------------------------------------
static CHAR *Lens_Cmd_GetLensModelString(UINT32 uiModelID)
{
    UINT32 uiTotal = (sizeof(gLensModelTbl)/sizeof(LENS_CMD_LENSMODEL));
    UINT32 i = 0;

    while(i < uiTotal)
    {
        if(gLensModelTbl[i].uiLensModelID == uiModelID)
        {
            return gLensModelTbl[i].pcLensModelStr;
        }
        i++;
    }
    debug_err(("LENS: Unknown lens model id %d\r\n", uiModelID));
    return NULL;
}

static CHAR *Lens_Cmd_GetMotorModelString(UINT32 uiModelID)
{
    UINT32 uiTotal = (sizeof(gMotorModelTbl)/sizeof(LENS_CMD_MOTORMODEL));
    UINT32 i = 0;

    while(i < uiTotal)
    {
        if(gMotorModelTbl[i].uiMotorModelID == uiModelID)
        {
            return gMotorModelTbl[i].pcMotorModelStr;
        }
        i++;
    }
    debug_err(("LENS: Unknown motor model id %d\r\n", uiModelID));
    return NULL;
}

// ----------------------------------------------------------------------------
// Lens User Command List
// ----------------------------------------------------------------------------
void Lens_Cmd(CHAR *pcCmdStr)
{
    static PLENS_TAB pLensTab = NULL;
    static PMOTOR_TAB pMotorTab = NULL;
    static BOOL bLensInit = FALSE;
    INT32 status;

    switch (*pcCmdStr)
    {
    /* Init */
    case 'i':

        if (!strncmp(pcCmdStr, "init", 4))
        {
            LENS_DEVICE_OBJ LensDevice = {0};
            UINT32          uiBufGPIO[LENS_IO_MAX];
            UINT32          uiBufPAD[LENS_PAD_MAX];

            debug_err(("lens init\r\n"));
            debug_err(("LENS: LensDrv (%s), MotorDrv (%s)\r\n",
            Lens_Cmd_GetLensModelString(USER_LENSMID),
            Lens_Cmd_GetMotorModelString(USER_MOTORMID)));

            LensDevice.puiMotorCtrlGPIOArray                    = &uiBufGPIO[0];
            LensDevice.puiMotorCtrlPADArray                     = &uiBufPAD[0];

            //Lens
            pLensTab = lens_getObject();

            //Motor
            #if 0
            #if (USER_MOTORMID == MOTORMID_WT7026)
            #if (USER_LENSMID == LENSMID_ST2089)
            #if 1//for realboard
            /*LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_SIF|MOTOR_CTRL_GPIO;
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_GPIOSIM;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH0;
            
            LensDevice.LensSIFPinObj.uiGPIOSen                  = (D_GPIO_1);//GPIO_LENS_IO_SIMSIF_SEN;
            LensDevice.LensSIFPinObj.uiGPIOClk                  = (C_GPIO_11);//GPIO_LENS_IO_SIMSIF_CLK;
            LensDevice.LensSIFPinObj.uiGPIOData                 = (C_GPIO_12);//GPIO_LENS_IO_SIMSIF_DATA;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = 63;//GPIO_LENS_IO_MODE_1;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = 63;//GPIO_LENS_IO_MODE_2;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = (S_GPIO_17);//GPIO_LENS_IO_IN_0;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = (S_GPIO_18);//GPIO_LENS_IO_IN_1;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = (C_GPIO_5);//GPIO_LENS_IO_IN_2;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = (C_GPIO_6);//GPIO_LENS_IO_IN_3;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = 63;//GPIO_LENS_IO_PWR;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR_INVERSE]   = 0; // motor power is not inverse
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOTOR_INVERSE] = 0; // motor is not inverse

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI]   = (D_GPIO_0);//GPIO_LENS_IO_ZOOM_PI;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = (P_GPIO_1);//GPIO_LENS_IO_ZOOM_PR;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = (C_GPIO_9);//GPIO_LENS_IO_FOCUS_PI;

            LensDevice.uiMotorCtrlGPIOIntrID                    = (GPIO_INT_32);//GPIO_LENS_INT_ID;

            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI]   = (PAD_PIN_DGPIO0);//GPIO_LENS_PAD_ZOOM_PI;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = (PAD_PIN_SBDAT0);//GPIO_LENS_PAD_ZOOM_PR;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = (PAD_PIN_MC9);//GPIO_LENS_PAD_FOCUS_PI;*/

            #else// for cdoe demoboard
            LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_SIF|MOTOR_CTRL_GPIO;
#if 0
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_CONTROLLER;
            LensDevice.LensSIFPinObj.uiSIFChannel               = GPIO_LENS_SIF_CHANNEL;
#else
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_GPIOSIM;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH0;
#endif
            LensDevice.LensSIFPinObj.uiGPIOSen                  = (D_GPIO_1);//GPIO_LENS_IO_SIMSIF_SEN;
            LensDevice.LensSIFPinObj.uiGPIOClk                  = (P_GPIO_0);//GPIO_LENS_IO_SIMSIF_CLK;
            LensDevice.LensSIFPinObj.uiGPIOData                 = (P_GPIO_1);//GPIO_LENS_IO_SIMSIF_DATA;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = (C_GPIO_11);//GPIO_LENS_IO_MODE_1;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = 63;//GPIO_LENS_IO_MODE_2;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = (S_GPIO_17);//GPIO_LENS_IO_IN_0;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = (S_GPIO_18);//GPIO_LENS_IO_IN_1;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = (C_GPIO_5);//GPIO_LENS_IO_IN_2;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = (C_GPIO_12);//GPIO_LENS_IO_IN_3;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = 63;//GPIO_LENS_IO_PWR;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR_INVERSE]   = 0; // motor power is not inverse
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOTOR_INVERSE] = 0; // motor is not inverse

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI]   = (C_GPIO_13);//GPIO_LENS_IO_ZOOM_PI;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = (C_GPIO_6);//GPIO_LENS_IO_ZOOM_PR;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = (C_GPIO_10);//GPIO_LENS_IO_FOCUS_PI;

            LensDevice.uiMotorCtrlGPIOIntrID                    = (GPIO_INT_07);//GPIO_LENS_INT_ID;

            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI]   = (PAD_PIN_MC13);//GPIO_LENS_PAD_ZOOM_PI;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = (PAD_PIN_MC6);//GPIO_LENS_PAD_ZOOM_PR;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = (PAD_PIN_MC10);//GPIO_LENS_PAD_FOCUS_PI;
            #endif
            #else
            LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_SIF|MOTOR_CTRL_GPIO;
            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_GPIOSIM;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH0;
            LensDevice.LensSIFPinObj.uiGPIOSen                  = P_GPIO_8;//UART_RX
            LensDevice.LensSIFPinObj.uiGPIOClk                  = P_GPIO_0;
            LensDevice.LensSIFPinObj.uiGPIOData                 = P_GPIO_1;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = 87;//reset
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = 87;
            #if 0
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = C_GPIO_5;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = C_GPIO_12;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = S_GPIO_17;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = S_GPIO_18;
            #else  // for CDOE_223
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_0]      = S_GPIO_17;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_1]      = S_GPIO_18;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_2]      = C_GPIO_5;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_IN_3]      = C_GPIO_12;
            #endif
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = C_GPIO_11;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOTOR_INVERSE] = 0;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PI]   = C_GPIO_13;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = C_GPIO_6;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = C_GPIO_10;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR_INVERSE]   = 0;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOTOR_INVERSE]  = 1;

            LensDevice.uiMotorCtrlGPIOIntrID                    = GPIO_INT_07;

            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI]   = PAD_PIN_MC13;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = PAD_PIN_MC6;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = PAD_PIN_MC10;

            #endif
            #elif (USER_MOTORMID == MOTORMID_R2A30423)
            #if 0
            LensDevice.uiMotorCtrlMode                          = MOTOR_CTRL_SIF|MOTOR_CTRL_GPIO;

            LensDevice.LensSIFPinObj.uiSIFType                  = LENS_SIF_CONTROLLER;
            LensDevice.LensSIFPinObj.uiSIFChannel               = SIF_CH0;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_PWR]       = (D_GPIO_3|GPIO_IS_DGPIO);

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_1]    = S_GPIO_19;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MODE_2]    = C_GPIO_31;

            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_ZOOM_PR]   = P_GPIO_11;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_FOCUS_PI]  = P_GPIO_12;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT1]  = P_GPIO_16;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_EXT2]  = P_GPIO_15;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB1]  = P_GPIO_20;
            LensDevice.puiMotorCtrlGPIOArray[LENS_IO_MOT_MOB2]  = P_GPIO_19;

            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PI]   = PAD_PIN_UART2_TX;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_ZOOM_PR]   = PAD_PIN_SBCS2;
            LensDevice.puiMotorCtrlPADArray[LENS_PAD_FOCUS_PI]  = PAD_PIN_SBCS3;
            #endif

            #else
            //Do nothing
            #endif
	     #endif

            // Init motor object (get motor function table)
            pMotorTab                               = motor_getObject();

            Lens_Module_Init(&LensDevice, pLensTab, pMotorTab);
            bLensInit = TRUE;
           pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "info", 4))
        {
            debug_err(("power = %d\r\n", gpio_getPin(C_GPIO_11)));
            //debug_err(("clk en = %d\r\n", gpio_getPin(S_GPIO_19)));
            //debug_err(("reset = %d\r\n", gpio_getPin(S_GPIO_17)));
            pMotorTab->power(MOTOR_POWER_ON, 0);
            debug_err(("zoom PR = %d\r\n", pMotorTab->zoom_checkPR()));
            debug_err(("focus PI = %d\r\n", pMotorTab->focus_checkPI()));
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }

        break;

    /* Focus */
    case 'f':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "fh", 2))
        {
            debug_err(("lens focus home\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoHome();

            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "ffw", 3))
        {
            #if (USER_LENSMID == LENSMID_ST2089)
            UINT32 step;
            debug_err(("lens focus fw\r\n"));
            sscanf(pcCmdStr+3,"%d",&step);
            debug_msg("%d\r\n",step);
            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC|FOCUS_FORWARD, step);
            //pMotorTab->focus_setState(MOTOR_FOCUS_FWD, 256);
            #else
            debug_err(("lens focus fw\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 256);
            //pMotorTab->focus_setState(MOTOR_FOCUS_FWD, 256);

            #endif
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "fbw", 3))
        {
            #if (USER_LENSMID == LENSMID_ST2089)
            UINT32 bstep;
            debug_err(("lens focus bw\r\n"));
            sscanf(pcCmdStr+3,"%d",&bstep);
            debug_msg("%d\r\n",bstep);
            
            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC|FOCUS_BACKWORD, bstep);
            //pMotorTab->focus_setState(MOTOR_FOCUS_BWD, 256);
            #else
            debug_err(("lens focus bw\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_FOCUS);

            pLensTab->focus_gotoPosition(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, 0);
            //pMotorTab->focus_setState(MOTOR_FOCUS_BWD, 256);

            #endif
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        break;

    /* Aperture */
    case 'a':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "ab", 2))
        {
            debug_err(("lens aperture big\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_APERTURE);

            pLensTab->aperture_gotoState(LENS_APERTURE_BIG);
            TimerDelayMs(LENS_MAX_WAIT_TIME);
            pLensTab->aperture_gotoState(LENS_APERTURE_OFF);

            pMotorTab->changeMode(MOTOR_MODE_STANDBY);
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "as", 2))
        {
            debug_err(("lens aperture small\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_APERTURE);

            pLensTab->aperture_gotoState(LENS_APERTURE_SMALL);
            TimerDelayMs(LENS_MAX_WAIT_TIME);
            pLensTab->aperture_gotoState(LENS_APERTURE_OFF);

            pMotorTab->changeMode(MOTOR_MODE_STANDBY);
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }

        break;

    /* Shutter */
    case 's':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "so", 2))
        {
            debug_err(("lens shutter open\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_SHUTTER);

            pLensTab->shutter_gotoState(LENS_SHUTTER_OPEN);
            TimerDelayMs(LENS_MAX_WAIT_TIME);
            pLensTab->shutter_gotoState(LENS_SHUTTER_OFF);

            pMotorTab->changeMode(MOTOR_MODE_STANDBY);
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "sc", 2))
        {
            debug_err(("lens shutter close\r\n"));

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_SHUTTER);

            status = pLensTab->shutter_gotoState(LENS_SHUTTER_CLOSE);

            if(status != ERR_OK_TIMER_CLOSE_SH)
            {
                TimerDelayMs(LENS_MAX_WAIT_TIME);
                pLensTab->shutter_gotoState(LENS_SHUTTER_OFF);
                pMotorTab->changeMode(MOTOR_MODE_STANDBY);
                pMotorTab->power(MOTOR_POWER_OFF, 0);
            }
        }

        break;

    /* Zoom */
    case 'z':

        if(!bLensInit)
        {
            debug_err(("lens doesn't init\r\n"));
            return;
        }

        if (!strncmp(pcCmdStr, "zfw", 3))
        {
            debug_err(("lens zoom fw\r\n"));

            Lens_Zoom_EnableIsr(FALSE);

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_ZOOM);
            pMotorTab->zoom_setState(MOTOR_ZOOM_FWD, 300);
            TimerDelayMs(500);
            pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "zbw", 3))
        {
            debug_err(("lens zoom bw\r\n"));

            Lens_Zoom_EnableIsr(FALSE);

            pMotorTab->power(MOTOR_POWER_ON, 0);
            pMotorTab->changeMode(MOTOR_MODE_ZOOM);
            pMotorTab->zoom_setState(MOTOR_ZOOM_BWD, 300);
            TimerDelayMs(500);
            pMotorTab->zoom_setState(MOTOR_ZOOM_OFF, 0);
            pMotorTab->power(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "zinit1", 6))
        {
            Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
            Lens_Init(LENS_INIT_ZOOM_PART1);
        }
        else if (!strncmp(pcCmdStr, "zinit2", 6))
        {
            Lens_Init(LENS_INIT_ZOOM_PART2);
            Lens_OnOff(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "zre", 3))
        {
            Lens_OnOff(MOTOR_POWER_ON, MOTOR_POWERON_NORMAL);
            Lens_Retract(0);
            Lens_OnOff(MOTOR_POWER_OFF, 0);
        }
        else if (!strncmp(pcCmdStr, "zgo", 3))
        {
            UINT32 section;

            sscanf((pcCmdStr+4), "%d", &section);
            debug_err(("lens zoom goto = %d\r\n", section));

            Lens_Zoom_Goto(section);
        }
        break;
    }
}
