/**
    @file       GPSParser.h
    @ingroup

    @brief      Sample module brief
                Sample module description

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _NMEAParser_H
#define _NMEAParser_H

#include "OSCommon.h"
#include "Type.h"
#include "UART.h"

#define _GPS_CheckSum_Enable      0   //Enable to open check sum mechanism
#define RECEIVE_FROM_UART2        0

#define NMEA_BUFFER_OFFSET       (35*1024)   //35KB
#define NMEA_BUFFER_RESERVED     128
#define NMEA_SENTENCE_SIZE       180
#define GSA_SENTENCE             0x41534750 //'A''S''G''P'
#define RMC_SENTENCE             0x434D5250 //'C''M''R''P'
#define GGA_SENTENCE             0x41474750 //'A''G''G''P'
#define GSV_SENTENCE             0x56534750 //'V''S''G''P'
#define GLL_SENTENCE             0x4C4C4750 //'L''L''G''P'
#define VTG_SENTENCE             0x47545650 //'G''T''V''P'
#define ZDA_SENTENCE             0x41445A50 //'A''D''Z''P'
#define SNR_SENTENCE             0x4F534E45 //'O''S''N''E'
#define ROS_SENTENCE             0x534F5259 //'S''O''R''Y'

#define GPS_FLAG_DATAVALID      0x00000001
#define GPS_FLAG_LOG            0x00000002

//#NT#2011/1/14#Philex Lin-begin
typedef struct {
    CHAR    *pGPSDataStartAddr;
    UINT32  GPSBufUsedPos;
    UINT32  GPSBufSize;
} GPSBuf,*pGPSBuf;
//#NT#2011/1/14#Philex Lin-end
typedef struct{

    CHAR    *GGA;
    CHAR    *RMC;
    CHAR    *GSA;
    CHAR    *GSV;
    CHAR    *GLL;
    CHAR    *VTG;
    GPSBuf  GPSDataBuf;

}NMEASENTENCE;
extern NMEASENTENCE NMEASentence;

typedef enum{
    No_Fix = 1,
    TWO_D_Fix,
    THREE_D_Fix
}FIXMODE;


//GSA, Satellite status
typedef struct{

    CHAR    SelMode;          //'A' auto selection of 2D or 3D fix
                              //'M' manual
    UINT32  FixMode;          //'1' no fix
                              //'2' 2D fix
                              //'3' 3D fix
    UINT32  SatUsed01;        //PRNs of satellites used for fix (space for 12)
    UINT32  SatUsed02;
    UINT32  SatUsed03;
    UINT32  SatUsed04;
    UINT32  SatUsed05;
    UINT32  SatUsed06;
    UINT32  SatUsed07;
    UINT32  SatUsed08;
    UINT32  SatUsed09;
    UINT32  SatUsed10;
    UINT32  SatUsed11;
    UINT32  SatUsed12;
    FLOAT   PDOP;             //Position dilution of precision
    FLOAT   HDOP;             //Horizontal dilution of precision
    FLOAT   VDOP;             //Vertical dilution of precision

}GSAINFO;
extern GSAINFO GSAInfo;

//RMC, Recommended Minimum sentence C
typedef struct{

    UINT32    Hour;
    UINT32    Minute;
    UINT32    Second;
    CHAR      Status;              //Status A=active or V=Void
    FLOAT     Latitude;
    CHAR      NSInd;
    FLOAT     Longitude;
    CHAR      EWInd;
    FLOAT     Speed;               //Speed over the ground in knots
    FLOAT     Angle;               //Track angle in degrees True
    UINT32    Year;
    UINT32    Month;
    UINT32    Day;
    CHAR       Reserve;          //jack lan
}RMCINFO;
extern RMCINFO RMCInfo;

//GGA, Global Positioning System Fix Data
typedef struct{

    UINT32    Hour;
    UINT32    Minute;
    UINT32    Second;
    FLOAT     Latitude;
    CHAR      NSInd;
    FLOAT     Longitude;
    CHAR      EWInd;
    UINT32    PosFixInd;         //Fix quality: 0 = invalid
                                 //             1 = GPS fix
    UINT32    SatNumber;         //Number of satellites being tracked
    FLOAT     HDOP;              //Horizontal dilution of position
    FLOAT     Altitude;          //Altitude, Meters, above mean sea level
    FLOAT     MSL;               //Height of geoid (mean sea level) above WGS84

}GGAINFO;
extern GGAINFO GGAInfo;

typedef struct{

    UINT32    SatNum;            //Satellite PRN number
    UINT32    Elev;              //Elevation, degrees
    UINT32    Azi;               //Azimuth, degrees
    UINT32    SNR;               //SNR - higher is better

}SATDATA;

//GSV, Satellites in view
typedef struct{

    UINT32    NumOfSen;          //Number of sentences for full data
    UINT32    SenNum;            //Sentence number
    UINT32    SatInView;         //Number of satellites in view
    SATDATA   SAT01;
    SATDATA   SAT02;
    SATDATA   SAT03;
    SATDATA   SAT04;
    SATDATA   SAT05;
    SATDATA   SAT06;
    SATDATA   SAT07;
    SATDATA   SAT08;
    SATDATA   SAT09;
    SATDATA   SAT10;
    SATDATA   SAT11;
    SATDATA   SAT12;
    SATDATA   SAT13;
    SATDATA   SAT14;
    SATDATA   SAT15;
    SATDATA   SAT16;

}GSVINFO;
extern GSVINFO GSVInfo;

typedef enum{
    GSA,
    RMC,
    GGA,
    GSV,
    GLL,
    VTG
}NMEATYPE;

extern ER GPSRec_Open(UART_BAUDRATE BaudRate);
extern ER GPSRec_Close(void);
extern ER GPSRec_Reset(void);
extern BOOL GPSRec_CheckData(void);

//#NT#2011/1/14#Philex Lin - begin
extern void     GPSRec_EnableGPSLog(BOOL bEnable);
extern void     GPSRec_SetGPSBufAddr(UINT32 uiGPSLogAddr,UINT32 uisize);
extern UINT32   GPSRec_GetGPSLogAddr(void);
extern ER       GPSRec_LogGPSData(void);
extern void     GPSLog_LogGPSData(void);
extern ER       GPSLog_Open(void);
extern ER       GPSLog_Close(void);
//#NT#2011/1/14#Philex Lin - end

#endif




