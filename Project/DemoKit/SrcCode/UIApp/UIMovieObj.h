#ifndef UI_MOVIEOBJ_H
#define UI_MOVIEOBJ_H

#include "UIFramework.h"
#include "NVTUserCommand.h"
#include "GPS.h"
#include "Gsensor.h"

extern VControl UIMovieObjCtrl;
extern UINT32   GetMovieSizeWidth(UINT32 ubIndex);
extern UINT32   GetMovieSizeHeight(UINT32 ubIndex);
extern UINT32   GetMovieFrameRate(UINT32 ubIndex);
extern void     SetMovieFrameRate(UINT32 value);
extern UINT32   GetMovieQualityValue(UINT16 ubIndex);
extern UINT32   GetMovieTargetBitrate(UINT16 ubIndex);

extern UINT32   GetMovieSizeWidth_Low(UINT32 ubIndex);
extern UINT32   GetMovieSizeHeight_Low(UINT32 ubIndex);
extern UINT32   GetMovieFrameRate_Low(UINT32 ubIndex);
extern UINT32 GetMovieTargetBitrate_Low(UINT16 ubIndex);

//#NT#2009/04/13#Lily Kao -begin
#define UIPLAYAVI_SPEED_X1_IDX          0
#define UIPLAYAVI_SPEED_X2_IDX          1
#define UIPLAYAVI_SPEED_X4_IDX          2

typedef enum
{
    RECMOVIE_MAXSECOND,
    RECMOVIE_GET_DATATYPE_MAX
}RECMOVIE_GET_DATATYPE;


typedef enum
{
    PLAYMOVIE_DIRID,
    PLAYMOVIE_FILEID,
    PLAYMOVIE_TOTAL_FRAME,
    PLAYMOVIE_TOTAL_SECOND,
    PLAYMOVIE_CURR_FRAME,
    PLAYMOVIE_CURR_SECOND,
    PLAYMOVIE_LOCKSTATUS,
    PLAYMOVIE_FILENUM,
    PLAYMOVIE_GET_DATATYPE_MAX
}PLAYMOVIE_GET_DATATYPE;

typedef enum
{
    PLAYMOVIE_SPEEDL1,
    PLAYMOVIE_SPEEDL2,
    PLAYMOVIE_SPEEDL4,
    PLAYMOVIE_SET_DATATYPE_MAX
}PLAYMOVIE_SET_DATATYPE;


typedef struct {
    RMCINFO         rmcinfo;
    Gsensor_Data    gs_data;

//#NT#2011/10/14#Spark Chou -begin
//#NT# Video IQ debug info
#if _MOVIE_IQLOG_
    char    IQDebugInfo[5120];
#endif
//#NT#2011/10/14#Spark Chou -end
} GPSDATA,*pGPSDATA;
extern UINT32 UIPlayMovie_GetData(PLAYMOVIE_GET_DATATYPE dataType);
extern void UIPlayMovie_SetData(PLAYMOVIE_SET_DATATYPE dataType);
extern UINT32 UIRecMovie_GetData(RECMOVIE_GET_DATATYPE dataType);
//#NT#2009/04/13#Lily Kao -end


#endif
