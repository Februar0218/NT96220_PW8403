
#include "stdio.h"
#include "Gsensor.h"
#include "spi.h"
#include "GsensorNone.h"


void gsensor_getFP(PGSENSOR_FUNC pGsensorFunc)
{
    memcpy(pGsensorFunc,&GsensorNoneFP,sizeof(GSENSOR_FUNC));
}

#if 0
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

static void LIS331DL_PowerOn(void)
{
 UINT32 buf[2];

    buf[0]=CTRL_REG1;
    buf[1]=0x47;
    LIS331DL_Write2Byte(buf);
}

static UINT32 LIS331DL_GetDev(void)
{
 UINT32 buf[2];

    buf[0]=WHO_AM_I;
    LIS331DL_Write1BRead1B(buf);
    debug_msg("dev:%x\r\n",buf[1]);
    return (UINT32)buf[1];
}
#endif

BOOL GsensorNone_GsensorInit(void)
{
    return TRUE;
}

void GsensorNone_GetGsensorData(Gsensor_Data *GS_Data)
{

}

void GsensorNone_GetGsensorStatus(UINT32 status,UINT32 *uiData)
{

}

void GsensorNone_OpenInterface(void)
{

}

void GsensorNone_CloseInterface(void)
{

}


GSENSOR_FUNC GsensorNoneFP = {
    GsensorNone_GsensorInit,        // GsensorInit
    GsensorNone_GetGsensorData,     // GetGsensorData
    GsensorNone_GetGsensorStatus,   // GetGsensorStatus
    GsensorNone_OpenInterface,      // OpenInterface
    GsensorNone_CloseInterface,     // closeInterface
};


