#ifndef _GX_LANGUAGE_H_
#define _GX_LANGUAGE_H_

#include "GxData.h"

#define UICode_Thai     1   //泰文
#define UICode_Hindi    2   //北印度文
#define UICode_Arabic   3   //阿拉伯文
#define UICode_Persian  4   //波斯文
#define UICode_Hebrew   5   //希伯來文
#define UICode_MAX      6

void GxString_SetLang(UINT32 uiLang, BOOL bReverse);

void GxString_TransCode(WCHAR* strUICode, const WCHAR* strUnicode, int n);

#endif //_GX_LANGUAGE_H_