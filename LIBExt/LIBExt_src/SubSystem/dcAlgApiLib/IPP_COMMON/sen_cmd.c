#include <string.h>
#include <stdio.h>
#include "Type.h"
#include "debug.h"
#include "SIF.h"
#include "FileSysTsk.h"
#include "utility.h"
#include "ide.h"
#include "dma.h"
#include "datastamp.h"
#include "sensor.h"
#include "aaa_lib.h"
#include "ipe_lib.h"
#include "ImageAlgInfor.h"
#include "IPPBuffer.h"
#include "IPPManager.h"
#include "Lens.h"
#include "alg_iplmode.h"
#include "errorno.h"

#define        SEN_COMMAND    1
#if SEN_COMMAND

void RegisterWrite(char *pcStr)
{
    UINT32 uiAddr, uiData;
    CHAR arg_num;

    debug_msg("RegisterWrite\r\n");

    arg_num = sscanf(pcStr," %x %x \n", &uiAddr, &uiData);

    if(arg_num == 2)
    {
        Sensor_WriteReg(uiAddr, uiData);
    }
    else
    {
        debug_err(("missing syntax:sensor \r\n "));
    }
}

void RegisterRead(char *pcStr)
{
    UINT32 uiAddr;
    CHAR arg_num;

    debug_msg("RegisterRead\r\n");
    arg_num = sscanf(pcStr," %x \n", &uiAddr);

    if(arg_num == 1)
    {
        debug_msg("Read Reg[%x, %x]\r\n", uiAddr, Sensor_ReadReg(uiAddr));
    }
    else
    {
        debug_err(("missing syntax:sensor \r\n "));
    }
}

void sen_cmd(char *pcCmdStr);
void sen_cmd(char *pcCmdStr)
{
    int i;

    switch (*pcCmdStr)
    {
        case 'a':
            if (!strncmp(pcCmdStr,"ae lock",7))
            {
                aaa_AeLock(LOCK);
                debug_err(("AE locked\r\n"));
            }
            else if (!strncmp(pcCmdStr,"ae unlock",9))
            {
                aaa_AeLock(UNLOCK);
                debug_err(("AE unlocked\r\n"));
            }
            else if (!strncmp(pcCmdStr,"awb lock",8))
            {
                aaa_AwbLock(LOCK);
                debug_err(("AWB locked\r\n"));
            }
            else if (!strncmp(pcCmdStr,"awb unlock",10))
            {
                aaa_AwbLock(UNLOCK);
                debug_err(("AWB unlocked\r\n"));
            }
            else if(!strncmp(pcCmdStr,"awbmsg ",7))
            {
                extern BOOL g_bawbinfo;
                UINT32 a;
                sscanf(pcCmdStr+7,"%d",&a);
                g_bawbinfo = a;
                debug_msg("awbmsg = %d\r\n",g_bawbinfo);
            }
            else if(!strncmp(pcCmdStr,"aemsg ",6))
            {
                extern UINT32 AEUartMsg;
                UINT32 a;
                sscanf(pcCmdStr+6,"%d",&a);
                AEUartMsg = a;
                debug_msg("AE MSG %d\r\n",AEUartMsg);
            }
            else
            {
                debug_msg("bad command or invalid parameters\r\n");
            }
            break;

        case 'c':
            if (!strncmp(pcCmdStr,"cal dp old",11))
            {
/*
                if(t1==0)
                    Get_Sensor_CalInfor();
                DP_Search();
*/
            }
            else if (!strncmp(pcCmdStr,"cal infor",11))
            {
/*
                Get_Sensor_CalInfor();
*/            }
            else if (!strncmp(pcCmdStr,"cal dp th",9))
            {
/*
                if(t1==0)
                    Get_Sensor_CalInfor();
                sscanf(pcCmdStr+9,"%d %d %d %d",&t1,&t2,&t3,&t4);
                    CalInfor.BrightLimitPrv=t1;
                    CalInfor.BrightLimitCap=t2;
                    CalInfor.DarkLimitPrv=    t3;
                    CalInfor.DarkLimitCap=    t4;
*/
            }
            else if (!strncmp(pcCmdStr,"chroma",6))
            {
                debug_err(("chroma filter test...\r\n"));
                //Chroma_filter_test();
            }
            else if (!strncmp(pcCmdStr,"chksie io",9))
            {
                UINT32 HD_Phase = ((*(volatile UINT32*)0x9a000080) & 0x00000010) >>4;
                UINT32 VD_Phase = ((*(volatile UINT32*)0x9a000080) & 0x00000008) >>3;
                UINT32 Data_Phase = ((*(volatile UINT32*)0x9a000080) & 0x00000020) >>5;
                UINT32 HD_Invert = ((*(volatile UINT32*)0x9a00001c) & 0x80000000) >> 31;
                UINT32 VD_Invert = ((*(volatile UINT32*)0x9a000018) & 0x80000000) >> 31;
                UINT32 HD_Sync = ((*(volatile UINT32*)0x9a00001c) & 0x7FFF0000) >>16;
                UINT32 VD_Sync = ((*(volatile UINT32*)0x9a000018) & 0x7FFF0000) >>16;
                UINT32 HD_Total = ((*(volatile UINT32*)0x9a00001c) & 0x0000FFFF);
                UINT32 VD_Total = ((*(volatile UINT32*)0x9a000018) & 0x0000FFFF);
                UINT32 ACT_x = ((*(volatile UINT32*)0x9a00004c) & 0x00003FFF);
                UINT32 ACT_y = ((*(volatile UINT32*)0x9a00004c) & 0xFFFF0000) >>16;
                UINT32 ACT_w = ((*(volatile UINT32*)0x9a000048) & 0x00003FFF);
                UINT32 ACT_h = ((*(volatile UINT32*)0x9a000048) & 0xFFFF0000) >>16;
                UINT32 CRP_x = ((*(volatile UINT32*)0x9a000034) & 0x00007FFF);
                UINT32 CRP_y = ((*(volatile UINT32*)0x9a000030) & 0x0000FFFF);
                UINT32 CRP_w = ((*(volatile UINT32*)0x9a000034) & 0x3FFF0000) >>16;
                UINT32 CRP_h = ((*(volatile UINT32*)0x9a000030) & 0xFFFF0000) >>16;
                UINT32 OB_x0 = ((*(volatile UINT32*)0x9a000060) & 0x00003FFF);
                UINT32 OB_y0 = ((*(volatile UINT32*)0x9a000060) & 0xFFFF0000) >>16;
                UINT32 OB_x1 = ((*(volatile UINT32*)0x9a000064) & 0x00003FFF);
                UINT32 OB_y1 = ((*(volatile UINT32*)0x9a000064) & 0xFFFF0000) >>16;
                UINT32 OUT_w = ((*(volatile UINT32*)0x9a00009c) & 0x00003FFC);
                debug_msg("SIE Inv    h,v   = (%d,%d)\r\n", HD_Invert, VD_Invert);
                debug_msg("SIE Phase  h,v,d = (%d,%d,%d)\r\n", HD_Phase, VD_Phase, Data_Phase);
                debug_msg("SIE VDHD s,s,t,t = (%d,%d,%d,%d)\r\n", HD_Sync+1, VD_Sync+1, HD_Total+1, VD_Total+1);
                debug_msg("SIE ACT  x,y,w,h = (%d,%d,%d,%d)\r\n", ACT_x, ACT_y, ACT_w+1, ACT_h+1);
                debug_msg("SIE CRP  x,y,w,h = (%d,%d,%d,%d)\r\n", CRP_x, CRP_y, CRP_w+1, CRP_h+1);
                debug_msg("SIE OB   x,y,w,h = (%d,%d,%d,%d)\r\n", OB_x0, OB_y0, OB_x1-OB_x0+1, OB_y1-OB_y0+1);
                debug_msg("SIE OUT      w   = (%d)\r\n", OUT_w);
            }
            else
                debug_msg("bad command or invalid parameters\r\n");
            break;

        case 'd':
            if (!strncmp(pcCmdStr,"drawca",6))
            {
                //DrawCAWindow();
            }
            else if (!strncmp(pcCmdStr,"drawva",6))
            {
                //DrawVAWindow();
            }
            else if(!strncmp(pcCmdStr,"dmaprot ",8))
            {
                UINT32 a,b,c,d;
                DMA_WRITEPROT_ATTR   writeProcAttr;
                sscanf(pcCmdStr+8,"%d %d %x %x",&c,&d,&a,&b);

                dma_disableWPFunc(DMA_WPSET_1);

                dma_clrWPStatus(DMA_WPSET_0, DMA_WPCH_ALL);
                dma_clrWPStatus(DMA_WPSET_1, DMA_WPCH_ALL);
                dma_clrWPStatus(DMA_WPSET_2, DMA_WPCH_ALL);
                if(c==0)
                    writeProcAttr.uiProtectSrc = DMA_WPSRC_DMA;                          // < === 硂琌ノㄓ盎代 module  DMA ㄓ糶硂竚
                else if(c==1)
                    writeProcAttr.uiProtectSrc = DMA_WPSRC_CPU;                          // < === 硂琌ノㄓ盎代 module  DMA ㄓ糶硂竚
                else
                    writeProcAttr.uiProtectSrc = DMA_WPSRC_ALL;                          // < === 硂琌ノㄓ盎代 module  DMA ㄓ糶硂竚

                if(d==0)
                    writeProcAttr.uiProtectlel = DMA_WPLEL_UNWRITE;                   // <=== 硂琌ノㄓň糶, 把计 DMA_WPLEL_DETECT  盎代
                else
                    writeProcAttr.uiProtectlel = DMA_WPLEL_DETECT;                   // <=== 硂琌ノㄓň糶, 把计 DMA_WPLEL_DETECT  盎代

                writeProcAttr.uiStartingAddr = a;                             // <==== 叫癘眔硂璶琌 2 words alignment
                writeProcAttr.uiSize= b;                                                           // < ==== Size
                dma_configWPFunc(DMA_WPSET_1, &writeProcAttr, NULL);        // <=== ノ硂 API config  DRAM竚 / size / ň糶把计
                dma_enableWPFunc(DMA_WPSET_1);                                                // < === Enable 硂
                debug_msg("dmaprotect open %x %x,mode (%d,%d)\r\n",a,b,c,d);
            }
            else
            {
                debug_msg("bad command or invalid parameters\r\n");
            }
            break;

        case 'f':
            if (!strncmp(pcCmdStr,"fd",2))
            {
                debug_err(("fd test...\r\n"));
                //fd_test();
            }
            else if(!strncmp(pcCmdStr,"func ",5))
            {
                UINT32 a,b;
                sscanf(pcCmdStr+5,"%d %d\r\n",&a,&b);
                SetIPPFuncInfor(a,b);
                debug_msg("IPPFUNC %d is %d\r\n",a,b);
            }
            else if(!strncmp(pcCmdStr,"flip ", 5))
            {
                UINT32 a;
                PImageAlgInfo pimginfo;
                sscanf(pcCmdStr+5,"%d",&a);
                UI_SetParameter(_ImageFlip, a);
                pimginfo = UI_GetParameter();
                debug_msg("%d\r\n",pimginfo->ImageFlip);
            }
/*
            else if(!strncmp(pcCmdStr,"fraw ", 5))
            {
                UINT32 addr,size;
                int fsStatus;
                char filename[64];
                static unsigned long fcount=0;
                sprintf(filename, "A:\\FPGA\\F_%d.RAW",fcount);
                sscanf(pcCmdStr+5,"%x %x",&addr,&size);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename,
                    (UINT8*)addr,
                    &size,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getsieraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                fcount++;
            }
*/
            break;

        case 'g':
            if (!strncmp(pcCmdStr,"getsieraw",9))
            {
                int fsStatus;
                char filename1[64],filename2[64] ;
                static UINT32  length;
                sprintf(filename1, "A:\\FPGA\\SIE0.RAW");
                sprintf(filename2, "A:\\FPGA\\SIE1.RAW");
                length = GetIPPSizeInfor(_IpeOutVsize) * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getsieraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = GetIPPSizeInfor(_IpeOutVsize) * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename2,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getsieraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                debug_msg("Get SIE RAW(%dx%d)...\r\n",GetIPPSizeInfor(_IpeOutVsize),(GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF));
                debug_msg("SIE RAW ADDR %8x %8x \r\n",ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1),ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2));
            }
            else if (!strncmp(pcCmdStr,"getsiesliceraw",14))
            {
                int fsStatus;
                char filename1[64],filename2[64],filename3[64] ;
                static UINT32  length;
                sprintf(filename1, "A:\\FPGA\\SIE0.RAW");
                sprintf(filename2, "A:\\FPGA\\SIE1.RAW");
                sprintf(filename3, "A:\\FPGA\\SIE2.RAW");
                length = Ipp_Param.Sie_Struct->SieSliceHight * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getsieraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = Ipp_Param.Sie_Struct->SieSliceHight * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename2,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getsieraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = Ipp_Param.Sie_Struct->SieSliceHight * (GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename3,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_SIERAW_3),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                debug_msg("Get SIE RAW(%dx%d)...\r\n",Ipp_Param.Sie_Struct->SieSliceHight,(GetIPPSizeInfor(_IpeOutHsize) + IPE_HSIZE_IODIFF));
                debug_msg("SIE RAW ADDR %8x %8x \r\n",ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1),ippbuf_GetBuf(IPPBUF_PRV_SIERAW_2));
            }
            else if (!strncmp(pcCmdStr,"getimeraw1",10))
            {
                int fsStatus;
                char filename1[64],filename2[64],filename3[64] ;
                static UINT32  length;
                UINT32 _shift = 0;
                sprintf(filename1, "A:\\FPGA\\IME0.RAW");
                sprintf(filename2, "A:\\FPGA\\IME1.RAW");
                sprintf(filename3, "A:\\FPGA\\IME2.RAW");
                switch(Ipp_Param.Ime_Struct.pIme_fbaddr->out_format)
                {
                    case IME_OUTPUT_YCC_422_COS:
                    case IME_OUTPUT_YCC_422_CEN:
                        _shift = 1;
                        break;
                    case IME_OUTPUT_YCC_420_COS:
                    case IME_OUTPUT_YCC_420_CEN:
                        _shift = 2;
                        break;
                    default:
                        debug_err(("getimeraw outfmt error(%d)\r\n",Ipp_Param.Ime_Struct.pIme_fbaddr->out_format));
                        break;
                }
                length = (GetIPPSizeInfor(_ImeOut1Vsize) * GetIPPSizeInfor(_ImeOut1LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P1),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (GetIPPSizeInfor(_ImeOut1Vsize) * GetIPPSizeInfor(_ImeOut1LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename2,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P1_2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (GetIPPSizeInfor(_ImeOut1Vsize) * GetIPPSizeInfor(_ImeOut1LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename3,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P1_3),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                debug_msg("Get IME RAW(%dx%d(%d))...\r\n",GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset) ,Ipp_Param.Ime_Struct.pIme_fbaddr->out_format);
                debug_msg("IME RAW ADDR %8x %8x %8x\r\n",ippbuf_GetBuf(IPPBUF_PRV_IME_P1),ippbuf_GetBuf(IPPBUF_PRV_IME_P1_2),ippbuf_GetBuf(IPPBUF_PRV_IME_P1_3));
            }
            else if (!strncmp(pcCmdStr,"getimeraw2",10))
            {
                int fsStatus;
                char filename1[64],filename2[64],filename3[64],filename4[64];
                static UINT32  length;
                UINT32 _shift = 0;
                sprintf(filename1, "A:\\FPGA\\IME0.RAW");
                sprintf(filename2, "A:\\FPGA\\IME1.RAW");
                sprintf(filename3, "A:\\FPGA\\IME2.RAW");
                sprintf(filename4, "A:\\FPGA\\IME3.RAW");
                switch(Ipp_Param.Ime_Struct2.pIme_fbaddr->out_format)
                {
                    case IME_OUTPUT_YCC_422_COS:
                    case IME_OUTPUT_YCC_422_CEN:
                        _shift = 1;
                        break;
                    case IME_OUTPUT_YCC_420_COS:
                    case IME_OUTPUT_YCC_420_CEN:
                        _shift = 2;
                        break;
                    default:
                        debug_err(("getimeraw outfmt error(%d)\r\n",Ipp_Param.Ime_Struct2.pIme_fbaddr->out_format));
                        break;
                }
                length = (GetIPPSizeInfor(_ImeOut2Vsize) * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (GetIPPSizeInfor(_ImeOut2Vsize) * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename2,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (GetIPPSizeInfor(_ImeOut2Vsize) * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename3,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_3),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (GetIPPSizeInfor(_ImeOut2Vsize) * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename4,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_4),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                debug_msg("Get IME RAW(%dx%d(%d))...\r\n",GetIPPSizeInfor(_ImeOut2Vsize),GetIPPSizeInfor(_ImeOut2LineOffset) ,Ipp_Param.Ime_Struct2.pIme_fbaddr->out_format);
                debug_msg("IME RAW ADDR %8x %8x %8x %8x\r\n",ippbuf_GetBuf(IPPBUF_PRV_IME_P2),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_2),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_3),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_4));
            }
            else if (!strncmp(pcCmdStr,"getimesliceraw",14))
            {
                int fsStatus;
                char filename1[64],filename2[64],filename3[64],filename4[64] ;
                static UINT32  length;
                UINT32 _shift = 0;
                sprintf(filename1, "A:\\FPGA\\IME_S0.RAW");
                sprintf(filename2, "A:\\FPGA\\IME_S1.RAW");
                sprintf(filename3, "A:\\FPGA\\IME_S2.RAW");
                sprintf(filename4, "A:\\FPGA\\IME_s3.RAW");
                switch(Ipp_Param.Ime_Struct2.pIme_fbaddr->out_format)
                {
                    case IME_OUTPUT_YCC_422_COS:
                    case IME_OUTPUT_YCC_422_CEN:
                        _shift = 1;
                        break;
                    case IME_OUTPUT_YCC_420_COS:
                    case IME_OUTPUT_YCC_420_CEN:
                        _shift = 2;
                        break;
                    default:
                        debug_err(("getimeraw outfmt error(%d)\r\n",Ipp_Param.Ime_Struct.pIme_fbaddr->out_format));
                        break;
                }
                length = (IME_D2_Para_ext.p1_out_slice_height * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (IME_D2_Para_ext.p1_out_slice_height * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename2,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_2),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (IME_D2_Para_ext.p1_out_slice_height * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename3,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_3),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                length = (IME_D2_Para_ext.p1_out_slice_height * GetIPPSizeInfor(_ImeOut2LineOffset));
                length += length/_shift;
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename4,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_PRV_IME_P2_4),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);

                debug_msg("Get IME RAW(%dx%d(%d))...\r\n",IME_D2_Para_ext.p1_out_slice_height , GetIPPSizeInfor(_ImeOut2LineOffset) ,Ipp_Param.Ime_Struct.pIme_fbaddr->out_format);
                debug_msg("IME RAW ADDR %8x %8x %8x %8x\r\n",ippbuf_GetBuf(IPPBUF_PRV_IME_P2),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_2),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_3),ippbuf_GetBuf(IPPBUF_PRV_IME_P2_4));
            }
            else if (!strncmp(pcCmdStr,"getstamp",8))
            {
                UINT32  length = 480*64*2;
                UINT32 fsStatus;
                char filename[64];
                DS_STAMP_INFOR dsinfor;
                datastamp_GetStampInfor(&dsinfor);
                sprintf(filename, "A:\\FPGA\\stamp.RAW");
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename,
                    (UINT8*)dsinfor.AddrY,
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getimeraw - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);

                debug_msg("Get stamp(0X%x)...\r\n",dsinfor.AddrY);
            }
            else if(!strncmp(pcCmdStr,"getcaraw",8))
            {
                int fsStatus;
                char filename1[64];
                static UINT32  length;
                static UINT32 cacnt=0;
                sprintf(filename1, "A:\\FPGA\\CA%ld.RAW",cacnt);
                cacnt++;
                length = GetIPPSizeInfor(_CALineOffset)*GetIPPSizeInfor(_CAVsize);
                fsStatus = FilesysReadWriteByName2(
                    FST_CMD_WRITE_BY_NAME,
                    filename1,
                    (UINT8*)ippbuf_GetBuf(IPPBUF_CA),
                    &length,
                    0,
                    FST_TIME_NO_WAIT);
                if(fsStatus != FST_STA_OK)
                {
                    debug_msg("getcarst - card write error!!");
                }
                FilesysWaitCmdFinish(FST_TIME_INFINITE);
                debug_msg("Get CA RAW_%d X %d(0x%x)\r\n",GetIPPSizeInfor(_CALineOffset),GetIPPSizeInfor(_CAVsize),ippbuf_GetBuf(IPPBUF_CA));
            }
            break;

        case 'i':
            if (!strncmp(pcCmdStr,"i0",2))
                {
                    debug_msg("IRIS 0 ......\r\n");
                    Lens_Aperture_Move(0);
                }
            else if (!strncmp(pcCmdStr,"i1",2))
                {
                    debug_msg("IRIS 1 ......\r\n");
                    Lens_Aperture_Move(1);
                }
            else if (!strncmp(pcCmdStr,"i2",2))
                {
                    debug_msg("IRIS 2 ......\r\n");
                    Lens_Aperture_Move(2);
                }
            else if(!strncmp(pcCmdStr,"ipl ", 4))
            {
                IPL_OBJ IplObj;
                IME_OUTFMT uiImgFmt = IMEOUT2_FMT422_2;
                UINT a,b,c,d;
                static UINT32 orgratio;
                sscanf(pcCmdStr+4,"%d %d %d %d",&a,&b,&c,&d);
                IplObj.uiCmd = IPL_CMD_CHG_MODE;
                IplObj.Mode = a;
                IplObj.uiDzoomStep = Get_DzoomIndex();
                IplObj.uiImeOutWidth1 = 320;//GetDisplayH();
                IplObj.uiImeOutHeight1 = 240;//GetDisplayV();
                IplObj.uiImeOutYoft1 = 320;//GetDisplayH();
                if(a == IPL_MODE_VIDEOREC)
                {
                    uiImgFmt = IMEOUT2_FMT420_2;
                    IplObj.uiImeOutWidth2 = b;
                    IplObj.uiImeOutHeight2 = c;
                    IplObj.uiImeOutYoft2 = b;
                    orgratio = UI_GetParameter()->ImageRatio;
                    UI_SetParameter(_ImageRatio, d);
                }

                if(ippbuf_GetBuf(IPPBUF_PRV_START)==0)
                {
                    ippbuf_SetBuf(IPPBUF_PRV_START,0x31c860);
                    ippbuf_SetBuf(IPPBUF_PRV_END,0x9d9860);
                }
                debug_msg("fpga ipl - %d\r\n",IplObj.Mode);
                //#NT#2011/06/13#Ethan Lau -begin
                //#NT#add IPL img fmt
                IplObj.uiImgFmt = uiImgFmt;
                //#NT#2011/06/13#Ethan Lau -end
                UI_SetParameter(_ImageRatio, orgratio);
                if(IPL_SetCmd(&IplObj) == E_BOVR)
                {
                    uiImgFmt = IMEOUT2_FMT420_2;
                    IplObj.uiImgFmt = uiImgFmt;
                    IPL_SetCmd(&IplObj);
                }
                debug_msg("^R ~~~~~~~~~~~~~~~~~~~\r\n");
            }
            else
                debug_msg("bad command or invalid parameters\r\n");
            break;

        case 'n':    // switch mode
        break;

        case 'r':
            if (!strncmp(pcCmdStr,"r ",2))
            {
                RegisterRead(pcCmdStr + 2);
            }
            else
                debug_msg("bad command or invalid parameters\r\n");
            break;
        case 'u':
            if (!strncmp(pcCmdStr,"ui ",3))
            {
                UINT32 a,b;
                sscanf(pcCmdStr+3,"%d %d",&a,&b);
                UI_SetParameter(a, b);
                debug_msg("ui: %d %d\r\n",a,b);
            }
            else
                debug_msg("bad command or invalid parameters\r\n");
            break;
            break;
        case 't':
            if(!strncmp(pcCmdStr, "test ", 5))
            {
            }
            else
                debug_msg("bad command or invalid parameters\r\n");

            break;

        case 'w':
            if(!strncmp(pcCmdStr, "w ", 2))
            {
                RegisterWrite(pcCmdStr + 2);
            }
            else
            {
                debug_msg("bad command or invalid parameters\r\n");
            }
            break;

        case 'z':
            if (!strncmp(pcCmdStr,"zit",3))
            {
                for(i=10;i<=Get_DzoomMaxIndex();i++)
                {
                    Set_DzoomIndex(i);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                        SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());
                }
            }
            else if (!strncmp(pcCmdStr,"zot",3))
            {
                for(i=Get_DzoomMaxIndex();i>=10;i--)
                {
                    Set_DzoomIndex(i);
                    //Set_digitalzoom(zoom_index);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                        SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());
                }
            }
            else if (!strncmp(pcCmdStr,"zis",3))
            {
                    INT index;
                    index = Get_DzoomIndex() + 1;
                    Set_DzoomIndex(index);
                    if(Get_DzoomIndex()<10)
                       Set_DzoomIndex(10);
                    if(Get_DzoomIndex()>Get_DzoomMaxIndex())
                       Set_DzoomIndex(Get_DzoomMaxIndex());



                    //Set_digitalzoom(zoom_index);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                       SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());
                    //aaa_AwbLock(UNLOCK);
            }
            else if (!strncmp(pcCmdStr,"zos",3))
            {
                    INT index;
                    index = Get_DzoomIndex() - 1;
                    Set_DzoomIndex(index);
                    if(Get_DzoomIndex()<10)
                       Set_DzoomIndex(10);
                    if(Get_DzoomIndex()>Get_DzoomMaxIndex())
                       Set_DzoomIndex(Get_DzoomMaxIndex());

                    //Set_digitalzoom(zoom_index);
                    if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                       SetDzoom2Video(Get_DzoomIndex());
                    else
                        SetDzoom2Preview(Get_DzoomIndex());

                    //aaa_AwbLock(UNLOCK);
            }
            else if(!strncmp(pcCmdStr,"ztest ",6))
            {
                UINT32 times,j;
                sscanf(pcCmdStr+6,"%d",&times);
                for(j = 0;j<times;j++)
                {
                    for(i=10;i<=Get_DzoomMaxIndex();i++)
                    {
                        Set_DzoomIndex(i);
                        if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                            SetDzoom2Video(Get_DzoomIndex());
                        else
                            SetDzoom2Preview(Get_DzoomIndex());
                    }
                    for(i=Get_DzoomMaxIndex();i>=10;i--)
                    {
                        Set_DzoomIndex(i);
                        //Set_digitalzoom(zoom_index);
                        if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                            SetDzoom2Video(Get_DzoomIndex());
                        else
                            SetDzoom2Preview(Get_DzoomIndex());
                    }
                }
            }
            else if(!strncmp(pcCmdStr,"zgoto ",6))
            {
                UINT32 idx;
                sscanf(pcCmdStr+6,"%d",&idx);
                Set_DzoomIndex(idx);
                if(gAlgMsg.Alg_Mode == MODE_Video_Recording)
                   SetDzoom2Video(Get_DzoomIndex());
                else
                   SetDzoom2Preview(Get_DzoomIndex());
            }
            else
                debug_msg("bad command or invalid parameters\r\n");
        break;

    }
}

#endif // EOF
