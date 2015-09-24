#include "ImageQualitySim_int.h"
#include "FileSysTsk.h"

IQSim_SliceSizeInfo_Out _OutInfo;
IQSim_SliceSizeInfo_In _InInfo;

////////////////////////////////////////
//for auto-slice sie/ipe/ime in/out size config
//#NT#2011/02/14#Yuppie Lin -begin
//#NT#modify SliceSize Gen to more condition
int IQSim_SliceSize_Gen(IQSim_SliceSizeInfo_Out* _pOutInfo, IQSim_SliceSizeInfo_In* _pInInfo)
{
    UINT32  IpeInH, IpeInV;
    UINT32  ImeInH, ImeInV;
    UINT32  ImeOutH, ImeOutV;

    IpeInH = _pInInfo->IpeInH;
    IpeInV = _pInInfo->IpeInV;
    ImeInH = IpeInH - 16;
    ImeInV = IpeInV;// - 8;
    ImeOutH = _pInInfo->ImeOutH;
    ImeOutV = _pInInfo->ImeOutV;

    if((ImeInH % 16) != 0)
    {
        debug_err(("^RIME horizontal input size must be 16x!!\r\n"));
        return 0;
    }
    if(_pInInfo->ImeMode == IPE2IME_FRAME_MODE) {

        _pInInfo->ImeSliceH = 4096;
    }
    //#NT#2011/02/15#ethanlau -begin
    //#NT#add ime slice mode or not
    if((_pInInfo->ImeMode == IPE2IME_SLICE_MODE)||(_pInInfo->ImeMode == DRAM2IME_SLICE_MODE))
    {
        if(ImeOutV < _pInInfo->ImeSliceH)
        {
            debug_err(("^RDoubt for IME Slice Height setting may be due to output size of vertical smaller than the Slice-Height\r\n"));
            return 0;
        }
    }
    //#NT#2011/02/15#ethanlau -end

    //IME Hstripe Calculate
    if((ImeInH >> 4) <= (22 - 1))//single
    {
        //_pOutInfo->Ime_Hstripe.n = 0;
        //_pOutInfo->Ime_Hstripe.m = 0;
        //_pOutInfo->Ime_Hstripe.l = ImeInH >> 4;

        _pOutInfo->Ime_Hstripe.n = (ImeInH >> 5) +1;
        _pOutInfo->Ime_Hstripe.m = 1;
        _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
    }
    else//multi
    {
        _pOutInfo->Ime_Hstripe.n = 21;//Max linebuffer => 336/16 = 21
        _pOutInfo->Ime_Hstripe.m = (ImeInH >> 4) / (_pOutInfo->Ime_Hstripe.n - 1);
        _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
        while(((_pOutInfo->Ime_Hstripe.l) < 8) && (_pOutInfo->Ime_Hstripe.n))
        {
            _pOutInfo->Ime_Hstripe.n --;
            _pOutInfo->Ime_Hstripe.m = (ImeInH >> 4) / (_pOutInfo->Ime_Hstripe.n - 1);
            _pOutInfo->Ime_Hstripe.l = (ImeInH >> 4) - ((_pOutInfo->Ime_Hstripe.n - 1) * _pOutInfo->Ime_Hstripe.m);
        }
    }

    //IME Vstripe Calculate
    if(ImeOutV <= _pInInfo->ImeSliceH)
    {
        if((_pInInfo->IpeMode == IPE_AUTOSLICE) && (ImeInV > (_pInInfo->Sie_SliceH_Default - 8))){
            _pOutInfo->Ime_Vstripe.n = _pInInfo->Sie_SliceH_Default - 8;
            _pOutInfo->Ime_Vstripe.n -= (_pOutInfo->Ime_Vstripe.n & 0x1);
            _pOutInfo->Ime_Vstripe.m = ImeInV / (_pOutInfo->Ime_Vstripe.n - 4);
            _pOutInfo->Ime_Vstripe.l = ImeInV - (_pOutInfo->Ime_Vstripe.n - 4) * _pOutInfo->Ime_Vstripe.m;
            while(((_pOutInfo->Ime_Vstripe.l) < 8) && (_pOutInfo->Ime_Vstripe.n))
            {
                _pOutInfo->Ime_Vstripe.m --;
                _pOutInfo->Ime_Vstripe.l += _pOutInfo->Ime_Vstripe.n - 4;
            }
            _pOutInfo->Ime_Vstripe.n--;
            _pOutInfo->Ime_Vstripe.l--;
        }
        else {
            _pOutInfo->Ime_Vstripe.n = 0;
            _pOutInfo->Ime_Vstripe.m = 0;
            _pOutInfo->Ime_Vstripe.l = ImeInV - 1;
        }
    }
    else
    {
        _pOutInfo->Ime_Vstripe.n = ((ImeInV * _pInInfo->ImeSliceH + (ImeOutV - 1)) / ImeOutV) + 4;
        _pOutInfo->Ime_Vstripe.n += (_pOutInfo->Ime_Vstripe.n & 0x1);
        _pOutInfo->Ime_Vstripe.m = ImeInV / (_pOutInfo->Ime_Vstripe.n - 4);
        _pOutInfo->Ime_Vstripe.l = ImeInV - (_pOutInfo->Ime_Vstripe.n - 4) * _pOutInfo->Ime_Vstripe.m;
        while(((_pOutInfo->Ime_Vstripe.l) < 8) && (_pOutInfo->Ime_Vstripe.n))
        {
            _pOutInfo->Ime_Vstripe.m --;
            _pOutInfo->Ime_Vstripe.l += _pOutInfo->Ime_Vstripe.n - 4;
        }
        _pOutInfo->Ime_Vstripe.n--;
        _pOutInfo->Ime_Vstripe.l--;
    }

    //IPE Hstripe Calculate
    if(_pOutInfo->Ime_Hstripe.m == 0)
    {
        _pOutInfo->Ipe_Hstripe.n = 0;
        _pOutInfo->Ipe_Hstripe.m = 0;
        _pOutInfo->Ipe_Hstripe.l = _pOutInfo->Ime_Hstripe.l + 1;
    }
    else
    {
        _pOutInfo->Ipe_Hstripe.n = _pOutInfo->Ime_Hstripe.n + 1;
        _pOutInfo->Ipe_Hstripe.m = _pOutInfo->Ime_Hstripe.m;
        _pOutInfo->Ipe_Hstripe.l = _pOutInfo->Ime_Hstripe.l + 1;
    }

    //IPE Vstripe Calculate
    if(_pOutInfo->Ime_Vstripe.m == 0)
    {
        _pOutInfo->Ipe_Vstripe.n = 0;
        _pOutInfo->Ipe_Vstripe.m = 0;
        _pOutInfo->Ipe_Vstripe.l = _pOutInfo->Ime_Vstripe.l;
    }
    else
    {
        _pOutInfo->Ipe_Vstripe.n = _pOutInfo->Ime_Vstripe.n + 8;
        _pOutInfo->Ipe_Vstripe.m = _pOutInfo->Ime_Vstripe.m;
        _pOutInfo->Ipe_Vstripe.l = _pOutInfo->Ime_Vstripe.l;
    }
    if(_pInInfo->IpeMode == IPE_AUTOSLICE) 
    {
        if(_pInInfo->Sie_SliceH_Default < _pOutInfo->Ipe_Vstripe.n)
        {
            debug_err(("SIE Slice is too small, %d is needed\r\n",_pOutInfo->Ipe_Vstripe.n));

            _pOutInfo->Sie_SliceH = _pOutInfo->Ipe_Vstripe.n;
        }
        else
        {
            _pOutInfo->Sie_SliceH = (IpeInV + 2) / 3;
            if(_pOutInfo->Sie_SliceH > _pInInfo->Sie_SliceH_Default)
            {
               _pOutInfo->Sie_SliceH = _pInInfo->Sie_SliceH_Default;
            }
            else if(_pOutInfo->Sie_SliceH  < _pOutInfo->Ipe_Vstripe.n)
            {
                _pOutInfo->Sie_SliceH = _pOutInfo->Ipe_Vstripe.n;
            }
        }
        _pOutInfo->Sie_SliceH += ( _pOutInfo->Sie_SliceH & 0x1);
    }
    return 1;
}
//#NT#2011/02/14#Yuppie Lin -end

////////////////////////////////////////

//use to show IPP debug msg when writing register info. of engines
//this variable is related to IPP control flow


void IQSim_DumpEngine(char *Filename, IQSim_EngineSel DbgEngine)
{
    UINT32 filesize    = 0;
    UINT32 EngBaseAddr = 0x0;
    int WriteFileStatus;

    switch(DbgEngine)
    {
    case IQSim_SIE:
        EngBaseAddr = (UINT32)IQSim_SIE_ADDR;
        filesize    = 4 * (UINT32)IQSim_RegNum_SIE;
        break;

    case IQSim_IPE:
        EngBaseAddr = (UINT32)IQSim_IPE_ADDR;
        filesize    = 4 * (UINT32)IQSim_RegNum_IPE;
        break;
        
    case IQSim_IME:
        EngBaseAddr = (UINT32)IQSim_IME_ADDR;
        filesize    = 4 * (UINT32)IQSim_RegNum_IME;
        break;
        
    case IQSim_SRA:
        EngBaseAddr = (UINT32)IQSim_SRA_ADDR;
        filesize    = 4 * (UINT32)IQSim_RegNum_SRA;
        break;

    case IQSim_IFE:
        EngBaseAddr = (UINT32)IQSim_IFE_ADDR;
        filesize    = 4 * (UINT32)IQSim_RegNum_IFE;
        break;

    default:
        debug_err(("^R[ IPP Debug ] - Engine Selection error\r\n"));
        break;        
    }

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    WriteFileStatus = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, 
        Filename, 
        (UINT8*)EngBaseAddr, 
        &filesize, 
        0, 
        FST_TIME_INFINITE);
}

#if (CAP_DEBUG_DUMP == ENABLE)
IQSIM_BIN dumpBin = {0};
#endif

#if (BIN_DEBUG_CONV == ENABLE)
IQSIM_BIN2 dumpBin2 = {0};
#endif

void IQSim_DumpBin(char *Filename, IQSIM_BIN* pBin)
{
    UINT32 filesize    = sizeof(IQSIM_BIN);
    int WriteFileStatus;

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    WriteFileStatus = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, 
        Filename, 
        (UINT8*)pBin, 
        &filesize, 
        0, 
        FST_TIME_INFINITE);
}

#if (BIN_DEBUG_CONV == ENABLE)
void IQSim_DumpBin2(char *Filename, IQSIM_BIN* pBin, IQSIM_BIN2* pBin2)
{
    UINT32 filesize    = sizeof(IQSIM_BIN2);
    int WriteFileStatus;

    debug_err(("BAS=%d\r\n", sizeof(NT96220_BASIC_IO_REG_STRUCT)));
    debug_err(("SIE=%d\r\n", sizeof(NT96220_SIE_REG_STRUCT)));
    debug_err(("SRA=%d\r\n", sizeof(NT96220_SRA_REG_STRUCT)));
    debug_err(("IFE=%d\r\n", sizeof(NT96220_IFE_REG_STRUCT)));
    debug_err(("IPE=%d\r\n", sizeof(NT96220_IPE_REG_STRUCT)));
    
    memcpy(&(pBin2->Basic), &(pBin->Basic), sizeof(NT96220_BASIC_IO_REG_STRUCT));
    memcpy(&(pBin2->Sie), &(pBin->Sie), sizeof(NT96220_SIE_REG_STRUCT));
    memcpy(&(pBin2->SieGamma), &(pBin->SieGamma), sizeof(NT96220_SRA_REG_STRUCT));
    memcpy(&(pBin2->IfeRaw), &(pBin->IfeRaw), sizeof(NT96220_IFE_REG_STRUCT));
    memcpy(&(pBin2->Ipe), &(pBin->Ipe), sizeof(NT96220_IPE_REG_STRUCT));
    memcpy(&(pBin2->IfeY), &(pBin->IfeY), sizeof(NT96220_IFE_REG_STRUCT));
    memcpy(&(pBin2->IfeCC), &(pBin->IfeCC), sizeof(NT96220_IFE_REG_STRUCT));
    memcpy(&(pBin2->IfeCCCrv), &(pBin->IfeCCCrv), sizeof(NT96220_SRA_REG_STRUCT));

    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    WriteFileStatus = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME, 
        Filename, 
        (UINT8*)pBin2, 
        &filesize, 
        0, 
        FST_TIME_INFINITE);
}
#endif

char g_sLogName[256] = {0};
__align(4) char g_sLogBuf[1024] = {0};

void slog_open(char *pFilename)
{
    ER Return = E_OK;
    UW BufSize = 0;
    CHAR buf[4];
    if(!pFilename)
        return;
    strcpy(g_sLogName, pFilename);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    sprintf(g_sLogBuf, "//This c file is dump by IQ Sim tool.\r\n\r\n");
    //debug_err(("SLOG [open]\r\n"));
    //debug_err(("SLOG %s", g_sLogBuf));

    //check file is exist or not
    BufSize = 4;
    Return = FilesysReadWriteByName2(FST_CMD_READ_BY_NAME,
       g_sLogName,
       (UB*)buf,
       &BufSize,
       0,
       FST_TIME_INFINITE);
    if(Return == FST_STA_OK)
    {
        //delete old file
        FilesysDeleteByName(g_sLogName, FALSE);
    }        

    //create new file
    BufSize = strlen(g_sLogBuf);
    Return = FilesysReadWriteByName2(FST_CMD_WRITE_BY_NAME,
       g_sLogName,
       (UB*)g_sLogBuf,
       &BufSize,
       0,
       FST_TIME_INFINITE);
}
void slog_out(void)
{
    ER Return = E_OK;
    UW BufSize = 0;
    if(g_sLogName[0] == 0)
        return;
    if(BufSize > 255)
        return;
    //debug_err(("SLOG %s", g_sLogBuf));
    BufSize = strlen(g_sLogBuf);
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    Return = FilesysReadWriteByName2(FST_CMD_WRITE_APP_BY_NAME,
       g_sLogName,
       (UB*)g_sLogBuf,
       &BufSize,
       0,
       FST_TIME_INFINITE);
}            
void slog_close(void)
{
    //debug_err(("SLOG [close]\r\n"));
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    FilesysCloseActFile();
    g_sLogName[0] = 0;
}


