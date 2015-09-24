#ifndef MULTIVIEW_H
#define MULTIVIEW_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "OSCommon.h"

typedef struct _MULTI_VIEW_RECT{
    INT32 l; //!< Left
    INT32 t; //!< Top
    INT32 w; //!< Width
    INT32 h; //!< Height
}tMULTI_VIEW_RECT;

typedef struct _MULTI_VIEW_BUF_DESC{
    UINT32 AddrY;
    UINT32 AddrU;
    UINT32 AddrV;
    UINT32 Width;
    UINT32 Height;    
    UINT32 PitchY;
    UINT32 PitchUV;
}tMULTI_VIEW_BUF_DESC;

typedef struct _MULTI_VIEW_INIT
{
    tMULTI_VIEW_BUF_DESC DescSrc[3][4]; //!< Source Buffer (ArcFilter Buffer)
    tMULTI_VIEW_BUF_DESC DescDst[2];    //!< Destination Buffer (IDE Buffer,Use Double)
    UINT32 WorkingBufAddr;              //!< Working Buffer (Require Source Image 3*(LineOffest_Y * Height)
    UINT32 WorkingBufSize;              //!< Working Buffer Size
    tMULTI_VIEW_RECT RectBegin;         //!< Begin Box Rectangle
    UINT32 (*SrcLock)(void);            //!< Source Buffer Lock
    void   (*SrcUnLock)(UINT32 Index);  //!< Source Buffer UnLock
    UINT32 (*DstLock)(void);            //!< Destination Buffer Lock
    void   (*DstUnLock)(UINT32 Index);  //!< Destination Buffer UnLock
    INT32 MiddleBoxWidth;      //!< Middle Box Height
    INT32 MiddleBoxHeight;     //!< Middle Box Width
    INT32 BoundaryBoxWidth;    //!< Boundary Box Width
    INT32 BoundaryBoxHeight;   //!< Boundary Box Height
    INT32 MirrorHeight;        //!< Mirror Height
    INT32 CenterPosX;          //!< Position X of Middle Box Center
    INT32 CenterPosY;          //!< Position Y of Middle Box Center    
    INT32 MoveDistance;        //!< Pixels of Moving from Middle Box Center Position to Boundary Box Center Position
    INT32 MoveStepPixel;       //!< Moving Pixels of Each Step (MoveDistance%MoveStepPixel must be 0)
    INT32 CandidateTotal;      //!< Numbers of Total Candidate Boxes    
    INT32 CandidateCurrent;    //!< Current Candidate Index (0,1,2,...)    
    INT32 nCircleCandidate;    //!< We select buffer by iBuf = (CandidateCurrent%nCircleCandidate), Max Value = 4.
    INT32 nStepsScreenFullToMiddleBox; //!<nSteps for FullScreen -> MiddleBox Animation
    BOOL  bEnableMirrorAlpha;  //!< Paste a image with alpha + vertical flip
    //#NT#2010/03/26#Niven Cho -begin
    //#NT#Add Alpha Constant 
    UINT8 iConstantAlpha;      //!< 0: Auto Gradient Alpha, 1~255 is Constant Alpha Value
    //#NT#2010/03/26#Niven Cho -end
    BOOL  bCircleLoop;         //!< No Boundary Limitation, User can move direction by circle loop
    //#NT#2010/05/03#Niven Cho -begin
    //#NT#User Mirror Distance 
    INT32 DistMirrorGap;   //!< Distance Between Image and Mirror Image, Normal is 1
    //#NT#2010/05/03#Niven Cho -end

    //User Callbacks, can be NULL (for MultiView_Begin and MultiView_Move)
    void   (*OnMoveBegin)(UINT32 TotalStep);        //!< Begin Animation, Info the total step to clinet
    void   (*OnMove)(UINT32 iStep,UINT32 nStep);    //!< Drawing Animation, Info the current step and total step to you
    void   (*OnMoveEnd)(void);                      //!< End Animation, Info the animation is finished
}tMULTI_VIEW_INIT;

typedef struct _MULTI_VIEW_SINGLE_INIT
{
    tMULTI_VIEW_BUF_DESC DescSrc[2];    //!< Source Buffer,[0]:Original Size, [1]:Scaled to Focuse Size
    tMULTI_VIEW_BUF_DESC DescDst[2];    //!< Destination Buffer (IDE Buffer,Use Double)
    UINT32 WorkingBufAddr;              //!< Working Buffer (Require Source Image 3*(LineOffest_Y * Height)
    UINT32 WorkingBufSize;              //!< Working Buffer Size
    tMULTI_VIEW_RECT rcSrc;             //!< Source Window On Display
    INT32 MirrorHeight;                 //!< Mirror Height
    //User Callbacks, can be NULL (for MultiView_Begin and MultiView_Move)
    void   (*OnMoveBegin)(UINT32 TotalStep);        //!< Begin Animation, Info the total step to clinet
    void   (*OnMove)(UINT32 iStep,UINT32 nStep);    //!< Drawing Animation, Info the current step and total step to you
    void   (*OnMoveEnd)(void);                      //!< End Animation, Info the animation is finished
}tMULTI_VIEW_SINGLE_INIT;

typedef enum{
    MULTI_VIEW_MOVE_DIR_LEFT = 0,
    MULTI_VIEW_MOVE_DIR_RIGHT
}MULTI_VIEW_MOVE_DIR;

//Init by 2 Methods
extern BOOL MultiView_Init(tMULTI_VIEW_INIT* pInit);        //!< return 'FALSE' is init failed! (Multi-View)
extern BOOL MultiView_Single_Init(tMULTI_VIEW_SINGLE_INIT* pInit); //!< return 'FALSE' is init failed! (Single-View)
//Moving Operation (Multi-View)
extern BOOL MultiView_Begin(void);                          //!< Begin Scale Form RectBegin to Middle Box
extern BOOL MultiView_End(void);                            //!< Scale Form RectBegin to Middle Box Full Screen 
extern BOOL MultiView_Refresh(void);                        //!< Only Update Each Boxes Content
extern BOOL MultiView_Move(MULTI_VIEW_MOVE_DIR Direction);  //!< Move Boxes by a Direction
extern UINT32 MultiView_GetCurrentCandidate(void);          //!< Get Current Candidate
extern void MultiView_SetCurrentCandidate(UINT32 iSel);     //!< Set Current Candidate
extern BOOL MultiView_Break(void);                          //!< Break Moving Animation
//Moving Operation (Single-View)
extern BOOL MultiView_SlideEffect(BOOL (*fpMVSE)(void));    //!< Input MVSE_xxxxxx to do slide effect
extern BOOL MVSE_FadingWithScaleDown(void);
extern BOOL MVSE_FadingBlockChecker(void);
extern BOOL MVSE_PrefectFix(void);
extern BOOL MVSE_BlendingLargeSize(void);

#ifdef __cplusplus
}
#endif

#endif