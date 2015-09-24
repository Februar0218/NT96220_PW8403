//#NT#2009/01/20#Connie Yu -begin
#ifndef _AFTRACKING_H_
#define _AFTRACKING_H_

extern UINT16 GetTrackingWindow_CenterX(void);
extern UINT16 GetTrackingWindow_CenterY(void);
extern void Start_Subject_Tracking(void);
extern void Stop_Subject_Tracking(void);
extern void Subject_Tracking(void);
extern unsigned long GetTrackingStableCnt(void);
//extern void SetRTAF(BOOL enable);
//extern void RTAF_SearchRange(unsigned short MinIndex, unsigned short MaxIndex);

extern INT32 abs(INT32 i);
//extern void RTAF_WindowSetting(unsigned short IMEStartX, unsigned short IMEStartY);
//extern void RT_AF2(void);
extern BOOL IsTrackingEn(void);
//extern void OSDxyShowStr(char *p,UINT16 x,UINT16 y, UINT color);
//extern void OSD_ShowDate(char *data, unsigned short X, unsigned short Y);

//#NT#2009/02/06#Harry Tsai -begin
/**
    @param UINT16 SizeX, Object window horizontal size
    @param UINT16 SizeY, Object window vertical size
    @param UINT16 SubWindow_SizeX, the sub-Window horizontal size of Object window// should be 8
    @param UINT16 SubWindow_SizeY, the sub-Window vertical size of Object window
*/
extern void setObjectWindowSize(UINT16 SizeX, UINT16 SizeY, UINT16 SubWindow_SizeX, UINT16 SubWindow_SizeY);
extern void setAftrackingThershold(unsigned short StableDistanceX, unsigned short StableDistanceY,unsigned short Th_OutOfScreen,unsigned short Th_BackToScreen);
extern void setY_C_Weighting(UINT16 W_Y,UINT16 W_C);
//#NT#2009/02/06#Harry Tsai -end

#endif
//#NT#2009/01/20#Connie Yu -end

