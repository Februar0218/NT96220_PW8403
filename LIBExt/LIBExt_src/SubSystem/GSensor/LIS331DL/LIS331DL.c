
#include "stdio.h"
#include "Gsensor.h"
#include "spi.h"
#include "LIS331DL.h"

extern void DelayMs(UINT32 ms);

void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc)
{
    memcpy(pGsensorFunc,&LIS331DLFP,sizeof(GSENSOR_FUNC));
}


static void LIS331DL_Write2Byte(UINT32 *buf)
{
    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(buf[0]& ~0x80);
    spi_writeSingle(buf[1]);
    spi_setCSActive(FALSE);
}

static void LIS331DL_Write1BRead1B(UINT32 *buf)
{
    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_writeSingle(buf[0]|0x80);
    spi_writeReadSingle(0,&buf[1]);
    spi_setCSActive(FALSE);
}

static void LIS331DL_Read2B(UINT32 *buf)
{
    spi_setCSActive(TRUE);
    spi_setTransferLen(SPI_TRANSFER_LEN_1BYTE);
    spi_readSingle(&buf[0]);
    spi_readSingle(&buf[1]);
    spi_setCSActive(FALSE);
}

static void LIS331DL_PowerOn(void)
{
    UINT32 buf[2];
#if 0

    buf[0]= CTRL_REG1;
    buf[1]= PD | FS | ZEN | YEN | XEN;
    LIS331DL_Write2Byte(buf);

	buf[0]= CTRL_REG2;
	buf[1]= BOOT | FDS | HP_FF_WU1 | HP_coeff1;
	LIS331DL_Write2Byte(buf);

	buf[0]= CTRL_REG3;
	buf[1]= PP_OD | I1CFG1 | I1CFG0;
	LIS331DL_Write2Byte(buf);

	buf[0]= FF_WU_CFG_1;
	buf[1]= 0x6a;
	LIS331DL_Write2Byte(buf);

	buf[0]= FF_WU_THS_1;
	buf[1]= 0x03;
	LIS331DL_Write2Byte(buf);

	buf[0]= FF_WU_DURATION_1;
	buf[1]= 0x8f;
	LIS331DL_Write2Byte(buf);
#else
    buf[0]=CTRL_REG1;
    buf[1]=0x47;
    LIS331DL_Write2Byte(buf);
#endif

}

void GSensorSensitivity(UINT32 level)
{
	UINT32 buf[2];

    switch(level)
    {
    case 0:
		buf[0]= FF_WU_CFG_1;
		buf[1]= 0x6a;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_THS_1;
		buf[1]= 0x05;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_DURATION_1;
		buf[1]= 0x8f;
		LIS331DL_Write2Byte(buf);

        break;
    case 1:
		buf[0]= FF_WU_CFG_1;
		buf[1]= 0x6a;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_THS_1;
		buf[1]= 0x03;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_DURATION_1;
		buf[1]= 0x8f;
		LIS331DL_Write2Byte(buf);

        break;
    case 2:
		buf[0]= FF_WU_CFG_1;
		buf[1]= 0x6a;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_THS_1;
		buf[1]= 0x01;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_DURATION_1;
		buf[1]= 0x8f;
		LIS331DL_Write2Byte(buf);

        break;
	case 3:
		buf[0]= FF_WU_CFG_1;
		buf[1]= 0x6a;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_THS_1;
		buf[1]= 0x01;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_DURATION_1;
		buf[1]= 0x8f;
		LIS331DL_Write2Byte(buf);

		break;

    default:
		buf[0]= FF_WU_CFG_1;
		buf[1]= 0x6a;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_THS_1;
		buf[1]= 0x03;
		LIS331DL_Write2Byte(buf);
		
		buf[0]= FF_WU_DURATION_1;
		buf[1]= 0x8f;
		LIS331DL_Write2Byte(buf);

        break;
    }

}

static UINT32 LIS331DL_GetDev(void)
{
 UINT32 buf[2];

    buf[0]=WHO_AM_I;
    LIS331DL_Write1BRead1B(buf);
    debug_msg("dev:%x\r\n",buf[1]);
    return (UINT32)buf[1];
}

static void LIS331DL_Init(void)
{
 UINT32 buf[2];

#if 0
    buf[0] = CTRL_REG2;         buf[1] = 0x5e;  LIS331DL_Write2Byte(buf);
    buf[0] = CTRL_REG3;         buf[1] = 0x63;  LIS331DL_Write2Byte(buf);
    buf[0] = FF_WU_CFG_1;       buf[1] = 0x6a;  LIS331DL_Write2Byte(buf);
    buf[0] = FF_WU_THS_1;       buf[1] = 0x0f;  LIS331DL_Write2Byte(buf);
    buf[0] = FF_WU_DURATION_1;  buf[1] = 0x8f;  LIS331DL_Write2Byte(buf);
#endif
}

static UINT32 LIS331DL_GetStatus(void)
{
 UINT32 buf[2];

    buf[0]=STATUS_REG;
    LIS331DL_Write1BRead1B(buf);
    return buf[1];
}

void LIS331DL_WriteData(UINT32 *buf)
{
    debug_msg("%s,%x,%x\r\n",__func__,buf[0],buf[1]);
    LIS331DL_Write2Byte(buf);
}

void LIS331DL_ReadData(UINT32 *buf)
{
    LIS331DL_Write1BRead1B(buf);
    debug_msg("%s,%x,%x\r\n",__func__,buf[0],buf[1]);
}

BOOL LIS331DL_GsensorInit(void)
{
 UINT32 i, erReturn = 0;


    // LIS331DL power on
    LIS331DL_PowerOn();
    DelayMs(100);

    // Get LIS331DL device ID
    erReturn = LIS331DL_GetDev();
    if (erReturn != LIS331DL_Dev)
    {
        debug_msg("LIS331DL Dev:%x\r\n",erReturn);
        return FALSE;
    }

    // LIS331DL init
    LIS331DL_Init();

    return TRUE;
}

static INT32 LIS331DL_GetAxisMg(UINT32 uiInt)
{
  UINT32 buf[2];
  INT32 mgStep;
  BOOL  bzNegative=FALSE;

    buf[0]=CTRL_REG1;
    LIS331DL_Write1BRead1B(buf);
    // please refer table 2(page 8) in LIS33DE spec
    if (buf[1]&CTRL_FS)
        mgStep = 72;
    else
        mgStep = 18;

    if (uiInt&0x80)
    {
      // negative number
      uiInt = ~uiInt;
      uiInt++;
      bzNegative = TRUE;
    }

    return (bzNegative)?(-1*uiInt*mgStep):(uiInt*mgStep);
}

void LIS331DL_GetGsensorData(Gsensor_Data *GS_Data)
{
 UINT32 uiValue;
 UINT32 buf[2];

    uiValue=LIS331DL_GetStatus();
    if (uiValue&(STATUS_XDA|STATUS_ZYXDA))
    {
        buf[0]=OUT_X;
        LIS331DL_Write1BRead1B(buf);
        GS_Data->Axis.Xacc = LIS331DL_GetAxisMg(buf[1]);
    }
    if (uiValue&(STATUS_YDA|STATUS_ZYXDA))
    {
        buf[0]=OUT_Y;
        LIS331DL_Write1BRead1B(buf);
        GS_Data->Axis.Yacc = LIS331DL_GetAxisMg(buf[1]);
    }
    if (uiValue&(STATUS_ZDA|STATUS_ZYXDA))
    {
        buf[0]=OUT_Z;
        LIS331DL_Write1BRead1B(buf);
        GS_Data->Axis.Zacc = LIS331DL_GetAxisMg(buf[1]);
    }
}

void LIS331DL_GetGsensorStatus(UINT32 status,UINT32 *uiData)
{
    *uiData = LIS331DL_GetStatus();
}

static BOOL bPreSpiOpen = FALSE;

void LIS331DL_OpenInterface(void)
{
  SPI_INIT_INFO spiConfig;
  ER uiRet;

        bPreSpiOpen = spi_isOpened();

        // close spi interface for storage pinmux
        if (bPreSpiOpen == TRUE)
        {
            uiRet = spi_close();
            if (uiRet != E_OK)
            {
                debug_msg("%s, spi close failed\r\n",__func__);
                return;
            }
        }
        else
        {
            // may be SDIO is opened, handshake with file system...

        }
        // set spi interface from SPI_PINMUX_STORAGE to SPI_PINMUX_SERIAL
        spiConfig.bCSActiveLow = TRUE;
        spiConfig.bLSB = FALSE;
        spiConfig.bMasterMode = TRUE;
        spiConfig.pinmuxChannel = SPI_PINMUX_SERIAL;
        spiConfig.spiMODE = SPI_MODE_0;
        spiConfig.uiClkDiv = 1;
        uiRet = spi_init(&spiConfig);
        if (uiRet != E_OK)
        {
            debug_msg("%s, spi init failed\r\n",__func__);
            return;
        }
        // open spi interface for SPI_PINMUX_SERIAL
        uiRet = spi_open();
        if (uiRet != E_OK)
        {
            debug_msg("%s, spi open failed\r\n",__func__);
            return;
        }
}

void LIS331DL_CloseInterface(void)
{
  SPI_INIT_INFO spiConfig;
  ER uiRet;

        // close spi interface for serial pinmux
        uiRet = spi_close();
        if (uiRet != E_OK)
        {
            debug_msg("%s, spi close failed\r\n",__func__);
            return;
        }
        // set spi interface from SPI_PINMUX_SERIAL to SPI_PINMUX_STORAGE
        spiConfig.bCSActiveLow = TRUE;
        spiConfig.bLSB = FALSE;
        spiConfig.bMasterMode = TRUE;
        spiConfig.pinmuxChannel = SPI_PINMUX_STORAGE;
        spiConfig.spiMODE = SPI_MODE_0;
        spiConfig.uiClkDiv = 1;
        uiRet = spi_init(&spiConfig);
        if (uiRet != E_OK)
        {
            debug_msg("%s, spi init failed\r\n",__func__);
            return;
        }
        // open spi interface for SPI_PINMUX_STORAGE

        if (bPreSpiOpen == TRUE)
        {
            uiRet = spi_open();
            if (uiRet != E_OK)
            {
                debug_msg("%s, spi open failed\r\n",__func__);
                return;
            }
        }

}


GSENSOR_FUNC LIS331DLFP = {
    LIS331DL_GsensorInit,        // GsensorInit
    LIS331DL_GetGsensorData,     // GetGsensorData
    LIS331DL_GetGsensorStatus,   // GetGsensorStatus
    LIS331DL_OpenInterface,      // OpenInterface
    LIS331DL_CloseInterface,     // closeInterface
};


