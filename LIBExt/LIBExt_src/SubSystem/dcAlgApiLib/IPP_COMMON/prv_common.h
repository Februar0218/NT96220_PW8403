#ifndef _PRV_COMMON_H
#define _PRV_COMMON_H
#include "IPPManager.h"

extern UINT32 Set2Prv_Common(IPP_PARAM *IppParam_Prv, UINT32 SenInitEn, BOOL bPath2);

extern void sie_Set2Preview(SIE_PRE_PARAM *SiePreParam);
extern void sie_Set2Dzoom(void);
extern void sie_Set2Video(SIE_PRE_PARAM *SiePreParam);

extern void ipe_Set2Preview(IPE_PARAM *IpeParam);
extern void ipe_ife_Set2Preview(pIPE_PARAM _IpeParam, pIFE_ENG_SETTING _pIfeParam);
extern void ipe_Set2Video(IPE_PARAM *IpeParam);
extern void ipe_ife_Set2Video(pIPE_PARAM _IpeParam, pIFE_ENG_SETTING _pIfeParam);

extern void ime_Set2Preview(pIME_FBADDR_PRAM  pIME_Para1,pIME_EXTEN pIME_Ext1);
extern void ime_Set2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2);
extern void ime_SetDzoom2Video(pIME_FBADDR_PRAM  pIME_Para1,pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext1,pIME_EXTEN pIME_Ext2);
extern void ime_SetDzoom2Video_Path1(pIME_FBADDR_PRAM  pIME_Para1);
extern void ime_SetDzoom2Video_Path2(pIME_FBADDR_PRAM  pIME_Para2,pIME_EXTEN pIME_Ext2);

extern void ipe_ime_set2dzoom(void);
#endif
