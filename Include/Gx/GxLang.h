#ifndef _GX_LANGUAGE_H_
#define _GX_LANGUAGE_H_

#include "GxData.h"

#define UICode_Thai     1   //����
#define UICode_Hindi    2   //�_�L�פ�
#define UICode_Arabic   3   //���ԧB��
#define UICode_Persian  4   //�i����
#define UICode_Hebrew   5   //�ƧB�Ӥ�
#define UICode_MAX      6

void GxString_SetLang(UINT32 uiLang, BOOL bReverse);

void GxString_TransCode(WCHAR* strUICode, const WCHAR* strUnicode, int n);

#endif //_GX_LANGUAGE_H_