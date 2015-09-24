/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CalibrationTbl.c
    @ingroup    mIPRJAPTest

    @brief      Calibration item table
                Calibration item table

    @note       Nothing.

    @date       2006/01/02
*/

/** \addtogroup mIPRJAPTest */
//@{

#include "CalibrationInt.h"
#include "CalibrationTsk.h"
#include "CalBurnIn.h"
#include "CalLCD.h"
#include "CalCIPA.h"
#include "CalMISC.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "KeyScanTsk.h"
#include "Debug.h"
#include "CalSensor.h"
#include "CalLens.h"
#include "UIConfig.h"

#if (_CALIBRATION_MODE_ == ENABLE)

// -------------------------------------------------------------------
// Internal Global variables
// -------------------------------------------------------------------
// Declare all global variables used in Calibration task here.
// Don't reference these variables in your code.

void Cal_Test_1(void)
{
    FLGPTN  uiFlag = 0;
    RECT    StringRect = {20, 100, 300, 30};

    UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
    Cal_ShowString("Press UP key to exit", &StringRect, TRUE);

    while (1)
    {
        wai_flg(&uiFlag, FLG_ID_KEY, FLGKEY_UP, TWF_ORW | TWF_CLR);
        break;
    }
}

// Calibration item table, please keep at least one item to make calibration task
// works correctly
CAL_TERM g_CalItem[]    =
{
    // Display Name           Function pointer
    {"Format SPI",            Cal_FormatPstoreSPI },
    {"Defect Pixel",          Cal_DefectPixel     },
    {"ECS",                   Cal_ECS             },
    {"AWB",                   Cal_AWB             },
    {"WriteAWBGS",            Cal_WriteAWBGS      },
    {"Cal Status",            Cal_ReadCalStatus   },
#if (_LENSLIB_ != _LENSLIB_FF_)
//    {"AF Near",             Cal_SenAFNear       },
//    {"AF Far",              Cal_SenAFFar        },
#endif
#if (UPDATE_CFG == UPDATE_CFG_YES)
    {"Update Logo",               UI_UpdateLogoFile },
    {"Update Cfg",                UI_UpdateCfgFile   },
#endif
    {"Read NorFlash",             UI_ReadNorFlash   },
    //{"CCD",                     Cal_DefectPixel           },
    //{"EV",                      Cal_AWB_Mshutter },
    //{"Burn In",                 Cal_BurnInMenu    },
    //{"Read Cal. Status",        Cal_ReadCalStatus   },
    //{"Set to Default",          Cal_SetDefault      },
    //{"MshutterReliability",     Cal_MshutterReliability},
    //{"Del CAL",                 Cal_DeleteCal    },
    //{"Flash Lasting",           Cal_BurnInFlash     },
    //{"168H",                    Cal_BurnIn168H      },
    //{"AWB",                     Cal_AWB          },
    //{"M Shutter",               Cal_MShutter     },
    //{"Check Info",              Cal_Write2File   },
    //{"Run CCD",                 Cal_DP_AWB_Mshutter },
    //{"168H",                    Cal_BurnIn168H      },
    //{"M Shutter Lasting",       Cal_MShutterLasting },
    //{"10Min BurnIn",            Cal_BurnIn10min     },
    //{"AWBSG",                   Cal_AWBSG        },
    //{"CheckNand",               Cal_CheckNand       },
    //{"Focus adjust",            Cal_VACC             },

    //{"reset AAA",               Cal_Reset_Adjust},
    //{"Read Status",             Cal_NoFunction      },
    //{"CIPA",                    Cal_CIPA            },
    //{"ISO",                     Cal_ISO          },
    //{"Flash",                   Cal_Flash        },
    //{"AF Far",                  Cal_AFFar        },
    //{"AF Near",                 Cal_AFNear       },
    //{"LENS",                    Cal_LENS         },
    //{"Zoom/AF Lasting",         Cal_BurnInZoomAF    },
    //{"LCD",                     Cal_LCDVerify       },
};

UINT32 g_CalMaxItemNum = sizeof(g_CalItem) / sizeof(CAL_TERM) - 1;

UINT32 Cal_GetMaxItemNum(void)
{
    return g_CalMaxItemNum;
}

#endif

//@}
