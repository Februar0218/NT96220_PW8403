
#include <stdio.h>
#include <string.h>
#include "Debug.h"
#include "Calibration.h"
#include "dpc_alg.h"
#include "ImageAlgInfor.h"
#include "sie_lib.h"
#include "ippbuffer.h"

#define DPC_DEBUG  DISABLE

#define DPC_MSG(...)    debug_msg ("DPC: "__VA_ARGS__)

#if (DPC_DEBUG)
#define DPC_ERR(...)    debug_msg ("DPC ERROR: "__VA_ARGS__)
#else
#define DPC_ERR(...)
#endif

#define DPC_HISTO_BIN_NUM   64
#define SIE_TOTALFIELDS     1

static DPC_SEARCH gSearchInfo;

static INT32 cal_abs(INT32 i)
{
    if (i >= 0)
    {
        return i;
    }
    return (-i);
}

static void DPC_Bayer_getHisto(UINT16 *puiHisto, DPC_SEARCH SearchInfo)
{
    UINT16 i,j,X,Y, m, n;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY,ExtraX,ExtraY;
    UINT16 blockX, blockY;
    int Diff;
    UINT32 uiBuf;
    UINT8 ucBuf;
    UINT32 Yadd, Xadd, Jadd, Iadd, JaddIncrement;
    UINT8 BayInd;
    UINT32 BayAvg[4], BaySum[4];

    {//compute Blocking parameters
        EndBlockX = SearchInfo.Width / SearchInfo.Block_X;
        EndBlockY = SearchInfo.Height / SearchInfo.Block_Y;
        RemainX = SearchInfo.Width - (EndBlockX * SearchInfo.Block_X);
        RemainY = SearchInfo.Height - (EndBlockY * SearchInfo.Block_Y);
    }

    for(i = 0; i < DPC_HISTO_BIN_NUM * SearchInfo.CapFields; i++)
    {//clean Histogram
        puiHisto[i] = 0;
    }

    {//initial before j-loop
        Jadd = SearchInfo.BayAdd;
        JaddIncrement = SearchInfo.Block_Y * SearchInfo.LineOffset;
    }

    for(j = 0; j < EndBlockY; j++)
    {
        {//initial before i-loop
            Iadd = Jadd;
        }

        for(i=0;(i<EndBlockX);i++)
        {
            {//setting
                //block size
                if(i==(EndBlockX-1))
                    ExtraX=RemainX;
                else
                    ExtraX=0;
                if(j==(EndBlockY-1))
                    ExtraY=RemainY;
                else
                    ExtraY=0;
                blockY = SearchInfo.Block_Y + ExtraY;
                blockX = SearchInfo.Block_X + ExtraX;
                //summation buffer cleaning
                for(X=0;X<4;X++)
                    BaySum[X] = 0;
            }

            {//initial before Y-loop
                Yadd = Iadd;
                BayInd = 0x0;
            }

            for(Y=0;Y<blockY;Y++)
            {
                {//initial before X-loop
                    Xadd = Yadd;
                }

                {//X=0 Job
                    uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                    uiBuf = uiBuf>>((Xadd&0x3)<<3);//"<<3" = "*8"
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    {//loop accumulation
                        Xadd += 1;
                        BayInd = BayInd ^ 0x1;
                    }
                }

                for(X=1;X<blockX;X++)
                {
                    if(Xadd&0x3)
                        uiBuf = uiBuf>>8;
                    else
                        uiBuf = *((UINT32*)Xadd);
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    {//loop accumulation
                        Xadd += 1;
                        BayInd = BayInd ^ 0x1;
                    }
                }

                {//loop accumulation
                    Yadd += SearchInfo.LineOffset;
                    BayInd = BayInd ^ 0x2;
                }
            }

            {//averaging
                uiBuf = ((blockY)*(blockX))>>2;//"<<2" = "/4"
                for(X=0;X<4;X++)
                    BayAvg[X] = BaySum[X]/uiBuf;
            }

            {//initial before Y-loop
                Yadd = Iadd;
                BayInd=0x0;
            }

            for(Y=0;Y<blockY;Y++)
            {
                {//initial before X-loop
                    Xadd = Yadd;
                }

                for(X=0;X<blockX;X++)
                {
                    {//getting data in byte
                        if((!(Xadd&0x3)) || X==0)
                        {
                            uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                            if(X==0)
                                uiBuf = uiBuf>>(8*(Xadd&0x3));
                        }
                        else
                            uiBuf = uiBuf>>8;
                        ucBuf = uiBuf;
                    }

                    {//compute Diff
///                        if(environment==Status_Black)
///                            Diff=ucBuf-Avg;//don't mind dark pixel in the dark
///                        else if(environment==Status_White)
                            Diff = ucBuf-BayAvg[BayInd];
                            if(Diff<0)
                                Diff = -Diff;
                    }

                    {//check on histogram
                        m = (j*SearchInfo.Block_Y + Y + SearchInfo.OffsetY)%SearchInfo.CapFields;
                        n = (Diff<DPC_HISTO_BIN_NUM)?Diff:(DPC_HISTO_BIN_NUM-1);
                        if(puiHisto[m*DPC_HISTO_BIN_NUM+n]<0xffff)
                            puiHisto[m*DPC_HISTO_BIN_NUM+n]++;
                    }

                    {//loop accumulation
                        BayInd = BayInd ^ 0x1;
                        Xadd += 1;
                    }
                }//loop X

                {//loop accumulaiton
                    BayInd = BayInd ^ 0x2;
                    Yadd += SearchInfo.LineOffset;
                }
            }//loop Y

            {//loop accumulation
                Iadd += SearchInfo.Block_X;
            }

        }//loop i

        {//loop accumulation
            Jadd += JaddIncrement;
        }

    }//loop j
}

UINT32 DPC_Bayer_Search(DPC_SEARCH SearchInfo)
{
    UINT16 i,j,X,Y,m;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY,ExtraX,ExtraY;
    UINT16 blockX, blockY;
    INT Diff;
    UINT32 uiBuf, uiBuf2;
    UINT8 ucBuf;
    UINT32 Yadd, Xadd, Jadd, Iadd, JaddIncrement;
    UINT8 BayInd;
    UINT32 BayAvg[4], BaySum[4];
    UINT32 DPadd_Cur, DPadd_Tmp;
    UINT8 DP_Double_Check, DP_Double;
    UINT16 DP_Cnt;
    UINT32 returnValue;
    BOOL autoThreshold;
    UINT8 puiDynamicThreshold[SIE_TOTALFIELDS] = {0};
    UINT16 puiHisto[SIE_TOTALFIELDS*DPC_HISTO_BIN_NUM] = {0};
    UINT16 fieldStartCnt[SIE_TOTALFIELDS] = {0};

    autoThreshold = (SearchInfo.Threshold==0)?TRUE:FALSE;

    DPC_MSG("DPC_Bayer_Search: width=%d, height=%d\r\n", SearchInfo.Width, SearchInfo.Height);
    DPC_MSG("DPC_Bayer_Search: OffsetX=%d, OffsetY=%d\r\n", SearchInfo.OffsetX, SearchInfo.OffsetY);
    DPC_MSG("DPC_Bayer_Search: threshold=%d\r\n", SearchInfo.Threshold);

    if(autoThreshold)
    {
        DPC_Bayer_getHisto(puiHisto, SearchInfo);

        DPadd_Tmp = SearchInfo.DPadd;
        DPC_MSG("fieldStartCnt: \r\n");
        for(i = 0; i < SearchInfo.StartCnt; i++)
        {
            uiBuf = *(UINT32 volatile*)(DPadd_Tmp);
            uiBuf = (uiBuf>>16)&0xffff;
            fieldStartCnt[uiBuf%SearchInfo.CapFields]++;
            DPadd_Tmp += 4;
        }
        //debug_err(("%d %d %d %d", fieldStartCnt[0], fieldStartCnt[1], fieldStartCnt[2], fieldStartCnt[3]));
        //debug_err(("\r\n"));

        DPC_MSG("DPC_Bayer_getHisto: \r\n");
        /*
        for(j = 0; j < SIE_TOTALFIELDS; j++)
        {
            DPC_MSG("field %d :    ", j);
            m=0;
            for(i = 0; i < DPC_HISTO_BIN_NUM; i++)
            {
                DPC_MSG("%d ", puiHisto[j*DPC_HISTO_BIN_NUM+i]);
            }
            DPC_MSG("\r\n");
        }
        */
        DPC_MSG("\r\n");

        DPC_MSG("dynamicThreshold: \r\n");
        for(j=0; j<SIE_TOTALFIELDS; j++)
        {
            DPC_MSG("field %d : MAX %d   ", j, ((SearchInfo.MaxCnt-SearchInfo.StartCnt)/SIE_TOTALFIELDS-fieldStartCnt[j]));
            m=0;
            for(i=(DPC_HISTO_BIN_NUM-1); i>0; i--)
            {
                m+=puiHisto[j*DPC_HISTO_BIN_NUM+i];
                //debug_err(("<%d,%d>", m,((MaxCnt-StartCnt)/SIE_TOTALFIELDS-fieldStartCnt[j])));
                //if(m > ((SearchInfo.MaxCnt - SearchInfo.StartCnt) / SearchInfo.CapFields - fieldStartCnt[j]))
                if(m>(SearchInfo.MaxCnt-fieldStartCnt[j]))
                    break;
            }
            puiDynamicThreshold[j] = i;
            DPC_MSG("%d :#%d \r\n", i,m-puiHisto[j*DPC_HISTO_BIN_NUM+i]);
        }
        DPC_MSG("\r\n");

        SearchInfo.Threshold=puiDynamicThreshold[SearchInfo.CapFields - 1];
        DPC_MSG("Threshold: %d\r\n", SearchInfo.Threshold);
    }

    EndBlockX = SearchInfo.Width / SearchInfo.Block_X;
    EndBlockY = SearchInfo.Height / SearchInfo.Block_Y;
    RemainX = SearchInfo.Width - (EndBlockX * SearchInfo.Block_X);
    RemainY = SearchInfo.Height - (EndBlockY * SearchInfo.Block_Y);
    DP_Cnt = SearchInfo.StartCnt;
    DPadd_Cur = SearchInfo.DPadd + (SearchInfo.StartCnt << 2);
    if(SearchInfo.StartCnt)
    {
        DP_Double_Check=1;
    }
    else
    {
        DP_Double_Check=0;
    }

    Jadd = SearchInfo.BayAdd;
    JaddIncrement = SearchInfo.Block_Y * SearchInfo.LineOffset;

    for(j=0;(j<EndBlockY);j++)
    {
        //initial before i-loop
        Iadd = Jadd;
        for(i=0;(i<EndBlockX);i++)
        {
            //setting
            //block size
            if(i==(EndBlockX-1))
            {
                ExtraX=RemainX;
            }
            else
            {
                ExtraX=0;
            }
            if(j==(EndBlockY-1))
            {
                ExtraY=RemainY;
            }
            else
            {
                ExtraY=0;
            }
            blockY = SearchInfo.Block_Y + ExtraY;
            blockX = SearchInfo.Block_X + ExtraX;
            //summation buffer cleaning
            for(X=0;X<4;X++)
            {
                BaySum[X] = 0;
            }

            //initial before Y-loop
            Yadd = Iadd;
            BayInd=0x0;

            for(Y=0;Y<blockY;Y++)
            {
                //initial before X-loop
                Xadd = Yadd;

                //X=0 Job
                uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                uiBuf = uiBuf>>((Xadd&0x3)<<3);//"<<3" = "*8"
                ucBuf = uiBuf;
                BaySum[BayInd] += ucBuf;
                 //loop accumulation
                Xadd += 1;
                BayInd = BayInd ^ 0x1;

                for(X=1;X<blockX;X++)
                {
                    if(Xadd&0x3)
                    {
                        uiBuf = uiBuf>>8;
                    }
                    else
                    {
                        uiBuf = *((UINT32*)Xadd);
                    }
                    ucBuf = uiBuf;
                    BaySum[BayInd] += ucBuf;
                    //loop accumulation
                    Xadd += 1;
                    BayInd = BayInd ^ 0x1;
                }

                //loop accumulation
                Yadd += SearchInfo.LineOffset;
                BayInd = BayInd ^ 0x2;
            }

            //averaging
            uiBuf = ((blockY)*(blockX))>>2;//"<<2" = "/4"
            for(X=0;X<4;X++)
            {
                BayAvg[X] = BaySum[X]/uiBuf;
            }

            //initial before Y-loop
            Yadd = Iadd;
            BayInd = 0x0;

            for(Y=0;Y<blockY;Y++)
            {
                //initial before X-loop
                Xadd = Yadd;

                /*
                if(autoThreshold)
                {
                    m = (j*SearchInfo.Block_Y + Y + SearchInfo.OffsetY)%SearchInfo.CapFields;
                    SearchInfo.Threshold = puiDynamicThreshold[m];
                }
                */
                for(X=0;X<blockX;X++)
                {
                    //getting data in byte
                    if((!(Xadd&0x3)) || X==0)
                    {
                        uiBuf = *((UINT32*)(Xadd&0xfffffffc));
                        if(X==0)
                        uiBuf = uiBuf>>(8*(Xadd&0x3));
                    }
                    else
                    {
                        uiBuf = uiBuf>>8;
                    }
                    ucBuf = uiBuf;

                    //compute Diff
                    Diff = ucBuf-BayAvg[BayInd];
                    if(Diff<0)
                    {
                        Diff = -Diff;
                    }

                    if(Diff > SearchInfo.Threshold)
                    {
                        //recording DP found
                        uiBuf2 = ((((j*SearchInfo.Block_Y + Y + SearchInfo.OffsetY)<<16)&0xffff0000)|((i*SearchInfo.Block_X + X + SearchInfo.OffsetX)&0xffff));

                        DPC_ERR("DP%4d 0x%08x=(%4d,%3d) Val%3d Avg%3d Diff%3d\r\n",
                        DP_Cnt,
                        ((((j*SearchInfo.Block_Y+Y+SearchInfo.OffsetY)<<16)&0xffff0000)+((i*SearchInfo.Block_X+X+SearchInfo.OffsetX)&0xffff)),
                        (((j*SearchInfo.Block_Y+Y+SearchInfo.OffsetY))&0xffff), ((i*SearchInfo.Block_X+X+SearchInfo.OffsetX)&0xffff),
                        ucBuf, BayAvg[BayInd], Diff);

                        //DP double/DP repeat check
                        DP_Double=0;
                        if(DP_Double_Check)
                        {
                            DPadd_Tmp = SearchInfo.DPadd;
                            for(m = 0;m < SearchInfo.StartCnt; m++)
                            {
                                if(*(UINT32 volatile*)(DPadd_Tmp)==uiBuf2)
                                {
                                    DP_Double=1;
                                    break;
                                }
                                DPadd_Tmp += 4;
                            }
                        }

                        if(!DP_Double)
                        {
                            if(DP_Cnt >= SearchInfo.MaxCnt)
                            {
#if DP_SEARCH_TILL_END
                                goto DP_SEARCH_TILL_END_JUMP_END;
#else
                                goto DP_SEARCH_TOO_MUCH;
#endif
                            }

                            //update DP buffer
                            *(UINT32 volatile*)(DPadd_Cur)=uiBuf2;
#if DP_SEARCH_TILL_END
                            DP_SEARCH_TILL_END_JUMP_END:
#endif
                            //update parameters
                            DP_Cnt++;
                            DPadd_Cur += 4;
                        }
                    }

                    //loop accumulation
                    BayInd = BayInd ^ 0x1;
                    Xadd += 1;
                }//loop X

                //loop accumulaiton
                BayInd = BayInd ^ 0x2;
                Yadd += SearchInfo.LineOffset;
            }//loop Y

            //loop accumulation
            Iadd += SearchInfo.Block_X;
        }//loop i

        //loop accumulation
        Jadd += JaddIncrement;
    }//loop j
#if !DP_SEARCH_TILL_END
    DP_SEARCH_TOO_MUCH:
#endif
    returnValue = DP_Cnt - SearchInfo.StartCnt;
    return returnValue;
}

UINT32 DPC_VGA_Search(DPC_SEARCH SearchInfo)
{
    #define PATTERN_SIZE 4 //RGGB
    UINT32 RGGBAvg[PATTERN_SIZE], RGGBValue[PATTERN_SIZE], *puwDPCurAddr, uwTempValue;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY, ExtraX, ExtraY;
    UINT16 DP_Cnt, i , j, k, x, y;
    INT16 hDiff;
    UINT8 DP_Double_Check_Flag, CheckFlag, *pubBlockAddr, *pubTempAddr;

    //compute blocking parameter
    EndBlockX = SearchInfo.Width / SearchInfo.Block_X;
    EndBlockY = SearchInfo.Height / SearchInfo.Block_Y;
    RemainX = SearchInfo.Width - (EndBlockX * SearchInfo.Block_X);
    RemainY = SearchInfo.Height - (EndBlockY * SearchInfo.Block_Y);

    //set defect pixel address and recheck
    DP_Cnt = SearchInfo.StartCnt;
    puwDPCurAddr = (UINT32 *)SearchInfo.DPadd + DP_Cnt;
    if (SearchInfo.StartCnt)
    {
        DP_Double_Check_Flag = 1;
    }
    else
    {
        DP_Double_Check_Flag = 0;
    }


    for (y = 0; y < EndBlockY; y ++)
    {
        for (x = 0; x < EndBlockX; x ++)
        {
            // reset average value to zero
            RGGBAvg[0] = RGGBAvg[1] = RGGBAvg[2] = RGGBAvg[3] = 0;
            RGGBValue[0] = RGGBValue[1] = RGGBValue[2] = RGGBValue[3] = 0;

            // current block address;
            //#NT#2008/08/19#JJ Huang -begin
            //#NT#replace width by lineoffset
            pubBlockAddr = (UINT8 *)(SearchInfo.BayAdd + (SearchInfo.LineOffset * y * SearchInfo.Block_Y) + (SearchInfo.Block_X * x));
            //#NT#2008/08/19#JJ Huang -end

            ExtraY = 0;
            if (y == (EndBlockY - 1)) ExtraY = RemainY;

            ExtraX = 0;
            if (x == (EndBlockX - 1)) ExtraX = RemainX;

            // calculate average value;
            for (j = 0; j < (SearchInfo.Block_Y + ExtraY); j ++)
            {
                pubTempAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * SearchInfo.LineOffset)); // change line
                i = (SearchInfo.Block_X + ExtraX);
                while (i >= PATTERN_SIZE)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;

                    RGGBAvg[2] += *pubTempAddr;
                    RGGBValue[2] ++;
                    pubTempAddr ++;

                    RGGBAvg[3] += *pubTempAddr;
                    RGGBValue[3] ++;
                    pubTempAddr ++;

                    i -= PATTERN_SIZE;
                }

                if (i == 3)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;

                    RGGBAvg[2] += *pubTempAddr;
                    RGGBValue[2] ++;
                    pubTempAddr ++;
                }
                else if (i == 2)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;

                    RGGBAvg[1] += *pubTempAddr;
                    RGGBValue[1] ++;
                    pubTempAddr ++;
                }
                else if (i == 1)
                {
                    RGGBAvg[0] += *pubTempAddr;
                    RGGBValue[0] ++;
                    pubTempAddr ++;
                }
            }
            RGGBAvg[0] /= RGGBValue[0];
            RGGBAvg[1] /= RGGBValue[1];
            RGGBAvg[2] /= RGGBValue[2];
            RGGBAvg[3] /= RGGBValue[3];

            //get data and calculate diff
            for (j = 0; j < (SearchInfo.Block_Y + ExtraY); j ++)
            {
                pubTempAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * SearchInfo.LineOffset)); // change line

                for (i = 0; i < (SearchInfo.Block_X + ExtraX); i ++)
                {
                    hDiff = cal_abs((*pubTempAddr - RGGBAvg[(i & 0x3)]));

                    if (hDiff > SearchInfo.Threshold)
                    {
                        //record defect pixel 0xYYYYXXXX
                        uwTempValue = (((((SearchInfo.Block_Y * y) + j + SearchInfo.OffsetY) << 16) & 0xffff0000)
                                        | (((SearchInfo.Block_X * x) + i + SearchInfo.OffsetX) & 0x0000ffff));

                        CheckFlag = 0;
                        if (DP_Double_Check_Flag)
                        {
                            for (k = 0; k < SearchInfo.StartCnt; k ++)
                            {
                                if (((UINT32 *)SearchInfo.DPadd)[k] == uwTempValue)
                                {
                                    CheckFlag = 1;
                                    break;
                                }
                            }
                        }

                        if (CheckFlag == 0)
                        {
//#NT#2008/12/25#JJ Huang -begin
                            *(volatile UINT32*)puwDPCurAddr = uwTempValue;
//#NT#2008/12/25#JJ Huang -end
                            //increase index
                            puwDPCurAddr ++;
                            DP_Cnt ++;
                        }

                        if (DP_Cnt >= SearchInfo.MaxCnt)
                        {
                            //sprintf(message, "-E- defect pixel count overflow");
                            //debug_err((message));
                            return (DP_Cnt - SearchInfo.StartCnt);
                        }
                    }
                    //next address
                    pubTempAddr ++;
                }
            }
        }
    }

    return (DP_Cnt - SearchInfo.StartCnt);
}

//#NT#2008/08/19#JJ Huang -begin
//#NT#Add an API for panasonice CCD Preview DP Search
UINT32 DPC_9PxlMode_Search(DPC_SEARCH SearchInfo)
{
    #define PATTERN_X_SIZE  6
    #define PATTERN_Y_SIZE  2
    #define PATTERN_GROUP   12

    UINT32 PatternAvg[PATTERN_GROUP], PatternCnt[PATTERN_GROUP];
    UINT32 *puwDPCurAddr, uwTempValue;
    UINT8 *pubBlockAddr, *pubYAddr, *pubXAddr;
    UINT8 PatternIdx;
    INT16 hDiff;
    UINT16 RemainX,RemainY,EndBlockX,EndBlockY, ExtraX, ExtraY;
    UINT16 DP_Cnt, i , j, k, x, y;
    UINT8 DP_Double_Check_Flag, CheckFlag;

    {//Check and modify fatal arguments
        if(SearchInfo.Block_X%PATTERN_X_SIZE)
        {
            SearchInfo.Block_X=((SearchInfo.Block_X/PATTERN_X_SIZE)+1)*PATTERN_X_SIZE;
        }
        if(SearchInfo.Block_Y%PATTERN_Y_SIZE)
        {
            SearchInfo.Block_Y=((SearchInfo.Block_Y/PATTERN_Y_SIZE)+1)*PATTERN_Y_SIZE;
        }
    }
    {//compute blocking parameter
        EndBlockX = SearchInfo.Width / SearchInfo.Block_X;
        EndBlockY = SearchInfo.Height / SearchInfo.Block_Y;
        RemainX = SearchInfo.Width - (EndBlockX * SearchInfo.Block_X);
        RemainY = SearchInfo.Height - (EndBlockY * SearchInfo.Block_Y);
    }
    {//set defect pixel address and recheck
        DP_Cnt = SearchInfo.StartCnt;
        puwDPCurAddr = (UINT32 *)SearchInfo.DPadd + DP_Cnt;
        if (SearchInfo.StartCnt)
        {
            DP_Double_Check_Flag = 1;
        }
        else
        {
            DP_Double_Check_Flag = 0;
        }
    }
    {//debug message
        DPC_MSG("width %d   height %d LineOffset %d\r\n", SearchInfo.Width, SearchInfo.Height, SearchInfo.LineOffset);
        DPC_MSG("OffsetY %d   OffsetX %d\r\n", SearchInfo.OffsetY, SearchInfo.OffsetX);
    }

    for (y = 0; y < EndBlockY; y ++)
    {
        for (x = 0; x < EndBlockX; x ++)
        {
            {//Initial
                for (i=0; i<(PATTERN_X_SIZE*PATTERN_Y_SIZE); i ++)
                {
                    PatternAvg[i] = 0;
                    PatternCnt[i] = 0;
                }
                pubBlockAddr = (UINT8 *)(SearchInfo.BayAdd + (SearchInfo.LineOffset * y * SearchInfo.Block_Y) + (SearchInfo.Block_X * x));
                ExtraY = 0;
                if (y == (EndBlockY - 1))
                    ExtraY = RemainY;
                ExtraX = 0;
                if (x == (EndBlockX - 1))
                    ExtraX = RemainX;
            }

            for (j = 0; j < (SearchInfo.Block_Y + ExtraY); j ++)
            {// calculate average value;
                {//Initial
                    pubYAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * SearchInfo.LineOffset)); // change line
                    pubXAddr = pubYAddr;
                }
                for (i = 0; i < (SearchInfo.Block_X + ExtraX); i ++)
                {
                    PatternIdx = (j%PATTERN_Y_SIZE)*PATTERN_X_SIZE + (i%PATTERN_X_SIZE);
                    PatternAvg[PatternIdx] += *pubXAddr;
                    PatternCnt[PatternIdx] ++;
                    pubXAddr++;
                }
            }

            {//Compute average
                for (i=0; i<(PATTERN_X_SIZE*PATTERN_Y_SIZE); i ++)
                {
                    PatternAvg[i] /= PatternCnt[i] ;
                }
            }

            for (j = 0; j < (SearchInfo.Block_Y + ExtraY); j ++)
            {// compute diff. and threshold out DP
                {//Initial
                    pubYAddr = (UINT8 *)((UINT32)pubBlockAddr + (j * SearchInfo.LineOffset)); // change line
                    pubXAddr = pubYAddr;
                }
                for (i = 0; i < (SearchInfo.Block_X + ExtraX); i ++)
                {
                    {//compute difference
                        PatternIdx = (j%PATTERN_Y_SIZE)*PATTERN_X_SIZE + (i%PATTERN_X_SIZE);
                        hDiff = cal_abs((*pubXAddr - PatternAvg[PatternIdx]));
                    }

                    if (hDiff > SearchInfo.Threshold)
                    {//record defect pixel 0xYYYYXXXX
                        uwTempValue = (((((SearchInfo.Block_Y * y) + j + SearchInfo.OffsetY) << 16) & 0xffff0000)
                                        | (((SearchInfo.Block_X * x) + i + SearchInfo.OffsetX) & 0x0000ffff));

                        CheckFlag = 0;
                        #if 0
                        debug_err(("DP%4d 0x%08x=(%4d,%3d) Val%3d Avg%3d Diff%3d\r\n",
                            DP_Cnt,
                            ((((j+y*Block_Y+OffsetY)<<16)&0xffff0000)+((i+x*Block_X+OffsetX)&0xffff)),
                            (((j+y*Block_Y+OffsetY))&0xffff), ((i+x*Block_X+OffsetX)&0xffff),
                            *pubXAddr, PatternAvg[PatternIdx], hDiff));
                        #endif
                        if (DP_Double_Check_Flag)
                        {
                            for (k = 0; k < SearchInfo.StartCnt; k ++)
                            {
                                if (((UINT32 *)SearchInfo.DPadd)[k] == uwTempValue)
                                {
                                    CheckFlag = 1;
                                    break;
                                }
                            }
                        }

                        if (CheckFlag == 0)
                        {
//#NT#2008/12/25#JJ Huang -begin
                            *(volatile UINT32*)puwDPCurAddr = uwTempValue;
//#NT#2008/12/25#JJ Huang -end
                            //increase index
                            puwDPCurAddr ++;
                            DP_Cnt ++;
                        }

                        if (DP_Cnt >= SearchInfo.MaxCnt)
                        {
                            //sprintf(message, "-E- defect pixel count overflow");
                            //debug_err((message));
                            return (DP_Cnt - SearchInfo.StartCnt);
                        }
                    }
                    {//increase
                        pubXAddr++;
                    }
                }
            }


        }
    }

    return (DP_Cnt - SearchInfo.StartCnt);
}
//#NT#2008/08/19#JJ Huang -end

static void down_heap(unsigned long *Data, int parent_index, int last_heap_index)
{
    int p_index, c_index, last_parent_index;
    unsigned long temp;

    temp = Data[parent_index];
    p_index = parent_index;
    last_parent_index = (last_heap_index - 1) >> 1;

    while (p_index <= last_parent_index)
    {
        c_index = (p_index << 1) + 1;

        if (c_index < last_heap_index)
        {
            if (Data[c_index+1] > Data[c_index])
                c_index = c_index + 1;
        }

        if (Data[c_index] > temp)
        {
            Data[p_index] = Data[c_index];
            p_index = c_index;
        }
        else break;
    }

    if (p_index != parent_index)
        Data[p_index] = temp;
}

void DPC_SortDPdata(UINT32* Data,UINT32 Num)
{
    int last_heap_index, last_parent_index, temp, i;

    last_heap_index = Num - 1;
    last_parent_index = (last_heap_index - 1) >> 1;

    for (i = last_parent_index; i >= 0; i--)
        down_heap(Data, i,Num-1);

    temp = Data[0];
    Data[0] = Data[last_heap_index];
    Data[last_heap_index] = temp;
    last_heap_index = last_heap_index - 1;
    last_parent_index = (last_heap_index - 1) >> 1;

    for (i = Num-2; i > 0 ; i--)
    {
        down_heap(Data, 0,i);
        temp = Data[0];
        Data[0] = Data[i];
        Data[i] = temp;
    }
}

void DPC_SetSearchInfo(DPC_ALG_INFO param)
{
    STR_SIE_CROP CropParam = {0};

    sie_getCropWindow(&CropParam);

    if (param.Type == DPC_TYPE_BRIGHT_PREVIEW || param.Type == DPC_TYPE_DARK_PREVIEW)
    {
        //gSearchInfo.BayAdd = BayAddr + (Setting.SkipYU * GetIPPSizeInfor(_SieOutHsize) + Setting.SkipXL);
        gSearchInfo.BayAdd = param.BayAddr/*ippbuf_GetBuf(IPPBUF_PRV_SIERAW_1)*/ + (param.SkipYU * CropParam.Hsize/*GetIPPSizeInfor(_SieCropCapHsize)*/ + param.SkipXL);
        gSearchInfo.Width = CropParam.Hsize/*GetIPPSizeInfor(_SieCropCapHsize)*/ - param.SkipXL - param.SkipXR;
        gSearchInfo.Height = CropParam.Vsize - param.SkipYU - param.SkipYD;
        gSearchInfo.LineOffset = CropParam.Hsize/*GetIPPSizeInfor(_SieCropCapHsize)*/;
        gSearchInfo.CapFields = 1; //GetIPPSizeInfor(_CapTotalField);

        gSearchInfo.DPadd = param.DPAddr;
        gSearchInfo.OffsetX = CropParam.Hstart + param.SkipXL;
        gSearchInfo.OffsetY = CropParam.Vstart +param.SkipYU;
        gSearchInfo.StartCnt = param.Count;
        gSearchInfo.MaxCnt = param.MaxCount;
        gSearchInfo.Threshold = param.Threshold;
        gSearchInfo.Block_X = param.Block_X;
        gSearchInfo.Block_Y = param.Block_Y;
    }
    else if (param.Type == DPC_TYPE_BRIGHT_STILL || param.Type == DPC_TYPE_DARK_STILL)
    {
        gSearchInfo.BayAdd = param.BayAddr + (param.SkipYU * CropParam.Hsize/*GetIPPSizeInfor(_SieCropCapHsize)*/ + param.SkipXL);
        gSearchInfo.Width = CropParam.Hsize/*GetIPPSizeInfor(_SieCropCapHsize)*/ - param.SkipXL - param.SkipXR;
        gSearchInfo.Height = CropParam.Vsize/*GetIPPSizeInfor(_SieCropCapVsize)*/ - param.SkipYU - param.SkipYD;
        gSearchInfo.LineOffset = CropParam.Hsize;//GetIPPSizeInfor(_SieCropCapHsize);
        gSearchInfo.CapFields = 1; // GetIPPSizeInfor(_CapTotalField);

        gSearchInfo.DPadd = param.DPAddr;
        gSearchInfo.OffsetX = CropParam.Hstart + param.SkipXL;
        gSearchInfo.OffsetY = CropParam.Vstart * gSearchInfo.CapFields + param.SkipYU;
        gSearchInfo.StartCnt = param.Count;
        gSearchInfo.MaxCnt = param.MaxCount;
        gSearchInfo.Threshold = param.Threshold;
        gSearchInfo.Block_X = param.Block_X;
        gSearchInfo.Block_Y = param.Block_Y;
    }
}

UINT32 DPC_DataSearch(UINT32 Method)
{
    UINT32 rval;

    switch (Method)
    {
        default:
        case DPC_SEARCH_METHOD_BAYER:
            rval = DPC_Bayer_Search(gSearchInfo);
            break;

        case DPC_SEARCH_METHOD_VGA:
            rval = DPC_VGA_Search(gSearchInfo);
            break;

        case DPC_SEARCH_METHOD_9PXL:
            rval = DPC_9PxlMode_Search(gSearchInfo);
            break;
    }

    return rval;
}
// EOF
