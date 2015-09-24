#ifndef   _AUTOTEST_H
#define   _AUTOTEST_H

#include "Debug.h"
#include "Kernel.h"
#include "ide.h"
#include "OSCommon.h"

//Flow information Green
#define AUTO_FLW_LOG(...)               debug_msg("^G\033[1m"__VA_ARGS__)
#define AUTO_RST_LOG(...)               debug_msg("^Y\033[1m"__VA_ARGS__)
#define AUTO_ERR_LOG(...)               debug_msg("^R\033[1m"__VA_ARGS__)
#define AUTO_DES_LOG(...)               debug_msg("^C\033[1m"__VA_ARGS__)
#define AUTO_INF_LOG(...)               debug_msg("^M\033[1m"__VA_ARGS__)
#define AUTO_DBG_LOG(...)               debug_msg("^G\033[1m"__VA_ARGS__)

#define FW_DRAM_TEST_LOOP_OFFSET        (0x50 + 0x48)
#define FW_DRAM_DLYCHAIN_VALUE_OFFSET   (0x50 + 0x4C)

#define DRAM_TESTING_FLAG               0xA5            ///< Dram test alive flag
#define DRAM_DEADING_FLAG               0x5A            ///< Dram test dead  flag

#define UPPER_BOUNDARY                  0x00000010  //bit[4]
#define LOWER_BOUNDARY                  0x00000020  //bit[5]
#define UPPER_BOUNDARY_DONE             0x00000040  //bit[6]
#define LOWER_BOUNDARY_DONE             0x00000080  //bit[7]



/* AUTO test pattern information */
typedef enum
{
    DRAM_TEST_DQS_IN = 1,                   ///< scan DQS IN delay range
    DRAM_TEST_DQ_OUT,                       ///< scan DQ OUT delay range
    DRAM_TEST_CLK_INOUT,                    ///< scan clock in/out range

    DRAM_TEST_CANCEL,

    DRAM_TEST_TOTAL_PATTERN,
    ENUM_DUMMY4WORD(DRAM_TEST_PATTERN)
} DRAM_TEST_PATTERN;


/* AUTO test pattern information */
typedef enum
{
    CLK_TEST_PATT_IPP,                       ///< Image pipeline with path 2 HD size
    CLK_TEST_PATT_GRPH_IME_IDE,              ///< Graphic rotate + IDE scaling + IME scaling

    CLK_TEST_TOTAL_PATT,
    ENUM_DUMMY4WORD(CLOCK_TEST_PATTERN)
} CLOCK_TEST_PATTERN;

/* AUTO test pattern information */
typedef enum
{
    CLK_TEST_FIND_LOWER_BOUNDARY,            ///< Image pipeline with path 2 HD size
    CLK_TEST_FIND_UPPER_BOUNDARY,            ///< Graphic rotate + IDE scaling + IME scaling

    CLK_TEST_FIND_TOTAL_BOUNDARY_TYPE,
    ENUM_DUMMY4WORD(CLOCK_TEST_TYPE)
} CLOCK_TEST_TYPE;

/* AUTO test pattern information */
typedef enum
{
    DLYCHAIN_TEST_PATT_GRPH_180 = 1,            ///< Image pipeline with path 2 HD size

    TEST_TOTAL_DLYCHAIN_PATT,
    ENUM_DUMMY4WORD(DELAYCHAIN_TEST_PATTERN)
} DELAYCHAIN_TEST_PATTERN;

typedef enum
{
    TEST_FOREVER = 0x00,
    TEST_30_MIN,            ///< 30 min
    TEST_60_MIN,            ///< 1 hour
    TEST_180_MIN,           ///< 3 hour
    TEST_TIME_TOTAL,

    ENUM_DUMMY4WORD(BURNIN_TIME)
} BURNIN_TIME;

typedef enum
{
    UPPER_BOUND_STOP = 0x00,
    UPPER_BOUND_NOT_FOUND = UPPER_BOUND_STOP,

    UPPER_BOUND_ONGOING,
    UPPER_BOUND_FOUND = UPPER_BOUND_ONGOING,

    ENUM_DUMMY4WORD(UPPER_BOUND_ACTION)
}UPPER_BOUND_ACTION;

typedef enum
{
    LOWER_BOUND_STOP = 0x00,
    LOWER_BOUND_NOT_FOUND = LOWER_BOUND_STOP,
    LOWER_BOUND_ONGOING,
    LOWER_BOUND_FOUND = LOWER_BOUND_ONGOING,

    ENUM_DUMMY4WORD(LOWER_BOUND_ACTION)
}LOWER_BOUND_ACTION;

typedef enum
{
    BOUNDARY_NONE = 0x00,
    BOUNDARY_UPPER,
    BOUNDARY_LOWER,

    TOTAL_BOUNDARY,
    ENUM_DUMMY4WORD(BOUNDARY_TYPE)
} BOUNDARY_TYPE;

typedef enum
{
    ERR_OCCURRED_LOWER_FIRST = 0x00,
    ERR_OCCURRED_UPPER_FIRST,

    TOTAL_ERR_OCCURRED_TYPE,
    ERR_OCCURRED_CANCALATION,
    ENUM_DUMMY4WORD(ERROR_OCCURRED_TYPE)
} ERROR_OCCURRED_TYPE;

typedef enum
{
    ENGINEER_MODE_DISABLE = 0x00,
    ENGINEER_MODE_ENABLED,

    TOTAL_ENGINEER_MODE_TYPE,
    ENUM_DUMMY4WORD(ENGINEER_EN)
}ENGINEER_EN;

typedef enum
{
    ERR_CAUSE_LOWER_ERROR_UPPER_ERROR = 0x00,
    ERR_CAUSE_LOWER_ERROR_UPPER_CRASH,
    ERR_CAUSE_CRASH_ERROR_UPPER_ERROR,
    ERR_CAUSE_CRASH_ERROR_UPPER_CRASH,

    TOTAL_ERROR_CAUSE_TYPE,
    ENUM_DUMMY4WORD(ERROR_CAUSE_TYPE)
} ERROR_CAUSE_TYPE;

typedef enum
{
    CLK_DLY_LOWER_BOUND = 0x00,
    CLK_DLY_UPPER_BOUND,

    TOTAL_CLK_DLY_DIRECTION,
    ENUM_DUMMY4WORD(CLK_DLY_DIR)
} CLK_DLY_DIR;

typedef enum
{
    ODT_OFF = 0x00,
    ODT_ON,

    TOTAL_ODT_CFG,
    ENUM_DUMMY4WORD(ODT_PARAM)
} ODT_PARAM;


#define DMA_GETDQSINOUTTOTALREG(param, m)   (m == DRAM_TEST_DQS_IN)? \
                                                dma_getDQSInTotalReg(param):\
                                                dma_getDQOutTotalReg(param)

#define DMA_GETDQSINOUTDELAY(m)             (m == DRAM_TEST_DQS_IN)? \
                                                dma_getDQSInDelay():\
                                                dma_getDQOutDelay()


typedef ER            (* RD_CARD_SECTOR)(CHAR *, UINT32, UINT32);       // read sectors
typedef ER            (* WR_CARD_SECTOR)(CHAR *, UINT32, UINT32);       // write sectors
typedef ER            (* RD_CARD_TEST)(UINT32, UINT32, UINT32, BOOL);   // Read card data test

ER      GRPHPattern180Rotate(UINT32 uiPoolBuf, UINT32 bufSize);
ER      IDEScalingOpen(void);
void    idePat_SetIDEInfo(LCDSIZE * uiLCDSize, UINT32 uiFB, UINT32 uiTaskID);
void    autoTest_wdtReset(void);
void    autoTest_Desp(void);
void    rtcExt_setData0(UINT32 data0);
void    rtcExt_setData1(UINT32 data1);
UINT32  rtcExt_getData0(void);
UINT32  rtcExt_getData1(void);
ER      ReadCard_UpdateCardLoader(UINT32 value, UINT32 loop);
INT32   ReadCard_GetDelayChainFromLoader(void);
void    ReadCard_Init(RD_CARD_SECTOR pRdSectors, WR_CARD_SECTOR pWrSectors, UINT32 uiLDBuffer);
ER      autoTest_wdtStartup(void);
BOOL    autoTest_CheckFW(UINT32 uiAddress, UINT32 uiFileLen);
void    autiTest_SwitchIPLVideoMode(void);
ER      IDEScalingOpen(void);
ER      IDEScalingClose(void);
BOOL    autoTest_SetClk(UINT32 uiDLLDelay);
void    autoTest_PatternGenerate(UINT32 dstBuffer, UINT32 len);

// Auto test information get / set API
UINT32  auto_infoGetTestFlag(void);
void    auto_infoSetTestFlag(UINT32 flag);
void    auto_infoSetTestPattern(UINT32 PATT);
UINT32  auto_infoGetTestPattern(void);

#if 0
void    auto_infoSetUpperRange(UINT32 uiRange);
UINT32  auto_infoGetUpperRange(void);
void    auto_infoSetLowerRange(UINT32 uiRange);
UINT32  auto_infoGetLowerRange(void);
#endif
void    auto_infoSetUpperBoundOngoingBit(UINT32 uiUpper);
UINT32  auto_infoGetUpperBoundOngoingBit(void);
void    auto_infoSetLowerBoundOngoingBit(UINT32 uiLower);
UINT32  auto_infoGetlowerBoundOngoingBit(void);
void    auto_infoSetUpperBoundDoneBit(UINT32 uiUpperDone);
UINT32  auto_infoGetUpperBoundDoneBit(void);
void    auto_infoSetLowerBoundDoneBit(UINT32 uiLowerDone);
UINT32  auto_infoGetlowerBoundDoneBit(void);

void    auto_infoSetUpperBoundNextValue(UINT32 uiUpperBound);
UINT32  auto_infoGetUpperBoundNextValue(void);
void    auto_infoSetLowerBoundNextValue(UINT32 uiLowerBound);
UINT32  auto_infoGetLowerBoundNextValue(void);

void    auto_infoSetLoopCnt(UINT32 uiLoop);
UINT32  auto_infoGetLoopCnt(void);
void    auto_infoSetTimeInterval(UINT32 uiInterval);
UINT32  auto_infoGetTimeInterval(void);

UINT32  auto_infoGetNextDelayChainRegValue(void);
void    auto_infoSetOriginalValue(UINT32 uiOrgValue);
UINT32  auto_infoGetOriginalValue(void);

ER      autoTest_wdtClose(void);

UINT32  autoTest_TestParamSelection(void);

void    auto_infoSetErrorOccurredPriority(UINT32 uiErrPriority);
UINT32  auto_infoGetErrorOccurredPriority(void);
void    auto_infoSetErrorOccurredType(UINT32 uiErrCause);
UINT32  auto_infoGetErrorOccurredType(void);

void    auto_infoSetEngineerEN(UINT32 uiEN);
UINT32  auto_infoGetEngineerEN(void);

// Clock delay usage
void    auto_infoSetClkOriginalValue(UINT32 uiOrgValue);
UINT32  auto_infoGetClkOriginalValue(void);
void    auto_infoSetClkSearchDirection(UINT32 uiDir);
UINT32  auto_infoGetClkSearchDirection(void);

void    auto_infoSetClkUpperBoundNextValue(UINT32 uiUpperBound);
UINT32  auto_infoGetClkUpperBoundNextValue(void);
void    auto_infoSetClkLowerBoundNextValue(UINT32 uiLowerBound);
UINT32  auto_infoGetClkLowerBoundNextValue(void);
void    auto_infoSetClkTestPattern(UINT32 PATT);
UINT32  auto_infoGetClkTestPattern(void);

void    auto_infoSetClkTestTimeInterval(UINT32 uiIval);
UINT32  auto_infoGetClkTestTimeInterval(void);

void    auto_infoSetClkUpperBoundDoneBit(UINT32 uiUpperDone);
UINT32  auto_infoGetClkUpperBoundDoneBit(void);
void    auto_infoSetClkLowerBoundDoneBit(UINT32 uiLowerDone);
UINT32  auto_infoGetClklowerBoundDoneBit(void);

void    auto_infoSetODTEN(BOOL uiEN);
UINT32  auto_infoGetODTEN(void);


UINT32  autoTest_TestSearchRangeType(void);
UINT32  autoTest_TestTimeInterval(void);

void    autoTest_SelectODTEN(void);
ER      TIMERCountOpen(UINT32 taskID, UINT32 timerCnt, UINT32 uiTestLoop);
ER      TIMERCountClose(UINT32 taskID);

void    auto_infoSetDQS0_ODTEN(BOOL uiEN);
UINT32  auto_infoGetDQS0_ODTEN(void);
void    auto_infoSetDQS1_ODTEN(BOOL uiEN);
UINT32  auto_infoGetDQS1_ODTEN(void);

#endif
