#ifndef _DPC_ALG_H
#define _DPC_ALG_H

/*
  Defect pixel
*/
#define DP_SEARCH_TILL_END  ENABLE
typedef enum
{
    DPC_SEARCH_METHOD_BAYER = 0x00,
    DPC_SEARCH_METHOD_VGA,
    DPC_SEARCH_METHOD_9PXL,

    ENUM_DUMMY4WORD(DPC_SEARCH_METHOD)
} DPC_SEARCH_METHOD;

typedef struct _DPC_SEARCH
{
//    UINT32 Method;
    UINT32 BayAdd;
    UINT16 Width;
    UINT16 Height;
    UINT16 LineOffset;
    UINT32 CapFields;

    UINT32 DPadd;
    UINT16 OffsetX;
    UINT16 OffsetY;
    UINT16 StartCnt;
    UINT16 MaxCnt;
    UINT8 Threshold;
    UINT16 Block_X;
    UINT16 Block_Y;
} DPC_SEARCH, *PDPC_SEARCH;

/**
  defect pixel search API
*/
extern void DPC_SetSearchInfo(DPC_ALG_INFO param);
extern UINT32 DPC_DataSearch(UINT32 Method);
extern void DPC_SortDPdata(UINT32* Data,UINT32 Num);

#endif // EOF
