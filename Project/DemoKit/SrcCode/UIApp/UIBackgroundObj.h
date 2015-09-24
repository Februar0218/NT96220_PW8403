#ifndef UI_EXECUTE_FUNC_H
#define UI_EXECUTE_FUNC_H

#include "UIFramework.h"

// UI background obj return status
typedef enum
{
    UIBKW_STA_OK = 0,
    UIBKW_STA_CARDFULL,
    UIBKW_STA_MAX
} UIBKW_STATUS;

extern BACKGROUND_JOB_ENTRY gBackgroundExtFuncTable[];
#endif

