
#include    "ImageAlgInfor.h"
#include    "Type.h"
#include    "AfTracking.h"
#include    "Debug.h"
#include    "sie_lib.h"

//#define TrackingWin_SizeX 48
//#define TrackingWin_SizeY 24//48
//#define T_BlockPixel 8
#define TrackingAreaRatio 4
unsigned short TrackingWin_SizeX=48,TrackingWin_SizeY=24,X_BlockPixel=8,Y_BlockPixel=4;
unsigned long Weighting_Y=0,Weighting_C=4;
#define MinDiffWinNum 6
BOOL FocusTrack_Enable = FALSE;
BOOL f_Tracking = FALSE;
unsigned short LastCenterX=160,LastCenterY=120;
unsigned short StableCenterX,StableCenterY;
unsigned short SubjectWinAvg[3]; // Histogram/Cb/Cr
//unsigned short SubjectInfo[3][TrackingWin_SizeY/T_BlockPixel][TrackingWin_SizeX/T_BlockPixel];
unsigned short SubjectInfo[3][8][8];
unsigned short BlockInfo[3][32][32]; // catch a block of 96x96 pixels, 96/8=12
unsigned long Tracking_StableCnt=0;
unsigned char LastColor=0;
unsigned short LastStartX,LastStartY;
unsigned short Motion_Avg,MO_Hist[5],MO_Cnt;

unsigned short Th_Local2AllView=650, Th_AllView2Local=650; // The threshold for refind object
unsigned short THres_StableDistX=16,THres_StableDistY=8; // The threshold for stable window


void setObjectWindowSize(UINT16 SizeX, UINT16 SizeY, UINT16 SubWindow_SizeX, UINT16 SubWindow_SizeY)
{
    TrackingWin_SizeX = SizeX;
    TrackingWin_SizeY = SizeY;
    X_BlockPixel = SubWindow_SizeX;
    Y_BlockPixel = SubWindow_SizeY;
}

void setAftrackingThershold(unsigned short StableDistanceX, unsigned short StableDistanceY,unsigned short Th_OutOfScreen,unsigned short Th_BackToScreen)
{
    THres_StableDistX = StableDistanceX;
    THres_StableDistY = StableDistanceY;
    Th_Local2AllView = Th_OutOfScreen;
    Th_AllView2Local = Th_BackToScreen;
}

void setY_C_Weighting(UINT16 W_Y,UINT16 W_C)
{
    Weighting_Y = W_Y;
    Weighting_C = W_C;
}
UINT16 GetTrackingWindow_CenterX(void)
{
    return LastCenterX;
}

UINT16 GetTrackingWindow_CenterY(void)
{
    return LastCenterY;
}

BOOL Is_Tracking(void)
{
    return f_Tracking;
}

BOOL IsTrackingEn(void)
{
    return FocusTrack_Enable;
}

static void SetTrackingFocusEnable(BOOL TrackingEn)
{
    FocusTrack_Enable = TrackingEn;
}

unsigned long GetTrackingStableCnt(void)
{
    return Tracking_StableCnt;
}

unsigned short GetMotionAvg(void)
{

    return Motion_Avg;
}

unsigned char GetBlockAvg(unsigned long Addr, unsigned short StartX, unsigned short StartY, unsigned short SizeX, unsigned short SizeY, unsigned short Offset)
{
    unsigned short i,j; //,k;
    unsigned long BlockSum,DataCnt,TempData;
    unsigned char BlockAvg;

    BlockSum = 0;
    DataCnt = 0;
/*
    for(j=StartY;j<(StartY+SizeY);j++)
    {
        for(i=StartX;i<(StartX+SizeX);i+=4)
        {
            TempData =  *(unsigned long*)(Addr+(j*Offset)+i);
            BlockSum += TempData&0xff;
            BlockSum += (TempData>>8)&0xff;
            BlockSum += (TempData>>16)&0xff;
            BlockSum += (TempData>>24)&0xff;

            DataCnt+=4;
        }
    }
*/
    for(j=StartY;j<(StartY+SizeY);j+=2)
    {
        for(i=StartX;i<(StartX+SizeX);i+=2)
        {
            TempData =  *(unsigned char*)(Addr+(j*Offset)+i);
            BlockSum += TempData&0xff;
            DataCnt++;
        }
    }
    BlockAvg = BlockSum/DataCnt;
    return BlockAvg;
}

unsigned char GetBlockAvg_Y(unsigned long Addr, unsigned short StartX, unsigned short StartY, unsigned short SizeX, unsigned short SizeY, unsigned short Offset)
{
    unsigned short i,j; //,k;
    unsigned long BlockSum,DataCnt,TempData;
    unsigned char BlockAvg;

    BlockSum = 0;
    DataCnt = 0;

    for(j=StartY;j<(StartY+SizeY);j+=2)
    {
        for(i=StartX;i<(StartX+SizeX);i+=4)
        {
            TempData =  *(unsigned short*)(Addr+(j*Offset)+i);
            BlockSum += TempData&0xff;
            BlockSum += (TempData>>8)&0xff;
            DataCnt+=2;
        }
    }
    BlockAvg = BlockSum/DataCnt;
    return BlockAvg;
}


void GetSubjectInfo(void)
{
    unsigned long Y_Sum,Cb_Sum,Cr_Sum,Counter;
    unsigned short i,j,StartX,StartY,SizeX,SizeY;
    ACTFBADDR_PRAM Act_IMEAdd;
    unsigned long Add_Y,Add_Cb,Add_Cr; //,TempData;

    SizeX = TrackingWin_SizeX;
    SizeY = TrackingWin_SizeY;
    LastCenterX = GetIPPSizeInfor(_ImeOut1Hsize)/2;
    LastCenterY = GetIPPSizeInfor(_ImeOut1Vsize)/2;
    StartX = LastCenterX - SizeX/2;
    StartY = LastCenterY -SizeY/2;

    GetActImgFBAddr_Path1(&Act_IMEAdd);
    Add_Y = Act_IMEAdd.y_addr;
    Add_Cb = Act_IMEAdd.cb_addr;
    Add_Cr = Act_IMEAdd.cr_addr;
    Y_Sum = 0;
    Cb_Sum = 0;
    Cr_Sum = 0;
    Counter = 0;
/*
    for(j=StartY;j<(StartY+SizeY);j++)
    {
        for(i=StartX/2;i<(StartX+SizeX)/2;i+=4)
        {
            TempData =  *(unsigned long*)(Add_Cb+(j*GetPreImeOutHsize()/2)+i);
            Cb_Sum += TempData&0xff;
            Cb_Sum += (TempData>>8)&0xff;
            Cb_Sum += (TempData>>16)&0xff;
            Cb_Sum += (TempData>>24)&0xff;

            TempData =  *(unsigned long*)(Add_Cr+(j*GetPreImeOutHsize()/2)+i);
            Cr_Sum += TempData&0xff;
            Cr_Sum += (TempData>>8)&0xff;
            Cr_Sum += (TempData>>16)&0xff;
            Cr_Sum += (TempData>>24)&0xff;

            Counter+=4;
        }
    }
*/
    for(j=0;j<(TrackingWin_SizeY/Y_BlockPixel);j++)
    {
        for(i=0;i<(TrackingWin_SizeX/X_BlockPixel);i++)
        {
            SubjectInfo[0][j][i] = GetBlockAvg_Y(Add_Y,StartX+i*X_BlockPixel,StartY+j*Y_BlockPixel,X_BlockPixel,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize));
            SubjectInfo[1][j][i] = GetBlockAvg(Add_Cb,(StartX+i*X_BlockPixel)/2,StartY+j*Y_BlockPixel,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
            SubjectInfo[2][j][i] = GetBlockAvg(Add_Cr,(StartX+i*X_BlockPixel)/2,StartY+j*Y_BlockPixel,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
            Y_Sum += SubjectInfo[0][j][i];
            Cb_Sum += SubjectInfo[1][j][i];
            Cr_Sum += SubjectInfo[2][j][i];
            Counter++;
            //debug_err(("SubjectInfo[%d][%d]=(%d,%d,%d)\r\n",j,i,SubjectInfo[0][j][i],SubjectInfo[1][j][i],SubjectInfo[2][j][i]));
        }
    }

    SubjectWinAvg[0] = Y_Sum/Counter;
    SubjectWinAvg[1] = Cb_Sum/Counter;
    SubjectWinAvg[2] = Cr_Sum/Counter;
    //debug_err(("TrackingWin_SizeX=%d,TrackingWin_SizeY=%d\r\n",TrackingWin_SizeX,TrackingWin_SizeY));
    //debug_err(("X_BlockPixel=%d,Y_BlockPixel=%d\r\n",X_BlockPixel,Y_BlockPixel));
}

void Start_Subject_Tracking(void)
{
// sample parameter
    setObjectWindowSize(48,24,8,4);
    setY_C_Weighting(1,3);
    setAftrackingThershold(8,4,700,650);
//    
    GetSubjectInfo();
    SetTrackingFocusEnable(TRUE);
}
void Stop_Subject_Tracking(void)
{
    SetTrackingFocusEnable(FALSE);
    if(Is_Tracking())
        sie_waitVD(1);
    LastCenterX=GetIPPSizeInfor(_ImeOut1Hsize)/2;
    LastCenterY=GetIPPSizeInfor(_ImeOut1Hsize)/2;
    //Draw_FocusIcon(50,50,0);
}

//#NT#2009/01/20#Connie Yu -begin
//#Add auto detection of the object re-entering into view
void Subject_Tracking(void)
{
    unsigned short NewCenterX,NewCenterY,SubjectWin;
    unsigned long Diff_Y,Diff_C,MinDiff[MinDiffWinNum][3],MinDiff_Sum,Diff_Sum;

    unsigned long Y_Avg,Cb_Avg,Cr_Avg;
    unsigned short i,j,k,Block_i,Block_j,StartX,StartY,SizeX,SizeY,StartWinX,StartWinY;
    unsigned short WinSizeX,WinSizeY,BlockNumX,BlockNumY,MotionOffset;
    ACTFBADDR_PRAM Act_IMEAdd;
    unsigned long Add_Y,Add_Cb,Add_Cr,CountX,CountY;

    static UINT8 Flg_SearchAllView=FALSE;
    UINT32 subRate;

    if(FocusTrack_Enable == FALSE)
    {
        Tracking_StableCnt = 0;
        return;
    }
    //TimerCountUp(1);
    f_Tracking = TRUE;
    GetActImgFBAddr_Path1(&Act_IMEAdd);
    Add_Y = Act_IMEAdd.y_addr;
    Add_Cb = Act_IMEAdd.cb_addr;
    Add_Cr = Act_IMEAdd.cr_addr;


    if(Flg_SearchAllView) //Object missing -> Search all view area
    {

        BlockNumX = TrackingWin_SizeX/X_BlockPixel+1;
        BlockNumY = TrackingWin_SizeY/Y_BlockPixel+1;

        subRate=2;
        StartX=0;
        StartY=0;
        for(StartWinY=0; StartWinY<(GetIPPSizeInfor(_ImeOut1Vsize) - TrackingWin_SizeY); StartWinY+=(Y_BlockPixel*subRate))
        {
            for(StartWinX=0; StartWinX<(GetIPPSizeInfor(_ImeOut1Hsize) - TrackingWin_SizeX); StartWinX+=(X_BlockPixel*subRate))
            {
/*
                Y_Avg = 0;
                Cb_Avg = 0;
                Cr_Avg = 0;
                for(j=0;j<(TrackingWin_SizeY/Y_BlockPixel);j++)
                {
                    StartY = StartWinY+j*Y_BlockPixel;
                    for(i=0;i<(TrackingWin_SizeX/X_BlockPixel);i++)
                    {
                        StartX = StartWinX+i*X_BlockPixel;
                        //Get reference block avgs
                        BlockInfo[0][j][i] = GetBlockAvg_Y(Add_Y,StartX,StartY,X_BlockPixel,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize));
                        BlockInfo[1][j][i] = GetBlockAvg(Add_Cb,StartX/2,StartY,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
                        BlockInfo[2][j][i] = GetBlockAvg(Add_Cr,StartX/2,StartY,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
                        //Calculate diff(ref - obj)
                        Y_Avg += abs(BlockInfo[0][j][i]-SubjectInfo[0][j][i]);
                        Cb_Avg += abs(BlockInfo[1][j][i]-SubjectInfo[1][j][i]);
                        Cr_Avg += abs(BlockInfo[2][j][i]-SubjectInfo[2][j][i]);
                    }
                }
*/
                Y_Avg = GetBlockAvg_Y(Add_Y,StartWinX,StartWinY,TrackingWin_SizeX,TrackingWin_SizeY,GetIPPSizeInfor(_ImeOut1Hsize));
                Cb_Avg = GetBlockAvg(Add_Cb,StartWinX/2,StartWinY,TrackingWin_SizeX/2,TrackingWin_SizeY,GetIPPSizeInfor(_ImeOut1Hsize)/2);
                Cr_Avg = GetBlockAvg(Add_Cr,StartWinX/2,StartWinY,TrackingWin_SizeX/2,TrackingWin_SizeY,GetIPPSizeInfor(_ImeOut1Hsize)/2);

                Diff_C = abs(Cb_Avg-SubjectWinAvg[1])+abs(Cr_Avg-SubjectWinAvg[2]);
                Diff_Y = abs(Y_Avg-SubjectWinAvg[0]);
                Diff_Sum = ((Diff_Y*Weighting_Y)+(Diff_C*Weighting_C))*160/(Weighting_Y+Weighting_C+1);
                if(Diff_Sum < Th_AllView2Local)
                {
                    Flg_SearchAllView=FALSE;

                    MinDiff[0][1] = Diff_Sum;
                    MinDiff[0][0] = StartWinX;
                    MinDiff[0][2]= StartWinY;
                }
            }
        }
        //Target found
        if(Flg_SearchAllView==FALSE)
        {
            LastStartX = MinDiff[0][0]; //X
            LastStartY = MinDiff[0][2]; //Y
            LastCenterX = LastStartX+TrackingWin_SizeX/2;
            LastCenterY = LastStartY+TrackingWin_SizeY/2;
            debug_err(("Target found: %d %d md %d\r\n",LastStartX,LastStartY,MinDiff[0][1]));
        }


    }
    else
    {
        SizeX = TrackingWin_SizeX;
        SizeY = TrackingWin_SizeY;
        WinSizeX = SizeX*TrackingAreaRatio;
        WinSizeY = SizeY*TrackingAreaRatio;
        
        CountX = (TrackingWin_SizeX/X_BlockPixel);
        CountY = (TrackingWin_SizeY/Y_BlockPixel);

        //debug_err(("Size(%d,%d)\r\n",CountX,CountY));
        
        if(LastCenterX <= WinSizeX/2)
            StartWinX = 0;
        else if((LastCenterX+WinSizeX/2)>GetIPPSizeInfor(_ImeOut1Hsize))
            StartWinX = GetIPPSizeInfor(_ImeOut1Hsize) - WinSizeX;
        else
            StartWinX = LastCenterX -WinSizeX/2;

        if(LastCenterY <= WinSizeY/2)
            StartWinY = 0;
        else if((LastCenterY+WinSizeY/2)>GetIPPSizeInfor(_ImeOut1Vsize))
            StartWinY = GetIPPSizeInfor(_ImeOut1Vsize) - WinSizeY;
        else
            StartWinY = LastCenterY -WinSizeY/2;

//        for(Block_j=0;Block_j<(TrackingWin_SizeY*2/Y_BlockPixel);Block_j++)
        for(Block_j=0;Block_j<(CountY*TrackingAreaRatio);Block_j++)
        {
            StartY = StartWinY+Block_j*Y_BlockPixel;
//            for(Block_i=0;Block_i<(TrackingWin_SizeX*2/X_BlockPixel);Block_i++)
            for(Block_i=0;Block_i<(CountX*TrackingAreaRatio);Block_i++)
            {
                StartX = StartWinX+Block_i*X_BlockPixel;
                BlockInfo[0][Block_j][Block_i] = GetBlockAvg_Y(Add_Y,StartX,StartY,X_BlockPixel,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize));
                BlockInfo[1][Block_j][Block_i] = GetBlockAvg(Add_Cb,StartX/2,StartY,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
                BlockInfo[2][Block_j][Block_i] = GetBlockAvg(Add_Cr,StartX/2,StartY,X_BlockPixel/2,Y_BlockPixel,GetIPPSizeInfor(_ImeOut1Hsize)/2);
                //debug_err(("BlockInfo[%d][%d]=(%d,%d,%d)\r\n",Block_j,Block_i,BlockInfo[0][Block_j][Block_i],BlockInfo[1][Block_j][Block_i],BlockInfo[2][Block_j][Block_i]));
            }
        }

        BlockNumX = (WinSizeX-TrackingWin_SizeX)/X_BlockPixel+1;
        BlockNumY = (WinSizeY-TrackingWin_SizeY)/Y_BlockPixel+1;

        for(k=0;k<MinDiffWinNum;k++)
        {
            MinDiff[k][0] = 0xffffffff;
            MinDiff[k][1] = 0xffffffff;
        }

        for(Block_j=0;Block_j<BlockNumY;Block_j++)
        {
            for(Block_i=0;Block_i<BlockNumX;Block_i++)
            {
                Y_Avg = 0;
                Cb_Avg = 0;
                Cr_Avg = 0;
                //for(j=0;j<(TrackingWin_SizeY/Y_BlockPixel);j++)
                for(j=0;j<CountY;j++)
                {
 //                   for(i=0;i<(TrackingWin_SizeX/X_BlockPixel);i++)
                    for(i=0;i<CountX;i++)
                    {
                        Y_Avg += abs(BlockInfo[0][j+Block_j][i+Block_i]-SubjectInfo[0][j][i]);
                        Cb_Avg += abs(BlockInfo[1][j+Block_j][i+Block_i]-SubjectInfo[1][j][i]);
                        Cr_Avg += abs(BlockInfo[2][j+Block_j][i+Block_i]-SubjectInfo[2][j][i]);
                    }
                }
                Diff_C = Cb_Avg+Cr_Avg;
                Diff_Y = Y_Avg;
                //debug_err(("[%d][%d]=(%d,%d,%d)\r\n",Block_j,Block_i,Y_Avg,Cb_Avg,Cr_Avg));

                Diff_Sum = ((Diff_Y*Weighting_Y)+(Diff_C*Weighting_C))/(Weighting_Y+Weighting_C+1);// +1 to avoid devided by zero

                if(Diff_Sum < MinDiff[MinDiffWinNum-1][1])
                {
                    for(k=(MinDiffWinNum-1);k>=1;k--)
                    {
                        if(Diff_Sum < MinDiff[k-1][1])
                        {
                            //MinDiff[k][0] = MinDiff[k-1][0];
                            //MinDiff[k-1][0] = Diff_Y;
                            MinDiff[k][1] = MinDiff[k-1][1];
                            MinDiff[k-1][1] = Diff_Sum;
                            MinDiff[k][2] = MinDiff[k-1][2];
                            MinDiff[k-1][2] = Block_j*BlockNumX+Block_i;
                        }
                        else
                            break;
                    }
                }
#if 0
                if(Diff_Y < MinDiff[MinDiffWinNum-1][0])
                {
                    for(k=(MinDiffWinNum-1);k>=1;k--)
                    {
                        if(Diff_Y < MinDiff[k-1][0])
                        {
                            MinDiff[k][0] = MinDiff[k-1][0];
                            MinDiff[k-1][0] = Diff_Y;
                            MinDiff[k][1] = MinDiff[k-1][1];
                            MinDiff[k-1][1] = Diff_C;
                            MinDiff[k][2] = MinDiff[k-1][2];
                            MinDiff[k-1][2] = Block_j*BlockNumX+Block_i;
                        }
                        else
                            break;
                    }
                }
#endif


            }
        }

        SubjectWin = MinDiff[0][2];

        MinDiff_Sum = 0xffff;
        for(k=0;k<MinDiffWinNum;k++)
        {
            //debug_err(("Min[%d]=%d\r\n",k,MinDiff[k][1]));
            if(MinDiff[k][2]==(BlockNumY*BlockNumX/2))
            {
                if((MinDiff[k][1]-MinDiff[0][1]<=15))// && (MinDiff[k][0]<=20))
                    SubjectWin = (BlockNumY*BlockNumX/2);
                break;
            }
#if 0
            if(MinDiff[0][1]>40)
            {
                Diff_Sum =  MinDiff[k][0]+MinDiff[k][1];
                if(Diff_Sum<MinDiff_Sum)
                    SubjectWin = MinDiff[k][2];
            }
#endif
        }

        StartX = StartWinX+(SubjectWin%BlockNumX)*X_BlockPixel;
        StartY = StartWinY+(SubjectWin/BlockNumX)*Y_BlockPixel;
    /*
        for(j=0;j<6;j++)
            for(i=0;i<6;i++)
                SubjectInfo[0][j][i] = BlockInfo[0][j+(SubjectWin/BlockNumX)][i+(SubjectWin%BlockNumX)];
    */
        NewCenterX = StartX+TrackingWin_SizeX/2;
        NewCenterY = StartY+TrackingWin_SizeY/2;

        MotionOffset = abs(StableCenterX-NewCenterX)+abs(StableCenterY-NewCenterY);
        //if(MotionOffset<=T_BlockPixel*2)//((abs(LastCenterX-NewCenterX)<=T_BlockPixel)&&(abs(LastCenterY-NewCenterY)<=T_BlockPixel))
        if((abs(LastCenterX-NewCenterX)<=THres_StableDistX)&&(abs(LastCenterY-NewCenterY)<=THres_StableDistY))
        {
            Tracking_StableCnt ++;
            NewCenterX = LastCenterX;
            NewCenterY = LastCenterY;
        }
        else
        {
            Tracking_StableCnt = 0;
            LastColor = 0;
            StableCenterX = NewCenterX;
            StableCenterY = NewCenterY;
        }

        MO_Hist[MO_Cnt] = MotionOffset;
        if(MO_Cnt>=4)
            MO_Cnt =0;
        else
            MO_Cnt++;

        Motion_Avg = 0;
        for(i=0;i<5;i++)
        {
            Motion_Avg+=MO_Hist[i];
        }
        Motion_Avg/=5;

//#NT#2009/04/29#Harry -begin
//#NT#To smooth the ICON  movement
        //LastCenterX = NewCenterX;
        //LastCenterY = NewCenterY;
        //LastStartX = StartX;
        //LastStartY = StartY;
        LastCenterX = (NewCenterX+LastCenterX)/2;
        LastCenterY = (NewCenterY+LastCenterY)/2;
        LastStartX = (StartX+LastStartX)/2;
        LastStartY = (StartY+LastStartY)/2;
//#NT#2009/04/29#Harry -end

        //Object missing -> Search all view area
        if(MinDiff[0][1] > Th_Local2AllView)
        {
            Flg_SearchAllView=TRUE;
            debug_err(("Search All View ...........\r\n"));
        }

    }
    f_Tracking = FALSE;

    //RTAF_WindowSetting(LastStartX,LastStartY);
/***************Draw OSD********************/
#if 0
    PhotoFlow_ShowAllIcon1();
    DrawFocusStatusIcon(LastColor);

    if(Motion_Avg>1)
        MenuOff_ShowMode(13);
    else
        MenuOff_ShowMode(0);
#endif

/********************************************/
    if(Tracking_StableCnt<3)
    {
        //debug_err(("%d,%d, %d\r\n",GetTrackingWindow_CenterX(),GetTrackingWindow_CenterY(),MinDiff[0][1]));
        //debug_err(("%d\r\n",MinDiff[0][1]));
    }

    //sprintf(message,"%d",TimerCountUp(0));
    //sprintf(message,"%d",Tracking_StableCnt);
    //OSD_ShowDate(message,20,200);
}
//#NT#2009/01/20#Connie Yu -end

#if 0
BOOL RT_AF_Run=FALSE,RT_AF_Direction;
unsigned long LastAF_Value;
unsigned char RT_AF_Status=0,RT_AF_Step;

unsigned short TargetIdx;
unsigned short RTAF_BestIdx;
unsigned short Recode_FocusStep[100];
unsigned char Index_Cnt=0;

BOOL FocusRun=FALSE;
BOOL Inv_Direction=FALSE;
BOOL Resum_TrackingFocus;
unsigned short RTAF_MinIndex,RTAF_MaxIndex;

void RTAF_SearchRange(unsigned short MinIndex, unsigned short MaxIndex)
{
    RTAF_MinIndex = MinIndex;
    RTAF_MaxIndex = MaxIndex;
}

void aaa_SetVA_RTAF(UB source,UH StartH,UH StartV, UH hsize, UH vsize)
{
    UB hnf=0,vnf=0;
    UH stx,sty,window_h,window_v;
    unsigned long Norm_ratio;

    //ipe_EnableVacc();
    window_h=hsize;
    window_v=vsize;

    while(window_h>1)
    {
        window_h=(window_h>>1);
        hnf+=1;
    }
    while(window_v>1)
    {
        window_v=(window_v>>1);
        vnf+=1;
    }
    window_h=hsize;
    window_v=vsize;
    stx=StartH;
    sty=StartV;
    //ipe_SetVarDet(0,4,2);
    ipe_SetVarDet(0,4,1);
    ipe_SetVarAccMode(source,0,0,stx,sty,0);
    ipe_SetVarAccMode_432(source,0,0,stx,sty,0,0);
    ipe_SetVarAccWin(hnf,vnf,window_h,window_v);
    Norm_ratio = (1<<(hnf+8))/window_h;
    SetAfScale(Norm_ratio);
}


void SetRTAF(BOOL enable)
{
    RT_AF_Run=enable;
}

BOOL Is_RTAFRunning(void)
{

    return RT_AF_Run;
}

//extern unsigned short LastStartX,LastStartY;
unsigned char RTAF_Window;
void RTAF_WindowSetting(unsigned short IMEStartX, unsigned short IMEStartY)
{
    UINT16 StartX,StartY,SizeX,SizeY;

    SizeX = (48*GetIpeOutHsize())/GetPreImeOutHsize();
    if(SizeX%2)
        SizeX++;
    SizeY = (48*GetIpeOutVsize())/GetPreImeOutVsize();
    if(SizeY%2)
        SizeY++;
    StartX = (IMEStartX*GetIpeOutHsize())/GetPreImeOutHsize();
    if(StartX%2)
        StartX++;

    StartY = (IMEStartY*GetIpeOutVsize())/GetPreImeOutVsize();
    if(StartY%2)
        StartY++;

    RTAF_Window = 0;
    if((StartX+(SizeX*3))>GetIpeOutHsize())
    {
        StartX -= (SizeX*2);
        RTAF_Window += 2;
    }
    if((StartY+(SizeY*3))>GetIpeOutVsize())
    {
        StartY -= (SizeY*2);
        RTAF_Window += 6;
    }
    aaa_SetVA_RTAF(0,StartX,StartY,SizeX,SizeY);
}

void RT_AF2(void)
{
    UINT8 MinIdx,MaxIdx,CenterIdx;
    UINT16 NextIdx,StartX,StartY,SizeX,SizeY;
    UINT32 AF_Value;

    if(!Is_RTAFRunning())
    {
        RT_AF_Status=0;
        Index_Cnt = 0;
        return;
    }
    //debug_err(("RT_AF_Status=%d\r\n",RT_AF_Status));
    switch(RT_AF_Status)
    {
        case 0://first set status and get current contrast
            FocusPowerOnOff(ON);
            Resum_TrackingFocus=IsTrackingEn();
            RT_AF_Step=2;
            if(!Inv_Direction)
            {
                if(Get_FocusPosition()<((RTAF_MinIndex+RTAF_MaxIndex)/2))
                    RT_AF_Direction=1;
                else
                    RT_AF_Direction=0;
            }
            else
            {
                Inv_Direction=FALSE;
            }
            if(RT_AF_Direction==0)
                NextIdx=Get_FocusPosition()-RT_AF_Step;
            else
                NextIdx=Get_FocusPosition()+RT_AF_Step;
            Focus_Go2_AF(NextIdx);
            RT_AF_Status++;
            //debug_err(("RT_AF_Direction=%d\r\n",RT_AF_Direction));
            break;
        case 1:
            if(RT_AF_Direction==0)
                NextIdx=Get_FocusPosition()-RT_AF_Step;
            else
                NextIdx=Get_FocusPosition()+RT_AF_Step;

            Focus_Go2_AF(NextIdx);
            Index_Cnt ++;
            RT_AF_Status++;
            //debug_err(("RT_AF_Direction=%d\r\n",RT_AF_Direction));
            break;
        case 2:
            AF_Value=aaa_GetVariation(RTAF_Window);
            LastAF_Value=AF_Value;
            //debug_err(("AF_Value=%d\r\n",AF_Value));

            if(RT_AF_Direction==0)
                NextIdx=Get_FocusPosition()-RT_AF_Step;
            else
                NextIdx=Get_FocusPosition()+RT_AF_Step;
            Focus_Go2_AF(NextIdx);
            RT_AF_Status++;
            Index_Cnt ++;
            break;
        case 3:
            AF_Value=aaa_GetVariation(RTAF_Window);
            //debug_err(("AF_Value=%d\r\n",AF_Value));
            if(AF_Value<LastAF_Value)
            {
                RT_AF_Direction=!RT_AF_Direction;
                Inv_Direction = TRUE;
                RT_AF_Status=0;
                Index_Cnt=0;
                break;
            }
            LastAF_Value=AF_Value;

            if(RT_AF_Direction==0)
                NextIdx=Get_FocusPosition()-RT_AF_Step;
            else
                NextIdx=Get_FocusPosition()+RT_AF_Step;

            Focus_Go2_AF(NextIdx);
            RT_AF_Status++;
            Index_Cnt ++;
            break;
        case 4:
            AF_Value=aaa_GetVariation(RTAF_Window);
            //debug_err(("AF_Value=%d\r\n",AF_Value));

            if(AF_Value<LastAF_Value)
            {
                RTAF_BestIdx=Index_Cnt;
                RT_AF_Status++;
            }
            else if((Get_FocusPosition()>=RTAF_MaxIndex)||(Get_FocusPosition()<=RTAF_MinIndex))
            {
                RT_AF_Status++;
                RTAF_BestIdx=Index_Cnt;
            }
            else
            {
                if(RT_AF_Direction==0)
                    NextIdx=Get_FocusPosition()-RT_AF_Step;
                else
                    NextIdx=Get_FocusPosition()+RT_AF_Step;

                if(NextIdx>=RTAF_MaxIndex)
                    NextIdx=RTAF_MaxIndex;
                else if(NextIdx<=RTAF_MinIndex)
                    NextIdx=RTAF_MinIndex;

                RTAF_BestIdx=Index_Cnt;
                LastAF_Value=AF_Value;
                Focus_Go2_AF(NextIdx);
                Index_Cnt ++;
            }
            break;
        case 5:
            SetRTAF(FALSE);
            RT_AF_Status=0;
            Focus_Go2_AF(Recode_FocusStep[RTAF_BestIdx-1]);
            FocusPowerOnOff(OFF);

            //debug_err(("Best idx=%d\r\n",Get_FocusPosition()));
            Index_Cnt = 0;
            if(0)//(Resum_TrackingFocus)
            {
                SetTrackingFocusEnable(TRUE);
                Resum_TrackingFocus = FALSE;
            }
            break;
    }
    Recode_FocusStep[Index_Cnt] = Get_FocusPosition();
}
#endif

//#NT#2009/01/20#Connie Yu -begin
INT32 abs(INT32 i)
{
    if (i >= 0)
    {
        return i;
    }
    return (-i);
}
//#NT#2009/01/20#Connie Yu -end

