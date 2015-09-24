#include "SysCfg.h"

#if (_OSD_SCREEN_DUMP_ == ENABLE)
#include <stdio.h>
#include <string.h>
#include "OSDScreenDump.h"
#include "ide.h"
#include "FileSysTsk.h"
#include "OSCommon.h"
#include "UIFramework.h"
#include "UIGraphics.h"
#include "Utility.h"

#define BufferP0_offset     0
#define BufferP1_offset     0x4B000 //offset 300KB,for picuture Bitmap data
#define BufferP2_offset     0x96000 //offset 600KB,for OSD Bitmap data
#define BufferP3_offset     0xE1000 //offset 900KB,for Palette table
#define BufferP4_offset     0xE3800 //offset 910KB,for OSD buffer


__align(4)BMPStruct   g_BMPStruct;
__align(4)BMPHeader   g_BMPHeader={0};

static char     g_cDirName[]  = "A:\\OSD_DUMP\\";
static char     g_cFileName[] = "OSD_";
static char     g_cExtName[]  = "BMP";
static UINT32   g_uiMaxFileID = 0;

static void BMP_hwmemset(UINT32 uiDst, UINT8 ui8Value, UINT32 uiSize)
{
    hwmem_open();
    hwmem_memset(uiDst, ui8Value, uiSize);
    hwmem_close();
}

static void BMP_hwmemcpy(UINT32 uiDst, UINT32 uiSrc, UINT32 uiSize)
{
    hwmem_open();
    hwmem_memcpy(uiDst, uiSrc, uiSize);
    hwmem_close();
}

static void BMP_INITHeaer(BMPStruct *pgBMPStruct)
{
    // for BMP file header
    pgBMPStruct->BMP_Header.bmp_FileHeader.u16Identifier = 0x4D42; //'BM'
    //0x36=length(BMPHeader+BMPInfoHeader)
    // OSD_W*OSD_H = 0x12C00
    pgBMPStruct->BMP_Header.bmp_FileHeader.uiFileSize = 0x36+OSD_W*OSD_H*3; //24bit true color format
    pgBMPStruct->BMP_Header.bmp_FileHeader.uiReserved = 0x00;
    pgBMPStruct->BMP_Header.bmp_FileHeader.uiFileOffset = 0x36;//0x36,bitmap data offset

    // For BMP Info header
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiHeaderSize = 0x28;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiWidth = OSD_W;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiHeight = OSD_H;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.u16Planes = 1;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.u16BitsPerPix = 24;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiCompression = 0;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiDataSize = OSD_W*OSD_H*3; // 24bit true color
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiHResolution = 0x00;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiVResolution = 0x00;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiColors = 0;
    pgBMPStruct->BMP_Header.bmp_InfoHeader.uiImportantColors = 0;
}


static void BMP_ValidatePICT(BMPStruct *pgBMPStruct)
{
    UW  pos;

    // set g_BMPHeader buffer to 0
    memset((void *)&g_BMPHeader,0,sizeof(g_BMPHeader));

    // open pict file/load BMP Header info
    pos = sizeof(g_BMPHeader);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, pgBMPStruct->filename, (UINT8 *)&g_BMPHeader, &pos, 0, FST_TIME_INFINITE))
    {
        pgBMPStruct->bzPICTValid = FALSE;
        debug_msg("BMP Header reading, fail ...\r\n");
    }
    else
    {
        debug_msg("BMP Header reading, success ...\r\n");
    }

    //parse BMP file header
    if (g_BMPHeader.bmp_FileHeader.u16Identifier!=0x4D42)
    {
        debug_msg("wrong BMP ID\r\n");
        pgBMPStruct->bzPICTValid = FALSE;
    }

    // check Pict's width/height
    if (g_BMPHeader.bmp_InfoHeader.uiWidth!=OSD_W ||g_BMPHeader.bmp_InfoHeader.uiHeight!=OSD_H)
    {
        debug_msg("Wrong resolution,WxH=%dx%d\r\n",g_BMPHeader.bmp_InfoHeader.uiWidth,\
                   g_BMPHeader.bmp_InfoHeader.uiHeight);
        pgBMPStruct->bzPICTValid = FALSE;
    }

    // check bits per Pixel
    if (g_BMPHeader.bmp_InfoHeader.u16BitsPerPix != 24)
    {
        debug_msg("BPS=%d\r\n",g_BMPHeader.bmp_InfoHeader.u16BitsPerPix);
        pgBMPStruct->bzPICTValid = FALSE;
    }

    // check compression flag
    if (g_BMPHeader.bmp_InfoHeader.uiCompression != 0x00)
    {
        debug_msg("Compression=%d\r\n",g_BMPHeader.bmp_InfoHeader.uiCompression);
        pgBMPStruct->bzPICTValid = FALSE;
    }
}

static void BMP_GetPaletteTable(BMPStruct *pgBMPStruct)
{
    UINT32  uiIndex;
    INT32   Y,Cb,Cr,R,G,B,blend;
    UINT8   *pPaletteBufAddr;

    pPaletteBufAddr = (UINT8 *)pgBMPStruct->uiPaletteAddr;

    for (uiIndex=0;uiIndex<256;uiIndex++)
    {
        Y  = gDemoKit_Palette_Palette[uiIndex]&0xFF;
        Cb = (gDemoKit_Palette_Palette[uiIndex]>>8)&0xFF;
        Cr = (gDemoKit_Palette_Palette[uiIndex]>>16)&0xFF;
        blend = (gDemoKit_Palette_Palette[uiIndex]>>24)&0x0F;
        R = YUV_GET_R(Y,Cb,Cr);
        G = YUV_GET_G(Y,Cb,Cr);
        B = YUV_GET_B(Y,Cb,Cr);

        *(pPaletteBufAddr+uiIndex*4) = LIMIT(B,0,255);
        *(pPaletteBufAddr+uiIndex*4+1) = LIMIT(G,0,255);
        *(pPaletteBufAddr+uiIndex*4+2) = LIMIT(R,0,255);
        *(pPaletteBufAddr+uiIndex*4+3) = blend;
    }
}


static void  BMP_LoadPictData(BMPStruct *pgBMPStruct)
{
    UW      pos,Datasize;
    UINT32  uiBufferAddr;

    // read whole file
    pos = pgBMPStruct->BMP_Header.bmp_FileHeader.uiFileOffset;
    Datasize = pgBMPStruct->BMP_Header.bmp_InfoHeader.uiDataSize;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_READ_BY_NAME, pgBMPStruct->filename, (UINT8 *)(pgBMPStruct->uiPICTBuffAdr),\
            &Datasize, pos, FST_TIME_INFINITE))
    {
        pgBMPStruct->bzPICTValid = FALSE;
        debug_msg("Picture reading, fail ...\r\n");
    } else {
        debug_msg("Picture reading, success ...\r\n");
    }
}

static void BMP_CreateOSDBitmap(BMPStruct *pgBMPStruct)
{
    UINT32  uiBufAddr,i,j;
    UINT8   *pOSDBuf,*pOSDBitmapbufAddr,*pPaletteBufAddr;
    UINT8   uiColorindex;

    // get OSD buffer address
    ide_wait_frame_end();
    ide_get_osd_buf_addr(IDE_OSDID_1, &uiBufAddr);

    // read OSD buffer data
    pOSDBitmapbufAddr = (UINT8 *)pgBMPStruct->uiOSDBitmapBufAddr;
    pPaletteBufAddr = (UINT8 *)pgBMPStruct->uiPaletteAddr;
    //memcpy((void *)g_BMPStruct.uiOSDDataAddr, (void *)uiBufAddr, OSD_W*OSD_H);
    BMP_hwmemcpy((UINT32)g_BMPStruct.uiOSDDataAddr, (UINT32)uiBufAddr, OSD_W*OSD_H);
    pOSDBuf = (UINT8 *)g_BMPStruct.uiOSDDataAddr;

    // conversion from OSD palette data to 24bit True color data
    for (i=0;i<OSD_H;i++)
        for (j=0;j<OSD_W;j++)
        {
            uiColorindex = *(pOSDBuf+i*OSD_W+j);
            *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)   = *(pPaletteBufAddr+uiColorindex*4);     //B_component
            *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1) = *(pPaletteBufAddr+uiColorindex*4+1);   //G_component
            *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2) = *(pPaletteBufAddr+uiColorindex*4+2);   //R_component
        }
}


static void BMP_BitmapBlend(BMPStruct *pgBMPStruct)
{
    UINT8   *pOSDBitmapbufAddr,*pPaletteBufAddr,*pPictBitmapbufAddr;
    UINT8   *pBMPBufAddr,*pOSDBuf,uiColorindex;
    UINT32  uiBufAddr,i,j;
    INT32   R,G,B,blend;


    pOSDBuf = (UINT8 *)g_BMPStruct.uiOSDDataAddr;
    pBMPBufAddr = (UINT8 *)(pgBMPStruct->uiBMPBuffAdr+sizeof(BMPHeader));
    pOSDBitmapbufAddr = (UINT8 *)pgBMPStruct->uiOSDBitmapBufAddr;
    pPaletteBufAddr = (UINT8 *)pgBMPStruct->uiPaletteAddr;
    pPictBitmapbufAddr = (UINT8 *)pgBMPStruct->uiPICTBuffAdr;

    for (i=0;i<OSD_H;i++)
        for (j=0;j<OSD_W;j++)
        {
            uiColorindex = *(pOSDBuf+i*OSD_W+j);
            blend = *(pPaletteBufAddr+uiColorindex*4+3);
            switch(blend) {
                case BLEND_OPAQUE_000:
                    B = *(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3);
                    G = *(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1);
                    R = *(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2);
                    break;
                case BLEND_OPAQUE_012:
                    B = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)*7+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3))>>3;
                    G = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)*7+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1))>>3;
                    R = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)*7+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2))>>3;
                    break;
                case BLEND_OPAQUE_025:
                    B = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)*3+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3))>>2;
                    G = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)*3+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1))>>2;
                    R = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)*3+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2))>>2;
                    break;
                case BLEND_OPAQUE_050:
                    B = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3))>>1;
                    G = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1))>>1;
                    R = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2))>>1;
                    break;
                case BLEND_OPAQUE_075:
                    B = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)*3)>>2;
                    G = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)*3)>>2;
                    R = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)*3)>>2;
                    break;
                case BLEND_OPAQUE_088:
                    B = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3)*7)>>3;
                    G = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1)*7)>>3;
                    R = (*(pPictBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)+*(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2)*7)>>3;
                    break;
                case BLEND_OPAQUE_100:
                    B = *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3);
                    G = *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1);
                    R = *(pOSDBitmapbufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2);
                    break;
            }

            *(pBMPBufAddr+(OSD_H-i-1)*OSD_W*3+j*3)   = B;   //B_component
            *(pBMPBufAddr+(OSD_H-i-1)*OSD_W*3+j*3+1) = G;   //G_component
            *(pBMPBufAddr+(OSD_H-i-1)*OSD_W*3+j*3+2) = R;   //R_component
        }
}

static void BMP_CreateBMPFile(BMPStruct *pgBMPStruct)
{
    UW      DataSize;
    UINT32  uiPoolAddr,uiOSDBmpBufAddr,index;
    char    filename[Filename_Len];

    // create new filename
    sprintf(filename, "%s%s%04d.bmp", g_cDirName, g_cFileName, g_uiMaxFileID);

    uiPoolAddr = (UINT32)pgBMPStruct->uiBMPBuffAdr;
    uiOSDBmpBufAddr = (UINT32)pgBMPStruct->uiOSDBitmapBufAddr;
    DataSize = sizeof(g_BMPHeader);
    // copy BMP header(FileHeader+InfoHeader) to file buffer
    memcpy((void *)uiPoolAddr, (void *)&(pgBMPStruct->BMP_Header), DataSize);
    //BMP_hwmemcpy((UINT32)uiPoolAddr, (UINT32)&(pgBMPStruct->BMP_Header), DataSize);
    DataSize += OSD_W*OSD_H*3;

    // make new bmp file
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    if(FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, filename, (UINT8 *)uiPoolAddr, &DataSize, 0, FST_TIME_INFINITE))
    {
        debug_msg("Bin file writing, fail ...\r\n");
    }
    else
    {
        g_uiMaxFileID++;
        debug_msg("Bin file writing, success ...\r\n");
    }
}

static void Filesys_dirCB(FIND_DATA *findDir, BOOL *bContinue, UINT16 *cLongname, UINT32 Param)
{
    char    cFileName[9] = {0}; // 8.3 format
    char    cExtName[4] = {0};
    UINT32  uiFileID;

    memcpy(cFileName, findDir->FATMainName, 8);
    memcpy(cExtName, findDir->FATExtName, 3);
    //debug_err(("[Filesys_dirCB]: findDir->FATMainName = %s\r\n", cFileName));
    //debug_err(("[Filesys_dirCB]: findDir->FATExtName = %s\r\n", cExtName));

    if (!strncmp(cFileName, g_cFileName, 4) && !strncmp(cExtName, g_cExtName, 3))
    {
        uiFileID = atoi(cFileName + 4);
        if (uiFileID > g_uiMaxFileID)
            g_uiMaxFileID = uiFileID;
    }
}

void BMP_MakeFile(char *pFilename)
{
    UINT32  uiPoolAddr;
    static  BOOL bInit = TRUE;

    if (bInit)
    {
        // Parsing maximum file ID in specific folder
        FilesysDirDirectory(g_cDirName, Filesys_dirCB, FALSE);
        FilesysWaitCmdFinish(FST_TIME_INFINITE);
        g_uiMaxFileID++; // new file ID should be increased by 1
        bInit = FALSE;
    }

    // init buffer address
    get_blk((VP*)&uiPoolAddr, POOL_ID_OSD_DUMP);
    rel_blk(POOL_ID_OSD_DUMP, (VP)uiPoolAddr);

    g_BMPStruct.uiBMPBuffAdr        = uiPoolAddr;
    g_BMPStruct.uiPICTBuffAdr       = uiPoolAddr+BufferP1_offset;
    g_BMPStruct.uiOSDBitmapBufAddr  = uiPoolAddr+BufferP2_offset;
    g_BMPStruct.uiPaletteAddr       = uiPoolAddr+BufferP3_offset;
    g_BMPStruct.uiOSDDataAddr       = uiPoolAddr+BufferP4_offset;
    sprintf(g_BMPStruct.filename,"A:\\%s",pFilename);
    g_BMPStruct.bzPICTValid = TRUE;

    // validate Pict file
    BMP_ValidatePICT(&g_BMPStruct);

    // Init BMP header
    BMP_INITHeaer(&g_BMPStruct);

    // Load Palette data
    BMP_GetPaletteTable(&g_BMPStruct);

    // Load Pict Bitmap data
    if (g_BMPStruct.bzPICTValid==TRUE)
    {
        // Load Pict Bitmap data
        BMP_LoadPictData(&g_BMPStruct);
    }

    // Create OSD Bitmap data
    BMP_CreateOSDBitmap(&g_BMPStruct);

    // Combine image/osd Bitmap data
    if (g_BMPStruct.bzPICTValid==TRUE)
    {
        BMP_BitmapBlend(&g_BMPStruct);
    } else {
        // copy OSD Bitmap data into BMP Bitmap data buffer directly
        // only show OSD Bitmap data
        //memcpy((void *)(g_BMPStruct.uiBMPBuffAdr+sizeof(g_BMPHeader)), (void *)(g_BMPStruct.uiOSDBitmapBufAddr), OSD_W*OSD_H*3);
        BMP_hwmemcpy((UINT32)(g_BMPStruct.uiBMPBuffAdr+sizeof(g_BMPHeader)), (UINT32)(g_BMPStruct.uiOSDBitmapBufAddr), OSD_W*OSD_H*3);
    }

    // create BMP file
    BMP_CreateBMPFile(&g_BMPStruct);
}

#endif


