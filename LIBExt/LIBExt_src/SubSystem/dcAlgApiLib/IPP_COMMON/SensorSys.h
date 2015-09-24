#ifndef _SENSORSYS_H_
#define _SENSORSYS_H_

void SetDzoom2Preview(unsigned int ZoomIndex);
void SetDzoom2Video(unsigned int ZoomIndex);

//void SetSiePreCapParam(SIE_PRE_PARAM *SiePreInfo);
void siepre_GetCapRaw(SIE_PRE_PARAM *SiePreParam);
//void siepre_GetCapRawHighSpeed(SIE_PRE_PARAM *SiePreParam);
void sie_PrvParameter(struct  PUBLIC_STR_JOBREQS *job_req);
void sie_CapParameter(struct  PUBLIC_STR_JOBREQS *job_req);
void sie_D2DParameter(struct  PUBLIC_STR_JOBREQS *job_req);
//void IPPFunc_OnOffSwitch(void);
void AAFD_LockSwitch(UINT32 ImgMode);
UINT32 WriteDebugData(unsigned char *Add);
void Sensor_Cap2Prv(void);
//UINT32* Get_ChromaInfo(void);
void Set2Video(PIPPCTRL_INFO info, PIPPCTRL_RST result);
void Set2Preview(PIPPCTRL_INFO info, PIPPCTRL_RST result);

//extern const UINT32 CapBufAlloc[Max_BurstNum][3];
//#NT#2010/05/07#Daniel -BESTQUALITY- -begin
// modify for internal test
//extern unsigned char JPEG_Quality[];
//extern UINT8 JPEG_Quality[];
//#NT#2010/05/07#Daniel -BESTQUALITY- -end

void AF_PrvModeSel(UINT32 Mode, UINT32 ModeType, UINT32 step);
UINT32 AeProcessEnd(UINT32 ISO, UINT32 ExposureTime);
extern UINT32 Get_CapBufAlloc(UINT32 type);
extern void SetSiePreCapParam(SIE_PRE_PARAM *SiePreInfo);
extern void Get_CapInfo(UINT InfoID, UINT *ReturnInfo);
extern void CaptureOne(SIE_PRE_PARAM *SiePreParam);
extern void IPPFunc_OnOffSwitch(void);
#endif
