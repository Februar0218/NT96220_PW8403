/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       CommandTsk.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Kernel.h"
#include "SysCfg.h"
#include "Top.h"
#include "UART.h"
#include "CommandTsk.h"
#include "FileSysTsk.h"
#include "PrimaryTsk.h"
#include "KeyScanTsk.h"
#include "SystemInfoGet.h"
#include "KerVerInfo.h"
#include "AppVerInfo.h"
#include "Driver.h"
#include "ImageAlgInfor.h"
#include "ippbuffer.h"
#include "Alg_IPLMode.h"
#include "ReadNand.h"
#include "Nand.h"
#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "UIFlow.h"
#include "Sta.h"
#include "SystemClock.h"
#include "SieCapTsk.h"
#include "PhotoTsk.h"
#include "PlaybackTsk.h"
#include "AviRecApi.h"
#include "SystemInit.h"
#include "PStore.h"
#include "PSDrv.h"
#include "ide.h"
#include "GlobalVar.h"
#include "UserPStore.h"
#include "GPIOMapping.h"
#include "ADC.h"
#include "PWM.h"
#include "Audio.h"
#include "IDE.h"
#include "fs_file_op.h"
#include "Utility.h"
//#include "SDMMC.h"
#include "SDIO.h"
#include "BuildDateInfo.h"
#include "Lens.h"
//#include "AF_Lib.h"
#include "Gesture_Public.h"
#include "mediaplayapi.h"
#include "UserPhotoFrame.h"
#include "PhotoFrame.h"
#include "pll.h"
#include "clock.h"
#include "MovieInterface.h"
#include "dma.h"
#include "CalibrationTsk.h"
#include "CalibrationInt.h"
#include "driver.h"
#include "dispdev_ioctrl.h"


#if (_IQ_SIM_ == ENABLE)
#include "ImageQualitySim.h"
#endif

#if (_COMMANDTSK_MODE_ == ENABLE)

#define CmdTsk_OutputMsg(msg)           debug_msg msg
#define debug_print(msg)                debug_msg msg
//#define debug_print(msg)

#define _UART_MONITOR_CMD_SIZE          64
#define COMMAND_ERROR                   "bad command or invalid parameters\r\n"

//----- Static funcs & variables define here  ------------------
void CmdTsk_DoCommand(CHAR *pcCmdStr);
void CmdTsk_ShowClk(void);
static void CmdTsk_ReadMemory(CHAR *para_str);
static void CmdTsk_WriteMemory(CHAR *para_str);

//----- External funcs & variables declaration here  ------------------
extern void     sen_cmd(CHAR *pcCmdStr);
extern void     LCD_WriteReg(UINT8 addr, UINT8 value);
extern void     GPIOMap_TriggerFlash(BOOL bOn);

extern UINT32   VersionNumber, ReleaseDate;
extern void MP_AVIUti_SaveAllJPG(char *pFilePath, UINT32 startsec, UINT32 saveFN);
extern void MP_AVIUti_ShowAllSize(void);


//----- CommandTsk  ------------------
void CommandTsk(void)
{
    CHAR    pcCommandLine[_UART_MONITOR_CMD_SIZE];
    CHAR    cLen;

    /* start up messages */
    CmdTsk_OutputMsg(("\r\n-------------------------------------------------------------\r\n"));
    CmdTsk_OutputMsg(("  Novatek NT96220 \r\n"));
    CmdTsk_OutputMsg(("  Copyright (c) 2011 Novatek Microelectronic Corp.\r\n"));
    CmdTsk_OutputMsg(("  Kernel      ver: %s, build: %s\r\n", Ker_GetVerInfo(), Ker_GetBuildDate()));
    CmdTsk_OutputMsg(("  Driver(SYS) ver: %s, build: %s\r\n", drv_getVerInfo(), drv_getBuildDate()));
    CmdTsk_OutputMsg(("  Driver(IPP) ver: %s, build: %s\r\n", drv_getVerInfo_IPP(), drv_getBuildDate_IPP()));
    CmdTsk_OutputMsg(("  Driver(COD) ver: %s, build: %s\r\n", drv_getVerInfo_Codec(), drv_getBuildDate_Codec()));
    CmdTsk_OutputMsg(("  Application ver: %s, build: %s\r\n", App_GetVerInfo(), App_GetBuildDate()));
    strncpy(pcCommandLine, (char *)&VersionNumber, 8);
    pcCommandLine[8] = '\0';
    CmdTsk_OutputMsg(("  Project     ver: %s, ", pcCommandLine));
    strncpy(pcCommandLine, (char *)&ReleaseDate, 8);
    pcCommandLine[8] = '\0';
    CmdTsk_OutputMsg(("build: %s\r\n", pcCommandLine));
    CmdTsk_OutputMsg(("-------------------------------------------------------------\r\n"));
    CmdTsk_OutputMsg(("  Firmware    ver: %s\r\n", (CHAR *)GetVersionString()));
    CmdTsk_OutputMsg(("  Firmware  build: %s\r\n", (CHAR *)Prj_GetBuildDateStd()));
    CmdTsk_OutputMsg(("=============================================================\r\n\r\n"));

    while(1)
    {
        CmdTsk_OutputMsg(("> "));
        cLen = _UART_MONITOR_CMD_SIZE;
        uart_getString(pcCommandLine, &cLen);
        CmdTsk_OutputMsg(("\r\n"));
        if (cLen)
        {
            CmdTsk_DoCommand(pcCommandLine);
        }
    }
}

/**
  Excute command received from UART

  Excute command received from UART.
  [Command Task internal API]

  @param CHAR *pcCmdStr: Command String
  @return void
*/
void CmdTsk_DoCommand(CHAR *pcCmdStr)
{
    unsigned char item, data;
    char *pBuf;

    switch (*pcCmdStr)
    {
    // Help
    case 'h':
    case '?':
        if ((!strcmp(pcCmdStr, "h")) || (!strcmp(pcCmdStr, "?")) || (!strcmp(pcCmdStr, "help")))
        {
            CmdTsk_OutputMsg(("h   : display help message\r\n"));
            CmdTsk_OutputMsg(("?   : display help message\r\n"));
            CmdTsk_OutputMsg(("r 0xHex_Address [0xHex_Length]: read contents of memory\r\n"));
            CmdTsk_OutputMsg(("w 0xHex_Address 0xHex_Data : write data to memory\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'A'
    case 'a':
        if (!strncmp(pcCmdStr, "asen ", 5))
        {
            sen_cmd((pcCmdStr+5));
        }
        else if (!strncmp(pcCmdStr, "a5xff ", 5))
        {
            MediaPlay_SpeedUp(SPEED_H264_5X);
            MediaPlay_FWD();
            MediaPlay_WaitStepFinish();
        }
        else if (!strncmp(pcCmdStr, "a1x ", 3))
        {
            MediaPlay_SpeedUp(SPEED_NORMAL);
            MediaPlay_FWD();
            MediaPlay_WaitFFBW1XFinish();
            aud_setVolume(GetAudioVolume());
        }
        else if (!strncmp(pcCmdStr, "a5xbb ", 5))
        {
            MediaPlay_SpeedUp(SPEED_H264_5X);
            MediaPlay_BWD();
            MediaPlay_WaitStepFinish();
        }
        else if (!strncmp(pcCmdStr, "adec", 4))
        {
            //MediaPlayVideo_Step();
        }
        else if (!strncmp(pcCmdStr, "astart", 6))
        {
            debug_msg("rec start !!!!!!!\r\n");
            MovRec_Record();//AVIRec_Record();
        }
        else if (!strncmp(pcCmdStr, "autotest", 8))
        {
            Ux_BackgroundPostEvent(NVTEVT_BKW_BURNIN_TEST);
        }
        else if (!strncmp(pcCmdStr, "astop", 5))
        {
            debug_msg("rec stop !!!!!!!\r\n");
            MovRec_Stop(TRUE);//AVIRec_Stop(TRUE);
        }
        else if (!strncmp(pcCmdStr,"amba robust", 11))
        {
            UINT32 addr1, addr2, size;
            UINT32 loop, index;
            get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
            rel_blk(POOL_ID_CAPTURE, pBuf);

            addr1 = (UINT32)pBuf;
            addr1 &= 0xFFFFFFFC;
            addr2 = addr1 + (POOL_SIZE_CAPTURE) / 2 - 0x4000;
            size = (POOL_SIZE_CAPTURE) / 2 - 0x5000;

            for(loop = 1; loop < size; loop ++)
            {
                for(index = 0; index < loop; index ++)
                {
                    *(UINT8 *)(addr1 + index) = randomUINT32();
                }
                ambamem_memcpy(addr2, addr1, loop);

                if(memcmp((void *)addr2, (void *)addr1, loop) == 0)
                    debug_msg("amba dma test size = 0x%08x success\r\n", loop);
                else
                    debug_msg("amba dma test size = 0x%08x fail\r\n", loop);
            }
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'B'
    case 'b':
        get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
        rel_blk(POOL_ID_CAPTURE, pBuf);

        if(pBuf == NULL)
        {
            CmdTsk_OutputMsg(("Benchmark : get buffer fail\r\n"));
            break;
        }
        if (!strncmp(pcCmdStr,"benchmark card", 14))
        {
            CmdTsk_OutputMsg(("Benchmark Card...\r\n"));
            if (FilesysBenchmark(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), (UB *)pBuf, POOL_SIZE_CAPTURE) == E_OK)
            {
                CmdTsk_OutputMsg(("Benchmark done\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Benchmark fail\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"benchmark nand", 14))
        {
            CmdTsk_OutputMsg(("Benchmark Nand...\r\n"));
            if (FilesysBenchmark(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), (UB *)pBuf, POOL_SIZE_CAPTURE) == E_OK)
            {
                CmdTsk_OutputMsg(("Benchmark done\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Benchmark fail\r\n"));
            }
        }
        //#NT#2012/09/25#HH Chuang -begin
        //#NT#Add SD speed benchmark
        else if (!strncmp(pcCmdStr,"benchmark sd",strlen("benchmark sd")))
        {
            UINT8* pBuf;
            UINT32 i, uiElps, uiTotalTime = 0;
            UINT32 uiPeakI = 0, uiPeakTime = 0;
            UINT32 uiPeakMinI = 0;
            UINT32 uiPeakMinTime = 2000000; // unit: us
            UINT32 uiIdxTestSize, uiIdxWndowSize;
            STRG_TAB* pStrg;
            const UINT32 vTestSize[] = {64*1024, 128*1024, 256*1024, 512*1024, 1024*1024};
            const UINT32 vWindowSize[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
            UINT32 vWndIdx[sizeof(vWindowSize)/sizeof(vWindowSize[0])];
            UINT32 vWndTotalTime[sizeof(vWindowSize)/sizeof(vWindowSize[0])];
            double vMinPerf[sizeof(vWindowSize)/sizeof(vWindowSize[0])];
            double vCurrPerf[sizeof(vWindowSize)/sizeof(vWindowSize[0])];
            double vPerfResult[sizeof(vTestSize)/sizeof(vTestSize[0])][sizeof(vWindowSize)/sizeof(vWindowSize[0])];

            get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
            rel_blk(POOL_ID_CAPTURE, pBuf);
            pStrg = sdio_getStorageObject();

            Perf_Open();

            for (uiIdxTestSize=0; uiIdxTestSize<(sizeof(vTestSize)/sizeof(vTestSize[0])); uiIdxTestSize++)
            {
                if (1)
                {
                    sdio_printCardInformation();

                    uiTotalTime = 0;
                    uiPeakTime = 0;
                    uiPeakMinTime = 2000000;
                    for (uiIdxWndowSize=0; uiIdxWndowSize<(sizeof(vWindowSize)/sizeof(vWindowSize[0])); uiIdxWndowSize++)
                    {
                        vCurrPerf[uiIdxWndowSize] = 0.0;
                        vMinPerf[uiIdxWndowSize] = 1000.0;
                        vWndIdx[uiIdxWndowSize] = 0;
                        vWndTotalTime[uiIdxWndowSize] = 0;
                    }
                    for(i=0; i<1000; i++)
                    {
                        Perf_Mark();
                        pStrg->WrSectors((CHAR*)pBuf, 0xF0000+i*vTestSize[uiIdxTestSize]/512, vTestSize[uiIdxTestSize]/512);
                        uiElps = Perf_GetDuration();
                        uiTotalTime += uiElps;
                        if (uiElps > uiPeakTime)
                        {
                            uiPeakTime = uiElps;
                            uiPeakI = i;
                        }
                        if (uiElps < uiPeakMinTime)
                        {
                            uiPeakMinTime = uiElps;
                            uiPeakMinI = i;
                        }
                        for (uiIdxWndowSize=0; uiIdxWndowSize<(sizeof(vWindowSize)/sizeof(vWindowSize[0])); uiIdxWndowSize++)
                        {
                            vWndTotalTime[uiIdxWndowSize] += uiElps;
                            vWndIdx[uiIdxWndowSize]++;
                            if (vWndIdx[uiIdxWndowSize] == vWindowSize[uiIdxWndowSize])
                            {
                                vCurrPerf[uiIdxWndowSize] = ((double)vTestSize[uiIdxTestSize])*vWindowSize[uiIdxWndowSize]/vWndTotalTime[uiIdxWndowSize];
                                vWndIdx[uiIdxWndowSize] = 0;
                                vWndTotalTime[uiIdxWndowSize] = 0;
                                if (vCurrPerf[uiIdxWndowSize] < vMinPerf[uiIdxWndowSize])
                                {
                                    vMinPerf[uiIdxWndowSize] = vCurrPerf[uiIdxWndowSize];
                                }
                            }
                        }
                    }
                    CmdTsk_OutputMsg(("Peak MAX at %d: %d us\r\n", uiPeakI, uiPeakTime));
                    CmdTsk_OutputMsg(("Peak MIN at %d: %d us\r\n", uiPeakMinI, uiPeakMinTime));
                    CmdTsk_OutputMsg(("Total size %d MB, time %d us: %f MB/s\r\n", 1000*vTestSize[uiIdxTestSize]/1024/1024, uiTotalTime, 1000.0*vTestSize[uiIdxTestSize]/uiTotalTime));
                    for (uiIdxWndowSize=0; uiIdxWndowSize<(sizeof(vWindowSize)/sizeof(vWindowSize[0])); uiIdxWndowSize++)
                    {
                        CmdTsk_OutputMsg(("Worst%d perf is %f MB/s\r\n", uiIdxWndowSize, vMinPerf[uiIdxWndowSize]));
                        vPerfResult[uiIdxTestSize][uiIdxWndowSize] = vMinPerf[uiIdxWndowSize];
                    }

                }


            }

            CmdTsk_OutputMsg(("Benchmark result:\r\n"));
            CmdTsk_OutputMsg(("Worst"));
            for (i=0; i<(sizeof(vTestSize)/sizeof(vTestSize[0])); i++)
            {
                CmdTsk_OutputMsg(("\t%d KB\t", vTestSize[i]/1024));
            }
            CmdTsk_OutputMsg(("\r\n"));
            for (uiIdxWndowSize=0; uiIdxWndowSize<(sizeof(vWindowSize)/sizeof(vWindowSize[0])); uiIdxWndowSize++)
            {
                CmdTsk_OutputMsg(("%d unit", vWindowSize[uiIdxWndowSize]));

                for (i=0; i<(sizeof(vTestSize)/sizeof(vTestSize[0])); i++)
                {
                    CmdTsk_OutputMsg(("\t%2.2fMB/s", vPerfResult[i][uiIdxWndowSize]));
                }
                CmdTsk_OutputMsg(("\r\n"));
            }

            Perf_Close();
        }
        //#NT#2012/09/25#HH Chuang -end
        else if (!strncmp(pcCmdStr, "beep", 4))
        {
            KeyScan_PlaySound(0);
        }
        #if (_OSD_SCREEN_DUMP_ == ENABLE)
        else if (!strncmp(pcCmdStr,"bmp make", 8))
        {
            char filename[20];

            sscanf(pcCmdStr+9,"%s",filename);
            BMP_MakeFile(filename);
        }
        #endif
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'C'
    case 'c':
        if (!strncmp(pcCmdStr,"chknand all", 11))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
                UINT32 errblock;
                CmdTsk_OutputMsg(("Read check for all NAND: \r\n"));
                errblock = nand_readCheckAll(TRUE);
                CmdTsk_OutputMsg(("nand_readCheckAll report %d bad block!\r\n", errblock));
            #endif
        }
        else if (!strncmp(pcCmdStr,"chknand erase", 13))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
                UINT32 errblock;
                CmdTsk_OutputMsg(("Erase check NAND FAT area: \r\n"));
                errblock = nand_eraseCheckFAT();
                CmdTsk_OutputMsg(("nand_eraseCheckFAT report %d bad block!\r\n", errblock));
            #endif
        }
        //this match should be placed at the end of all of the aboves
        else if (!strncmp(pcCmdStr,"chknand", 7))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
                UINT32 errblock;
                CmdTsk_OutputMsg(("Read check for NAND read error: \r\n"));
                errblock = nand_readCheckAll(FALSE);
                CmdTsk_OutputMsg(("nand_readCheckAll report %d bad block!\r\n", errblock));
            #endif
        }
        else if (!strncmp(pcCmdStr,"card out", 8))
        {
            PSTRG_TAB storageObj;
            storageObj = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);
            storageObj->Close();
            CmdTsk_OutputMsg(("Remove card\r\n"));
        }
        else if (!strncmp(pcCmdStr,"card in", 7))
        {
            PSTRG_TAB storageObj;
            storageObj = Primary_GetStorageObj(PRIMARY_STGTYPE_CARD);
            storageObj->Open();
            CmdTsk_OutputMsg(("Insert card\r\n"));
        }
        else if (!strncmp(pcCmdStr, "copy iso", 8))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
                UINT32      uiduration;
                HNVT_FILE   *pFile;
                UINT32      uiFileSize;
                UINT32      sectNum;
                UINT32      copySectCnt = (5 * 1024 * 1024) / 512;  // 5MB per copy
                UINT32      copyCnt, i ,startSect, readSect;
                char        *pBuf;
                char        *fileName = "A:\\setup.iso";
                PSTRG_TAB   nandObj;
                PSTRG_TAB   sdmmcObj;
                ER          err;

                debug_err(("format NAND\r\n"));
                FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), FALSE);
                debug_err(("format NAND2\r\n"));
                FilesysFormat(nand2_getStorageObject(), FALSE);

                get_blk((void *)&pBuf,  POOL_ID_CAPTURE);
                rel_blk(POOL_ID_CAPTURE, pBuf);
                debug_err(("pBuf = 0x%08x\r\n", pBuf));
                pBuf += 0x100000;   // to avoid overwrite display buffer

                debug_err(("read iso file\r\n"));
                pFile = Filesys_fopen((char *)fileName, (char *)"r");
                if (pFile == NULL)
                {
                    debug_err(("open ISO file error\r\n"));
                    return;
                }

                // the number of sectors to copy a file
                sectNum = (pFile->fileSize / 512);
                if (pFile->fileSize % 512)
                    sectNum++;

                // Copy the "copySectCnt" numbers of sectors once
                copyCnt = sectNum / copySectCnt;
                if (sectNum % copySectCnt)
                    copyCnt++;

                debug_err(("ISO size: file(0x%x) pool(0x%x) sectors(%d)\r\n", pFile->fileSize, POOL_SIZE_CAPTURE, sectNum));
                FilesysCloseActFile();

                // Start to copy
                for (i = 0; i < copyCnt; i++)
                {
                    pFile = Filesys_fopen((char *)fileName, (char *)"r");
                    if (pFile == NULL)
                    {
                        debug_err(("open ISO file error\r\n"));
                        err = E_SYS;
                        break;
                    }

                    startSect = copySectCnt * i;
                    if (i < copyCnt - 1)
                        readSect = copySectCnt;
                    else
                        readSect = sectNum - (copySectCnt * i);

                    // Read to memory
                    FilesysReadWrite(
                        FST_CMD_CONTINUE_ACCESS,
                        (UINT8 *)pBuf,
                        readSect * 512,
                        startSect * 512,
                        FALSE,
                        FST_FMT_ANYFORMAT,
                        FST_TIME_INFINITE);

                    FilesysCloseActFile();
                    FilesysPause();
                    FilesysWaitPaused();

                    sdmmcObj = sdio_getStorageObject();
                    sdmmcObj->Close();

                    // Copy to flash
                    nandObj = nand2_getStorageObject();
                    nandObj->Open();
                    err = nandObj->WrSectors(pBuf, startSect, readSect);
                    nandObj->Close();

                    sdmmcObj->Open();
                    FilesysResume();

                    if (err != E_OK)
                    {
                        debug_err(("Copy iso failed!\r\n"));
                        return;
                    }
                }

                debug_err(("Copy iso OK!\r\n"));
            #endif
        }
        else if (!strncmp(pcCmdStr, "config save", 11))
        {
            CmdTsk_OutputMsg(("Saving menu info to flash......\r\n"));
            Save_MenuInfo();
            CmdTsk_OutputMsg(("Saving completed!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "config reset", 12))
        {
            CmdTsk_OutputMsg(("Reset menu info to default......\r\n"));
            SysReset_MenuInfo();
            CmdTsk_OutputMsg(("Resetting completed!\r\n"));
        }
        else if (!strncmp(pcCmdStr, "cal test", 8))
        {
            CmdTsk_OutputMsg(("Calibration mode test\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_ENGINEER);
        }
        else if (!strncmp(pcCmdStr, "cal ECS", 7))
        {
            CmdTsk_OutputMsg(("ECS Calibration test\r\n"));
            Cal_Open();
            ECS_Adjust();
        }
        else if (!strncmp(pcCmdStr, "cal AWB", 7))
        {
            CmdTsk_OutputMsg(("ECS Calibration test\r\n"));
            Cal_Open();
            AWB_Adjust();
        }
        else if (!strncmp(pcCmdStr,"chgclk pb", 9))
        {
            SysClk_SetClk(SPEED_ID_PLAYBACK);
            CmdTsk_ShowClk();

        }
        else if (!strncmp(pcCmdStr,"chgclk mp3", 10))
        {
            SysClk_SetClk(SPEED_ID_MP3);
            CmdTsk_ShowClk();

        }
        else if (!strncmp(pcCmdStr,"chgclk usb", 10))
        {
            SysClk_SetClk(SPEED_ID_USBSICD);
            CmdTsk_ShowClk();

        }
        else if (!strncmp(pcCmdStr,"chkclk", 6))
        {
            CmdTsk_ShowClk();
        }
        else if (!strncmp(pcCmdStr,"chkdelaychain", 13))
        {
            debug_msg("^C\033[1m ---DRAM & PAD driving parameter---\r\n");
            debug_msg("^Y\033[1m clk[%02d]T/DQSIN[%02d]T/DQOut[%02d]T\r\n", dma_getClockDelay(), dma_getDQSInDelay(), dma_getDQOutDelay());
            debug_msg("^Y\033[1m PAD driving 0x97100110[0x%08x] =>", *(UINT32 *)0x97100110, *(UINT32 *)0x97100114);
            if(((*(UINT32 *)0x97100114)&0xFF) == 0x00)
                debug_msg("^Y\033[1m 1/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x18)
                debug_msg("^Y\033[1m 2/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x02)
                debug_msg("^Y\033[1m 3/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x1A)
                debug_msg("^Y\033[1m 4/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x01)
                debug_msg("^Y\033[1m 6/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x19)
                debug_msg("^Y\033[1m 7/8 default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x03)
                debug_msg("^Y\033[1m default driving\r\n");
            else if(((*(UINT32 *)0x97100114)&0xFF) == 0x1B)
                debug_msg("^Y\033[1m 9/8 default driving\r\n");
            else
                debug_msg("^R\033[1m Unknow driving\r\n");
            debug_msg("^C\033[1m ---DRAM & PAD driving parameter---\r\n");
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'D'
    case 'd':
        //#NT#2008/03/20#Chris Hsu -begin
        //#NT#Replace with debug_dumpKernelStatus() API
        if (!strncmp(pcCmdStr,"dump ker", 8))
        {
            debug_dumpKernelStatus();
        }
        //#NT#2008/03/20#Chris Hsu -end
        else if (!strncmp(pcCmdStr,"dump nand", 9))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
            //ReadNand_Total();
            #endif
        }
        //#NT#2009/01/21#HH Chuang -begin
        //#NT#add printing sd card information
        else if (!strncmp(pcCmdStr, "dump sdinfo", 11))
        {
            sdio_printCardInformation();
        }
        //#NT#2009/01/21#HH Chuang -end
        else if (!strncmp(pcCmdStr, "dump drvinfo", 12))
        {
            drv_dumpInfo();
        }
        else if (!strncmp(pcCmdStr, "dump memory", 11))
        {
           volatile UINT32 *pAddr=0;
           UINT32 fileSize=0x200000;

           FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, (char *)"A:\\Memory.bin", (UB *)pAddr,
                            &fileSize, 0, FST_TIME_INFINITE);
           CmdTsk_OutputMsg(("Dump Memory ok\n\r"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'F'
    case 'f':
        if (!strncmp(pcCmdStr,"format card", 11))
        {
            CmdTsk_OutputMsg(("Format Card...\r\n"));
            if (FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_CARD), TRUE) == E_OK)
            {
                CmdTsk_OutputMsg(("Format Done.\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Format Fail\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"format nand2", 12))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
                CmdTsk_OutputMsg(("Format Nand...\r\n"));
                if (FilesysFormat(nand2_getStorageObject(), FALSE) == E_OK)
                {
                    CmdTsk_OutputMsg(("Format nand2 Done.\r\n"));
                }
                else
                {
                    CmdTsk_OutputMsg(("Format nand2 Fail\r\n"));
                }
            #endif
        }
        else if (!strncmp(pcCmdStr,"format nand", 11))
        {
            #if (_PSTOREDEVICE_ == _PSTOREDEVICE_NANDPS_)
            CmdTsk_OutputMsg(("Format Nand...\r\n"));
            if (FilesysFormat(Primary_GetStorageObj(PRIMARY_STGTYPE_NAND), TRUE) == E_OK)
            {
                CmdTsk_OutputMsg(("Format Done.\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("Format Fail\r\n"));
            }
            #endif
        }
        else if (!strncmp(pcCmdStr, "fl en", 6))
        {
            KeyScan_EnableFlashCharging(TRUE);
        }
        else if (!strncmp(pcCmdStr, "fl stop", 8))
        {
            KeyScan_EnableFlashCharging(FALSE);
        }
        else if (!strncmp(pcCmdStr, "fl trig", 8))
        {
            GPIOMap_TriggerFlash(ON);
            TimerDelayMs(30);
            GPIOMap_TriggerFlash(OFF);
        }
        else if (!strncmp(pcCmdStr,"fd",2))
        {
            UINT32 islock;
            CHAR arg_num;

            arg_num = sscanf((pcCmdStr+2)," %d\r\n",&islock);
            debug_msg("fd test=%d\r\n", islock);

            if (arg_num == 1)
            {
                Photo_LockFD(islock);
            }
            else
            {
                debug_msg("bad command or invalid parameters\r\n");
            }
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'G'
    case 'g':
#if (_IQ_SIM_ == ENABLE)
        if (!strncmp(pcCmdStr, "gc", 2))
        {
            IQSim_GenSourceCode();
        }
        else
#endif
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'I'
#if (_SYSINFO_MODE_ == ENABLE)
    case 'i':
        if (!strncmp(pcCmdStr, "info sys", 8))
        {
            SYSInfo_GetSystemInfo();
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;
#endif

    // Command start with 'K'
    case 'k':
        if (!strncmp(pcCmdStr, "kdel", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DEL);
            Ux_PostEvent(NVTEVT_KEY_DEL, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Del Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kdis", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DISPLAY);
            Ux_PostEvent(NVTEVT_KEY_DISPLAY, 1, NVTEVT_KEY_DISPLAY);
            //Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Display Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ku", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_UP);
            Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Up Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kd", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_DOWN);
            Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Down Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kr", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RIGHT);
            Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Right Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kl", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_LEFT);
            Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Left Key\r\n"));
        }
//#NT#2011/07/05#Ben Wang -begin
//#NT#Add ring buffer mechanism for JPEG bitstream
        else if (!strncmp(pcCmdStr, "kt0", 3))
        {
            CmdTsk_OutputMsg(("JPG_ENC_BS_DBG_NONE\r\n"));
            JpegEncSetData(JPG_DBG_LEVEL, JPG_ENC_BS_DBG_NONE);
        }
        else if (!strncmp(pcCmdStr, "kt1", 3))
        {
            CmdTsk_OutputMsg(("JPG_ENC_BS_DBG_FRAME\r\n"));
            JpegEncSetData(JPG_DBG_LEVEL, JpegEncGetData(JPG_DBG_LEVEL)|JPG_ENC_BS_DBG_FRAME);
        }
        else if (!strncmp(pcCmdStr, "kt2", 3))
        {
            CmdTsk_OutputMsg(("JPG_ENC_BS_DBG_QVALUE\r\n"));
            JpegEncSetData(JPG_DBG_LEVEL, JpegEncGetData(JPG_DBG_LEVEL)|JPG_ENC_BS_DBG_QVALUE);
        }
//#NT#2011/07/05#Ben Wang -end
        else if (!strncmp(pcCmdStr, "ke", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ENTER);
            Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Enter Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kmd", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_MODE);
            Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Mode Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "km", 2))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_MENU);
            Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Menu Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzir", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_ZOOMIN);
            Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Zoom In Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzi", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ZOOMIN);
            Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Zoom In Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzor", 4))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_RELEASED | FLGKEY_ZOOMOUT);
            Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_RELEASE);
            CmdTsk_OutputMsg(("Zoom Out Key release\r\n"));
        }
        else if (!strncmp(pcCmdStr, "kzo", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_ZOOMOUT);
            Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Zoom Out Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks1", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER1);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter1 Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks2", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER2);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter2 Key\r\n"));
        }
        else if (!strncmp(pcCmdStr, "ks", 3))
        {
//            set_flg(FLG_ID_KEY, FLGKEY_SHUTTER);
            Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
            CmdTsk_OutputMsg(("Shutter Key\r\n"));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'L'
    case 'l':
        if (!strncmp(pcCmdStr,"li",2))
        {
            //LensInit();
            CmdTsk_OutputMsg(("Lens Init\r\n"));
        }
        if (!strncmp(pcCmdStr,"lr",2))
        {
            //LensRetract();
            CmdTsk_OutputMsg(("Lens retract\r\n"));
        }
        else if (!strncmp(pcCmdStr,"laf",3))
        {
            //AF_Process();
            CmdTsk_OutputMsg(("AF\r\n"));
        }
        else if(!strncmp(pcCmdStr,"lm ",3))
        {
            sscanf(pcCmdStr+4,"%d %d \n",&item,& data);
            UI_SetParameter(item,data);
        }
        else if(!strncmp(pcCmdStr,"la ",3))
        {
            sscanf(pcCmdStr+4,"%d\n",& data);
            Lens_Aperture_Move(data);
        }
        else if (!strncmp(pcCmdStr, "lcd w", 5))
        {
            UINT8   addr, data;
            sscanf(pcCmdStr + 6, "%x %x", &addr, &data);
            ////LCD_WriteReg(addr, data);
        }
        else if (!strncmp(pcCmdStr, "lens ", 5))
        {
            Lens_Cmd((pcCmdStr+5));
        }
        break;

    // Command start with 'M'
    case 'm':
        if (!strncmp(pcCmdStr,"mode photo", 10))
        {
            CmdTsk_OutputMsg(("Photo Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PHOTO);
        }
        else if (!strncmp(pcCmdStr,"mode setup", 10))
        {
            CmdTsk_OutputMsg(("Setup Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_SETUP);
        }
        else if (!strncmp(pcCmdStr,"mode playback", 13))
        {
            CmdTsk_OutputMsg(("Playback Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_PLAYBACK);
        }
        else if (!strncmp(pcCmdStr,"mode msdc", 9))
        {
            CmdTsk_OutputMsg(("MSDC Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBMSDC);
        }
        else if (!strncmp(pcCmdStr,"mode sicd", 9))
        {
            CmdTsk_OutputMsg(("SICD Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBSICD);
        }
        else if (!strncmp(pcCmdStr,"mode pcc", 8))
        {
            CmdTsk_OutputMsg(("PCC Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_USBPCC);
        }
        else if (!strncmp(pcCmdStr,"mode movie", 10))
        {
            CmdTsk_OutputMsg(("Movie Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_MOVIE);
        }
        else if (!strncmp(pcCmdStr,"mode mp3", 8))
        {
            CmdTsk_OutputMsg(("MP3 Mode\r\n"));
            Primary_ChangeMode(PRIMARY_MODE_MUSIC);
        }
#if 0
        else if (!strncmp(pcCmdStr, "ms open", 7))
        {
            GPIOMap_MShutOpen();
            TimerDelayMs(20);
            GPIOMap_MShutOff();
        }
        else if (!strncmp(pcCmdStr, "ms close", 8))
        {
            GPIOMap_MShutClose();
            TimerDelayMs(20);
            GPIOMap_MShutOff();
        }
        else if (!strncmp(pcCmdStr, "ms off", 6))
        {
            GPIOMap_MShutOff();
        }
#endif
        else if (!strncmp(pcCmdStr,"motion on", 9))
        {
           while (1)
           {
                MotionDetection();
                sie_waitVD(3);
           }
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'p'
    case 'p':
        if (!strncmp(pcCmdStr, "ps init", 7))
        {
            SysInit_InitPStore();
        }
        else if (!strncmp(pcCmdStr, "ps format", 9))
        {
            PSTORE  PStore;

            debug_err(("Format PS...\r\n"));
            PStore.uiSignature  = 0x55AA55AA;
            PStore.uiPSVersion  = 0x0001;
            PStore.uiPStoreSize = SysInit_GetPStoreSize();
            //PStore_EnablePS();
            PStore_Format(&PStore);
            //PStore_DisablePS();
            SysInit_InitPStore();
            debug_err(("Format PS Done.\r\n"));
        }
        else if (!strncmp(pcCmdStr,"pf update ", 10))
        {
            UINT32  i;
            UINT32  uiFrameNum;
            char    pFileName[32];

            uiFrameNum = atoi(&pcCmdStr[10]);

            debug_err(("update %d pictures of photo frame\r\n", uiFrameNum));

            UserPF_DeleteAll();

            //SysSetFL(FL_PHOTO_FRAME, 0);

            for(i = 0; i <= uiFrameNum - 1; i++)
            {
                sprintf(pFileName, "A:\\MISC\\%02ld.JPG", i);

                UserPF_WriteFrame(i, pFileName);
            }

            UserPF_WriteHeader();

            debug_err(("update finish!!\r\n"));
        }
        else if (!strncmp(pcCmdStr,"pf read ", 8))
        {
        }
        else if (!strncmp(pcCmdStr,"pf enable", 9))
        {
            PrimaryModeID mode = Primary_GetCurrentMode();

            if (mode == PRIMARY_MODE_PHOTO)
            {
                UserPF_OpenPhotoFrame(PRIMARY_MODE_PHOTO, 0);
            }
            else if (mode == PRIMARY_MODE_PLAYBACK)
            {
                UserPF_OpenPhotoFrame(PRIMARY_MODE_PLAYBACK, 0);
                Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_PLAYCURR, 0);
            }
        }
        else if (!strncmp(pcCmdStr,"pf disable", 10))
        {
            UserPF_ClosePhotoFrame();
        }
        else if (!strncmp(pcCmdStr,"pf store", 8))
        {
            if (Primary_GetCurrentMode() == PRIMARY_MODE_PLAYBACK)
            {
                UINT32 uiDirId, uiFileId, uiFileType;

                // Save gResrInfo of file system
                uiDirId = PB_GetCurrFileSysInfo(PLAYFILEINFO_DIRID);
                uiFileId = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEID);
                uiFileType = PB_GetCurrFileSysInfo(PLAYFILEINFO_FILEFORMAT);
                FilesysSaveReservedInfo(uiDirId, uiFileId, uiFileType);

                // Save image with photo frame
                if (PhotoFrame_StoreNewImagePhotoFrame(uiDirId, uiFileId) != E_OK)
                {
                    // Show error message
                }

                // Restore gResrInfo of file system
                FilesysRestoreReservedInfo();

                // Play original image again
                Ux_SendEvent(&UIPlayObjCtrl, NVTEVT_PLAYCURR, 0);
            }
        }
        else if (!strncmp(pcCmdStr, "power off", 9))
        {
            KeyScan_Poweroff();
        }
        break;

    // Command start with 'R'
    case 'r':
        if (!strncmp(pcCmdStr, "r ", 2))
        {
            CmdTsk_ReadMemory((pcCmdStr+2));
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'S'
    case 's':
        if (!strncmp(pcCmdStr,"s2",2))//Single capture
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize,  _IMGSize_5M);
            ImgCapture_SetParameter(_FileFmt,  _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieCapBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_BurstNum,  _BurstNum_1);
            ImgCapture_SetParameter(_DriveMode,  _IMGDriveMode_Single);
            ide_disableVideo(IDE_VIDEOID_1);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("Single Capture \r\n"));
        }
        else if (!strncmp(pcCmdStr,"s3",2))//Burst Capture 3
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize,  _IMGSize_1M);
            ImgCapture_SetParameter(_FileFmt,  _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieCapBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_BurstNum,  _BurstNum_5);
            ImgCapture_SetParameter(_DriveMode,  _IMGDriveMode_SmallBurst16);
            ide_disableVideo(IDE_VIDEOID_1);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("Burst Capture 3 pic\r\n"));
        }
        else if (!strncmp(pcCmdStr,"s4",2))//InfBurst
        {
            ImgCapture_SetParameter(_FlashMode, _IMGFlashMode_ForceOFF);
            ImgCapture_SetParameter(_ImgSize, _IMGSize_5M);
            ImgCapture_SetParameter(_FileFmt, _FileFmt_Jpg);
            ImgCapture_SetParameter(_SieBitDepth, _SieBitDepth_8);
            ImgCapture_SetParameter(_SiePath,_PostGamma);
            ImgCapture_SetParameter(_DriveMode, _IMGDriveMode_InfBurst);
            ImgCapture_StartCapture();
            CmdTsk_OutputMsg(("InfBurst\r\n"));
        }
        else if (!strncmp(pcCmdStr,"ss",2))//Stop Burst Capture
        {
            ImgCapture_StopBurstCapture();
            CmdTsk_OutputMsg(("StopBurst\r\n"));
        }
        else if (!strncmp(pcCmdStr,"sn set",6)) // set social network value
        {
            UINT32  uiSocialNetworkValue;
            UINT32  uiMode = Primary_GetCurrentMode();

            sscanf(pcCmdStr+7, "%d", &uiSocialNetworkValue);
            debug_err(("Social Network value = %d\r\n", uiSocialNetworkValue));

            if (uiMode == PRIMARY_MODE_PHOTO)
            {
                Ux_SendEvent(&UIPhotoObjCtrl, NVTEVT_EXE_SOCIALNETWORK, 1, uiSocialNetworkValue);
            }
            else if (uiMode == PRIMARY_MODE_MOVIE)
            {
                Ux_SendEvent(&UIMovieObjCtrl, NVTEVT_EXE_SOCIALNETWORK, 1, uiSocialNetworkValue);
            }
            else
            {
                Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_SOCIALNETWORK, 1, uiSocialNetworkValue);
            }
        }
        else if (!strncmp(pcCmdStr,"sn get",6)) // get social network value
        {
            UINT32  uiMode = Primary_GetCurrentMode();

            if (uiMode == PRIMARY_MODE_PLAYBACK)
            {
                debug_err(("Social Network value = %d\r\n", PB_GetSocialNetworkValue()));
            }
            else if (uiMode == PRIMARY_MODE_PHOTO)
            {
                debug_err(("Social Network value = %d\r\n", SysGetFlag(FL_PHOTO_VIVILINK)));
            }
            else if (uiMode == PRIMARY_MODE_MOVIE)
            {
                debug_err(("Social Network value = %d\r\n", SysGetFlag(FL_MOVIE_VIVILINK)));
            }
        }
        else if (!strncmp(pcCmdStr,"sn all",6)) // set social network value for all images/movies
        {
            UINT32  uiSocialNetworkValue;

            sscanf(pcCmdStr+7, "%d", &uiSocialNetworkValue);
            debug_err(("Social Network set all, value = %d\r\n", uiSocialNetworkValue));
            Ux_SendEvent(&UISystemObjCtrl, NVTEVT_EXE_SOCIALNETWORK, 2, uiSocialNetworkValue, VIVILINK_SET_ALL);
        }
        else if (!strncmp(pcCmdStr,"showall",7))//save all
        {
            MP_AVIUti_ShowAllSize();
        }
        else if (!strncmp(pcCmdStr,"savealljpg",10))//save all
        {
            FILE_TSK_DCF_PARAM *pFS;
            char tempfilename[KFS_LONGNAME_PATH_MAX_LENG];
            int sec=0, frame=30;
            pFS = FilesysGetFSTParam();


            memcpy(tempfilename, pFS->pFile->fullFileName, KFS_LONGNAME_PATH_MAX_LENG);

                {
                    if (strlen(pcCmdStr) > 10)
                    {
                        sscanf(pcCmdStr+11, "%d %d", &sec, &frame);
                    }
                }

            MP_AVIUti_SaveAllJPG(tempfilename, sec, frame);
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    // Command start with 'T'
    case 't':
        {
//#NT#2007/04/17#Stephen Wang -begin
//#NT#Add rtc test utility
            if (!strncmp(pcCmdStr, "time", 4))
            {
                if (strlen(pcCmdStr) > 6)
                {
                    if (!strncmp(pcCmdStr+5, "set", 3))
                    {
                        int year,month,day;
                        int hour,minute,second;
                        sscanf(pcCmdStr+9, "%d/%d/%d %d:%d:%d", &year,&month,&day,&hour,&minute,&second);
                        rtc_setDate(year,month,day);
                        rtc_setTime(hour,minute,second);
                        rtc_triggerCSET();
                        break;
                    }
                }
                {
                    RTC_DATE date;
                    RTC_TIME time;
                    date = rtc_getDate();
                    time = rtc_getTime();
                    CmdTsk_OutputMsg(("%d/%d/%d %d:%d:%d\r\n",date.s.year,date.s.month,date.s.day,time.s.hour,time.s.minute,time.s.second));
                }
            }
//#NT#2007/04/17#Stephen Wang -end
//#NT#2007/05/10#Stephen Wang -begin
            //Stress test for clock change between photo and playback mode
            else if (!strncmp(pcCmdStr, "tclk", 4))
            {
                UINT32 loopcnt;
                int i;

                if (strlen(pcCmdStr) > 5)
                {
                    loopcnt = atoi(pcCmdStr+5);
                    for (i=0; i<loopcnt; i++)
                    {
                        CmdTsk_OutputMsg(("TestClk:%d\r\n",i));
                        SysClk_SetClk(SPEED_ID_PHOTO);
                        SysClk_SetClk(SPEED_ID_PLAYBACK);
                    }
                }

            }
//#NT#2007/05/10#Stephen Wang -end
            else if (!strncmp(pcCmdStr,"tpcal",strlen("tpcal")))
            {
                #if (_TouchPanel_ != DISABLE)
                CmdTsk_OutputMsg(("Touch Panel Cal\r\n"));
                Ux_OpenWindow((VControl *)(&UIFlowWndTouchTest_CalCtrl), 0);
                #endif
            }
            else if (!strncmp(pcCmdStr,"tptest",strlen("tptest")))
            {
                #if (_TouchPanel_ != DISABLE)
                Ux_OpenWindow((VControl *)(&UIFlowWndTouchTestCtrl), 0);
                #endif
            }
            else if (!strncmp(pcCmdStr,"test on",strlen("test on")))
            {
            		ide_enable_video(IDE_VIDEOID_1);            
            }
            else if (!strncmp(pcCmdStr,"test off",strlen("testoff")))
            {
            		ide_disable_video(IDE_VIDEOID_1);            
            }			
            else
            {
                CmdTsk_OutputMsg((COMMAND_ERROR));
            }
        }
        break;

    // Command start with 'U'
    case 'u':
        if (!strncmp(pcCmdStr,"unifont w", 9))
        {
            CmdTsk_OutputMsg(("Download Unicode font... "));

            if(UserPS_WriteUniFontFile((CHAR *)"A:\\UNI_FONT.BIN") == E_USERPS_OK)
            {
                CmdTsk_OutputMsg(("OK!!\r\n"));
            }
            else
            {
                CmdTsk_OutputMsg(("failed!!\r\n"));
            }
        }
        else if (!strncmp(pcCmdStr,"unifont r", 9))
        {
            UINT8   *pUniFont = NULL;
            char    str[] = {0xff, 0xfe, 0x11, 0x62, 0x2f, 0x66, 0x93, 0x8c, 0x00, 0x00}; // "I am a cat" in Traditional Chinese

            UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
            UI_DrawOsdString("Loading...", 114, 104, _OSD_INDEX_WHITE);

            Perf_Open();
            Perf_Mark();
            pUniFont = (UINT8 *)UserPS_InitUniFontData();
            debug_msg("Load Unicode font: time = %d ms\r\n", Perf_GetDuration() / 1000);
            Perf_Close();

            if(pUniFont)
            {
                UI_Set_FontTable(FONT_TABLE_1, (FONT* )pUniFont);
                GxGfx_SetTextStroke((const FONT* )pUniFont, FONTSTYLE_NORMAL, SCALE_1X);
            }

            UI_ClearOSD(_OSD_INDEX_TRANSPART); // clear OSD
            UI_DrawOsdString(str, 122, 104, _OSD_INDEX_YELLOW);
        }
        break;

    // Command start with 'W'
    case 'w':
        if (!strncmp(pcCmdStr,"w ",2))
        {
            CmdTsk_WriteMemory((pcCmdStr+2));
        }
        //#NT#2009/03/24#Hideo Lin -begin
        //#NT#For wav playback test
        #if (_WAVSTUDIO_MODE_ == ENABLE)
        else if (!strncmp(pcCmdStr,"wav play",8))
        {
            FlowPB_PlayWAV();
        }
        else if (!strncmp(pcCmdStr,"wav stop",8))
        {
            FlowPB_StopWAV();
        }
        #endif
        //#NT#2009/03/24#Hideo Lin -end
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;


    case 'z':
        if (!strncmp(pcCmdStr,"z1",strlen("z1")))
        {
            GPIOMap_TurnOffLCDBacklight();
        }
        else if(!strncmp(pcCmdStr,"zoom ",5))
        {
            UINT32 times,j;
            sscanf(pcCmdStr+6,"%d",&times);
            for(j = 0;j<times;j++)
            {
                while(1)
                {
                    Photo_DZoom_StepIn();
                    TimerDelayUs(20);
                    if(Photo_GetDZoomIdx() >= Photo_GetMaxDzoomIdx())
                    {
                        break;
                    }
                }
                while(1)
                {
                    Photo_DZoom_StepOut();
                    TimerDelayUs(20);
                    if(Photo_GetDZoomIdx() <= 10)
                    {
                        break;
                    }
                }
            }
        }
        else if(!strncmp(pcCmdStr,"zi",2))
        {
            Photo_DZoom_In();
        }
        else if(!strncmp(pcCmdStr,"zo",2))
        {
            Photo_DZoom_Out();
        }
        else if(!strncmp(pcCmdStr,"zs",2))
        {
            Photo_DZoom_Stop();
        }
        else if (!strncmp(pcCmdStr,"z2",strlen("z2")))
        {
            GPIOMap_TurnOnLCDBacklight();
        }
        else
        {
            CmdTsk_OutputMsg((COMMAND_ERROR));
        }
        break;

    default:
        CmdTsk_OutputMsg((COMMAND_ERROR));
        break;
    }
} /* DoCommand */

/**
  Read memory and output to UART

  Read memory and output to UART.
  [Command Task internal API]

  @param CHAR *para_str: Command String
  @return void
*/
static void CmdTsk_ReadMemory(CHAR *para_str)
{
    CHAR    str_output[_UART_MONITOR_CMD_SIZE],out_ch;
    UINT32  i=0,j;
    UINT32  addr,length,str_len;
    CHAR    arg_num;

    arg_num=sscanf(para_str, "%x %x %s\n", &addr, &length, str_output);

    if (arg_num==1)
        length=256;

    addr&=0xFFFFFFFC;

    if ((arg_num==1) || (arg_num==2))
    {
        CmdTsk_OutputMsg(("addr = 0x%08X, length = 0x%08X\r\n", addr, length));
        for (i=0;i<length;)
        {

            str_len=sprintf(str_output,"%08lX : %08lX %08lX %08lX %08lX  ",addr, *((UINT32 *)addr),
                              *((UINT32 *)(addr+4)),*((UINT32 *)(addr+8)), *((UINT32 *)(addr+12)));
            for (j=0;j<16;j++)
            {
                out_ch = *((char *)(addr+j));
                if ((out_ch<0x20) || (out_ch>=0x80))
                    str_len+=sprintf(str_output+str_len,".");
                else
                    str_len+=sprintf(str_output+str_len,"%c",out_ch);
            }
            CmdTsk_OutputMsg((str_output));
            CmdTsk_OutputMsg(("\r\n"));
            i+=16;
            addr+=16;
        }
    }
    else
    {
        CmdTsk_OutputMsg((COMMAND_ERROR));
    }
}

/**
  Wriet memory according to command received from UART

  Wriet memory according to command received from UART.
  [Command Task internal API]

  @param CHAR *para_str: Command String
  @return void
*/
static void CmdTsk_WriteMemory(CHAR *para_str)
{
    UINT32  addr, data;
    CHAR    arg_num, str_input[_UART_MONITOR_CMD_SIZE];

    arg_num=sscanf(para_str, "%x %x %s\n", &addr, &data, str_input);
    if (arg_num==2)
    {
        addr &= 0xFFFFFFFC;
        *((int *) addr)=data;
        CmdTsk_OutputMsg(("addr = 0x%08X, data = 0x%08X\r\n", addr, data));
    }
    else
    {
        CmdTsk_OutputMsg((COMMAND_ERROR));
    }
}
void CmdTsk_ShowClk(void)
{
            UINT32 uiCPUClk = 0xFFFFFFFF; //for unknown
            UINT32 uiAHBClk = 0xFFFFFFFF; //for unknown
            UINT32 uiDMAClk = 0xFFFFFFFF; //for unknown;
            switch (pll_getClockRate(PLL_CLKSEL_CPU))
            {
            case PLL_CLKSEL_CPU_48:
                uiCPUClk = 48;
                break;
            case PLL_CLKSEL_CPU_60:
                uiCPUClk = 60;
                break;
            case PLL_CLKSEL_CPU_120:
                uiCPUClk = 120;
                break;
            case PLL_CLKSEL_CPU_160:
                uiCPUClk = 160;
                break;
            case PLL_CLKSEL_CPU_240:
                uiCPUClk = 240;
                break;
            case PLL_CLKSEL_CPU_80:
                uiCPUClk = 80;
                break;
            default:
                CmdTsk_OutputMsg(("CPU??\r\n"));
            }
            switch (pll_getClockRate(PLL_CLKSEL_AHB))
            {
            case PLL_CLKSEL_AHB_CPUCLK:
                uiAHBClk = uiCPUClk;
                break;
            case PLL_CLKSEL_AHB_CPUCLK_D2:
                uiAHBClk = uiCPUClk/2;
                break;
            default:
                CmdTsk_OutputMsg(("AHB??\r\n"));
                break;
            }
            switch (pll_getClockRate(PLL_CLKSEL_DMA))
            {
            case PLL_CLKSEL_DMA_80:
                uiDMAClk = 80;
                break;
            case PLL_CLKSEL_DMA_120:
                uiDMAClk = 120;
                break;
            case PLL_CLKSEL_DMA_160:
                uiDMAClk = 160;
                break;
            default:
                CmdTsk_OutputMsg(("DMA??\r\n"));
                break;
            }
            CmdTsk_OutputMsg(("CPU/AHB/DMA = %d/%d/%d\r\n", uiCPUClk, uiAHBClk, uiDMAClk));
}

#endif
