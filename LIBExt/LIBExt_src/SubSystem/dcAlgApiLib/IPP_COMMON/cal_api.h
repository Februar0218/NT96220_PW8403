#ifndef _CAL_API_H
#define _CAL_API_H
#if 0
/**
  Get calibration RAW
*/
typedef enum
{
    RAW_TYPE_PREVIEW = 0x00,
    RAW_TYPE_STILL,

    ENUM_DUMMY4WORD(RAW_TYPE)
} RAW_TYPE;

typedef struct
{
    UINT32 Addr;
    UINT32 Height;
    UINT32 Width;
    UINT32 StartX;
    UINT32 StartY;
    UINT32 SizeX;
    UINT32 SizeY;
    UINT32 Sample;
    UINT8 StartPixel;
} CAL_RAW_INFO;

//cal ae api
extern void Cal_WaitAeStable(CAL_AE_INFO *AeInfo);
#endif
#endif // EOF
