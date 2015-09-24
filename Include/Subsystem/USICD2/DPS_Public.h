/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       DPS_Public.h
    @ingroup    mISYSDPS
    @brief      DPS (PictBridge) Functions for DSC system.
                DPS (PictBridge) Functions for DSC system.

*/

#ifndef DPS_PUBLIC_H
#define DPS_PUBLIC_H
//-------------------------------------------------------------------------------------------------
typedef enum
{
    //None:0
    DPS_EVENT_NONE,
    //Init:1
    DPS_EVENT_INIT_OK,
    DPS_EVENT_INIT_FAIL,
    //DPS End:3
    DPS_EVENT_END,
    //Print:4
    DPS_EVENT_PRINT_FINISH,
    //Notify Job:5
    DPS_EVENT_NOTIFY_JOB,
    //Disconnect:6
    DPS_EVENT_DISCONNET_ENABLE,
    //Capability Change:7
    DPS_EVENT_CAPABILITY_CHANGED,
    //Start Job:8
    DPS_EVENT_START_JOB_OK,
    DPS_EVENT_START_JOB_FAIL,
    //Abort Job:10
    DPS_EVENT_ABORT_JOB_OK,
    DPS_EVENT_ABORT_JOB_FAIL,
    //Continue Job:12
    DPS_EVENT_CONTINUE_JOB_OK,
    DPS_EVENT_CONTINUE_JOB_FAIL,
    //Warning:Paper:14
    DPS_EVENT_WARNING,
    DPS_EVENT_WARNING_PAPER,
    DPS_EVENT_WARNING_PAPER_EMPTY,
    DPS_EVENT_WARNING_PAPER_LOAD,
    DPS_EVENT_WARNING_PAPER_EJECT,
    DPS_EVENT_WARNING_PAPER_MEDIA,
    DPS_EVENT_WARNING_PAPER_JAM,
    DPS_EVENT_WARNING_PAPER_NEARLY_EMPTY,
    DPS_EVENT_WARNING_PAPER_NOT_FOUND,
    //Warning:Ink:23
    DPS_EVENT_WARNING_INK,
    DPS_EVENT_WARNING_INK_EMPTY,
    DPS_EVENT_WARNING_INK_LOW,
    DPS_EVENT_WARNING_INK_WASTE,
    //Warning:Hardware:27
    DPS_EVENT_WARNING_HARDWARE,
    DPS_EVENT_WARNING_HARDWARE_FATAL_ERROR,
    DPS_EVENT_WARNING_HARDWARE_SERVICE_CALL,
    DPS_EVENT_WARNING_HARDWARE_UNAVAILABLE,
    DPS_EVENT_WARNING_HARDWARE_BUSY,
    DPS_EVENT_WARNING_HARDWARE_LEVER,
    DPS_EVENT_WARNING_HARDWARE_COVER_OPEN,
    DPS_EVENT_WARNING_HARDWARE_MARKING_AGENT,
    DPS_EVENT_WARNING_HARDWARE_INK_OPEN,
    DPS_EVENT_WARNING_HARDWARE_NO_INK,
    //Warning:File:37
    DPS_EVENT_WARNING_FILE,
    DPS_EVENT_WARNING_FILE_PRINTINFO,
    DPS_EVENT_WARNING_FILE_DECODE,
    //Fatal Error:Paper:40
    DPS_EVENT_ERROR,
    DPS_EVENT_ERROR_PAPER,
    DPS_EVENT_ERROR_PAPER_EMPTY,
    DPS_EVENT_ERROR_PAPER_LOAD,
    DPS_EVENT_ERROR_PAPER_EJECT,
    DPS_EVENT_ERROR_PAPER_MEDIA,
    DPS_EVENT_ERROR_PAPER_JAM,
    DPS_EVENT_ERROR_PAPER_NEARLY_EMPTY,
    DPS_EVENT_ERROR_PAPER_NOT_FOUND,
    //Fatal Error:Ink:49
    DPS_EVENT_ERROR_INK,
    DPS_EVENT_ERROR_INK_EMPTY,
    DPS_EVENT_ERROR_INK_LOW,
    DPS_EVENT_ERROR_INK_WASTE,
    //Fatal Error:Hardware:53
    DPS_EVENT_ERROR_HARDWARE,
    DPS_EVENT_ERROR_HARDWARE_FATAL_ERROR,
    DPS_EVENT_ERROR_HARDWARE_SERVICE_CALL,
    DPS_EVENT_ERROR_HARDWARE_UNAVAILABLE,
    DPS_EVENT_ERROR_HARDWARE_BUSY,
    DPS_EVENT_ERROR_HARDWARE_LEVER,
    DPS_EVENT_ERROR_HARDWARE_COVER_OPEN,
    DPS_EVENT_ERROR_HARDWARE_MARKING_AGENT,
    DPS_EVENT_ERROR_HARDWARE_INK_OPEN,
    DPS_EVENT_ERROR_HARDWARE_NO_INK,
    //Fatal Error:File:63
    DPS_EVENT_ERROR_FILE,
    DPS_EVENT_ERROR_FILE_PRINTINFO,
    DPS_EVENT_ERROR_FILE_DECODE,
    //Unknow Error status:66
    DPS_EVENT_UNKNOWN_ERROR_STATUS,
    //XML syntax error:67
    DPS_EVENT_XML_SYNTAX_ERROR
}DPS_EVENT_ID;
//-------------------------------------------------------------------------------------------------
typedef void (*DPS_EVENT_HANDLER)(DPS_EVENT_ID  EVNET_ID);
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DPS_VENDOR_INFO_DPS_VERSION,
    DPS_VENDOR_INFO_SPECIFIC_VERSION,
    DPS_VENDOR_INFO_VENDOR_NAME,
    DPS_VENDOR_INFO_PRODUCT_NAME,
    DPS_VENDOR_INFO_SERIAL_NO
}DPS_VENDOR_INFO;
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DPS_DEVICE_STATUS_INDEX_DPSS,
    DPS_DEVICE_STATUS_INDEX_JER,
    DPS_DEVICE_STATUS_INDEX_ES,
    DPS_DEVICE_STATUS_INDEX_ER,
    DPS_DEVICE_STATUS_INDEX_DE,
    DPS_DEVICE_STATUS_INDEX_CC,
    DPS_DEVICE_STATUS_INDEX_NJO
}DPS_DEVICE_STATUS_INDEX;
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DPS_PRINT_SERVICE_STATUS_NONE,
    DPS_PRINT_SERVICE_STATUS_PRINTING,
    DPS_PRINT_SERVICE_STATUS_IDLE,
    DPS_PRINT_SERVICE_STATUS_PAUSED
}DPS_PRINT_SERVICE_STATUS;

typedef enum
{
    DPS_JOB_END_REASON_NONE,
    DPS_JOB_END_REASON_NOT_END,
    DPS_JOB_END_REASON_NORMALLY,
    DPS_JOB_END_REASON_ABORT_IMMEDIATELY,
    DPS_JOB_END_REASON_ABORT_AFTER_CURRENT_PAGE,
    DPS_JOB_END_REASON_OTHER
}DPS_JOB_END_REASON;

typedef enum
{
    DPS_ERROR_STATUS_NONE,
    DPS_ERROR_STATUS_NO_ERROR,
    DPS_ERROR_STATUS_WARNING,
    DPS_ERROR_STATUS_FATAL_ERROR
}DPS_ERROR_STATUS;

typedef enum
{
    DPS_ERROR_REASON_NONE,
    DPS_ERROR_REASON_NO_REASON,
    DPS_ERROR_REASON_PAPER,
    DPS_ERROR_REASON_PAPER_EMPTY,
    DPS_ERROR_REASON_PAPER_LOAD,
    DPS_ERROR_REASON_PAPER_EJECT,
    DPS_ERROR_REASON_PAPER_MEDIA,
    DPS_ERROR_REASON_PAPER_JAM,
    DPS_ERROR_REASON_PAPER_NEARLY_EMPTY,
    DPS_ERROR_REASON_PAPER_NOT_FOUND,
    DPS_ERROR_REASON_INK,
    DPS_ERROR_REASON_INK_EMPTY,
    DPS_ERROR_REASON_INK_LOW,
    DPS_ERROR_REASON_INK_WASTE,
    DPS_ERROR_REASON_HARDWARE,
    DPS_ERROR_REASON_HARDWARE_FATAL_ERROR,
    DPS_ERROR_REASON_HARDWARE_SERVICE_CALL,
    DPS_ERROR_REASON_HARDWARE_UNAVAILABLE,
    DPS_ERROR_REASON_HARDWARE_BUSY,
    DPS_ERROR_REASON_HARDWARE_LEVER,
    DPS_ERROR_REASON_HARDWARE_COVER_OPEN,
    DPS_ERROR_REASON_HARDWARE_MARKING_AGENT,
    DPS_ERROR_REASON_HARDWARE_INK_OPEN,
    DPS_ERROR_REASON_HARDWARE_NO_INK,
    DPS_ERROR_REASON_FILE,
    DPS_ERROR_REASON_FILE_PRINTINFO,
    DPS_ERROR_REASON_FILE_DECODE
}DPS_ERROR_REASON;

typedef enum
{
    DPS_DISCONNECT_ENABLE_NONE,
    DPS_DISCONNECT_ENABLE_FALSE,
    DPS_DISCONNECT_ENABLE_TRUE
}DPS_DISCONNECT_ENABLE;

typedef enum
{
    DPS_CAPABILITY_CHANGED_NONE,
    DPS_CAPABILITY_CHANGED_FALSE,
    DPS_CAPABILITY_CHANGED_TRUE
}DPS_CAPABILITY_CHANGED;

typedef enum
{
    DPS_NEW_JOB_OK_NONE,
    DPS_NEW_JOB_OK_FALSE,
    DPS_NEW_JOB_OK_TRUE
}DPS_NEW_JOB_OK;
//-------------------------------------------------------------------------------------------------
typedef enum
{
    DPS_CAPITEM_NONE,               //0
    DPS_CAPITEM_QUALITY,            //1
    DPS_CAPITEM_PAPERTYPE,          //2
    DPS_CAPITEM_PAPERSIZE,          //3
    DPS_CAPITEM_PAPERSIZE_MORE,     //4
    DPS_CAPITEM_FILETYPES,          //5
    DPS_CAPITEM_DATEPRINTS,         //6
    DPS_CAPITEM_NAMEPRINTS,         //7
    DPS_CAPITEM_IMGOPT,             //8
    DPS_CAPITEM_LAYOUTS,            //9
    DPS_CAPITEM_FIXSIZE,            //10
    DPS_CAPITEM_CROPPINGS           //11
}DPS_CAPITEM;
//-------------------------------------------------------------------------------------------------
//50000000
#define DPS_CAP_QUALITY_DEF         0x01 //default
#define DPS_CAP_QUALITY_NOR         0x02 //normal
#define DPS_CAP_QUALITY_DRA         0x04 //draft
#define DPS_CAP_QUALITY_FIN         0x08 //fine
//-------------------------------------------------------------------------------------------------
//51000000
#define DPS_CAP_PAPERSIZE_TOTAL     34 // total number of paper size
#define DPS_CAP_PAPERSIZE_DEF       0x00000001 //default
#define DPS_CAP_PAPERSIZE_01L       0x00000002 // L 89mmx127mm
#define DPS_CAP_PAPERSIZE_02L       0x00000004 // 2L 127mmx178mm
#define DPS_CAP_PAPERSIZE_POS       0x00000008 // postcard 100x148mm
#define DPS_CAP_PAPERSIZE_CARD      0x00000010 // card 54x85.6mm
#define DPS_CAP_PAPERSIZE_150       0x00000020 // 100x150mm
#define DPS_CAP_PAPERSIZE_4x6       0x00000040 // 4x6 101x152mm
#define DPS_CAP_PAPERSIZE_8x10      0x00000080 // 8x10 203x254mm
#define DPS_CAP_PAPERSIZE_LET       0x00000100 // letter 216x279mm
#define DPS_CAP_PAPERSIZE_11x17     0x00000200 // 11x17 279x431mm
#define DPS_CAP_PAPERSIZE_A0        0x00000400
#define DPS_CAP_PAPERSIZE_A1        0x00000800
#define DPS_CAP_PAPERSIZE_A2        0x00001000
#define DPS_CAP_PAPERSIZE_A3        0x00002000
#define DPS_CAP_PAPERSIZE_A4        0x00004000
#define DPS_CAP_PAPERSIZE_A5        0x00008000
#define DPS_CAP_PAPERSIZE_A6        0x00010000
#define DPS_CAP_PAPERSIZE_A7        0x00020000
#define DPS_CAP_PAPERSIZE_A8        0x00040000
#define DPS_CAP_PAPERSIZE_A9        0x00080000
#define DPS_CAP_PAPERSIZE_B0        0x00100000
#define DPS_CAP_PAPERSIZE_B1        0x00200000
#define DPS_CAP_PAPERSIZE_B2        0x00400000
#define DPS_CAP_PAPERSIZE_B3        0x00800000
#define DPS_CAP_PAPERSIZE_B4        0x01000000
#define DPS_CAP_PAPERSIZE_B5        0x02000000
#define DPS_CAP_PAPERSIZE_B6        0x04000000
#define DPS_CAP_PAPERSIZE_B7        0x08000000
#define DPS_CAP_PAPERSIZE_B8        0x10000000
#define DPS_CAP_PAPERSIZE_B9        0x20000000
#define DPS_CAP_PAPERSIZE_89RO      0x40000000 //89mm roll
#define DPS_CAP_PAPERSIZE_127RO     0x80000000 //127mm roll
#define DPS_CAP_PAPERSIZE_100RO     0x00000001 //100mm roll
#define DPS_CAP_PAPERSIZE_210RO     0x00000002 //210mm roll
//-------------------------------------------------------------------------------------------------
//52000000
#define DPS_CAP_PAPERTYPE_DEF       0x01//default
#define DPS_CAP_PAPERTYPE_PLA       0x02//plain paper
#define DPS_CAP_PAPERTYPE_PHO       0x04//photo paper
#define DPS_CAP_PAPERTYPE_FPHO      0x08//fast photo
//-------------------------------------------------------------------------------------------------
//53000000
#define DPS_CAP_FILETYPES_DEF       0x01
#define DPS_CAP_FILETYPES_EXIFJ     0x02 //exif/jpeg
#define DPS_CAP_FILETYPES_EXIF      0x04 //exif
#define DPS_CAP_FILETYPES_JPEG      0x08 //jpeg
#define DPS_CAP_FILETYPES_JFIF      0x10 // jfif
#define DPS_CAP_FILETYPES_ASSO      0x20 // association
#define DPS_CAP_FILETYPES_DPOF      0x40 // DPOF
//-------------------------------------------------------------------------------------------------
//54000000
#define DPS_CAP_DATEPRINT_DEF       0x01 // default
#define DPS_CAP_DATEPRINT_OFF       0x02 // off
#define DPS_CAP_DATEPRINT_ON        0x04 // on
//-------------------------------------------------------------------------------------------------
//55000000
#define DPS_CAP_NAMEPRINT_DEF       0x01
#define DPS_CAP_NAMEPRINT_OFF       0x02
#define DPS_CAP_NAMEPRINT_ON        0x04
//-------------------------------------------------------------------------------------------------
//56000000
#define DPS_CAP_IMGOPT_DEF          0x01
#define DPS_CAP_IMGOPT_OFF          0x02
#define DPS_CAP_IMGOPT_ON           0x04
//-------------------------------------------------------------------------------------------------
//57000000
#define DPS_CAP_LAYOUT_DEF          0x0001 //default
#define DPS_CAP_LAYOUT_01           0x0002 ///1-up
#define DPS_CAP_LAYOUT_02           0x0004 //2-up
#define DPS_CAP_LAYOUT_03           0x0008 //3-up
#define DPS_CAP_LAYOUT_04           0x0010 //4-up
#define DPS_CAP_LAYOUT_05           0x0020 //5-up
#define DPS_CAP_LAYOUT_06           0x0040 //6-up
#define DPS_CAP_LAYOUT_07           0x0080 //7-up
#define DPS_CAP_LAYOUT_08           0x0100 //8-up
#define DPS_CAP_LAYOUT_09           0x0200 //9-up
#define DPS_CAP_LAYOUT_10           0x0400 //16-up
#define DPS_CAP_LAYOUT_250UP        0x0800 //250-up
#define DPS_CAP_LAYOUT_IDX          0x1000 //index print
#define DPS_CAP_LAYOUT_NOB          0x2000 //borderless
//-------------------------------------------------------------------------------------------------
#define DPS_CAP_FIXSIZE_DEF         0x00000001//5800
#define DPS_CAP_FIXSIZE_2x3         0x00000002//5801
#define DPS_CAP_FIXSIZE_3x5         0x00000004//5802
#define DPS_CAP_FIXSIZE_4x6         0x00000008//5803
#define DPS_CAP_FIXSIZE_5x7         0x00000010//5804
#define DPS_CAP_FIXSIZE_8x10        0x00000020//5805
#define DPS_CAP_FIXSIZE_254         0x00000040//5806
#define DPS_CAP_FIXSIZE_110         0x00000080//5807
#define DPS_CAP_FIXSIZE_89          0x00000100//5808
#define DPS_CAP_FIXSIZE_100         0x00000200//5809
#define DPS_CAP_FIXSIZE_6_8         0x00000400//580A
#define DPS_CAP_FIXSIZE_7_10        0x00000800//580B
#define DPS_CAP_FIXSIZE_9_13        0x00001000//580C
#define DPS_CAP_FIXSIZE_10_15       0x00002000//580D
#define DPS_CAP_FIXSIZE_13_18       0x00004000//580E
#define DPS_CAP_FIXSIZE_15_21       0x00008000//580F
#define DPS_CAP_FIXSIZE_18_24       0x00010000//5810
#define DPS_CAP_FIXSIZE_A4          0x00020000//5811
#define DPS_CAP_FIXSIZE_LETTER      0x00040000//5812
//-------------------------------------------------------------------------------------------------
//59000000
#define DPS_CAP_CROPPINGS_DEF       0x01
#define DPS_CAP_CROPPINGS_OFF       0x02
#define DPS_CAP_CROPPINGS_ON        0x04
//-------------------------------------------------------------------------------------------------
typedef struct
{
    UINT32          qualities;
    UINT32          paperTypes;
    UINT32          paperSizes;
    UINT32          paperSizesMore;
    UINT32          fileTypes;
    UINT32          datePrints;
    UINT32          fileNamePrints;
    UINT32          imgOpti;
    UINT32          layouts;
    UINT32          fixedSizes;
    UINT32          croppings;
}DPS_CAPABILITY_INFO;

/**
    DPS Print Data.
*/
typedef struct
{
    UINT32          handle;
    UINT16          dirID;
    UINT16          fileID;
    UINT8           quantity;
    UINT8           zeroSize;       //For Backward Compatible, please do not use.
    UINT16          cropX;
    UINT16          cropY;
    UINT16          cropW;
    UINT16          cropH;
}DPS_PRINT_DATA_INFO;

/**
    Playback Print Information.
*/
typedef struct
{
    UINT16          total_images;//number of print images
    UINT16          total_prints;//number of paper prints
    UINT8           total_fPrints;
    UINT8           total_dPrints;
    UINT8           total_layouts;
    UINT8           rev1;
}DPS_PRINT_SETTING_INFO;

typedef struct
{
    DPS_PRINT_SERVICE_STATUS    prtServSts;
    DPS_JOB_END_REASON          jobEndReason;
    DPS_ERROR_STATUS            errSts;
    DPS_ERROR_REASON            errReason;
    DPS_DISCONNECT_ENABLE       disConnEn;
    DPS_CAPABILITY_CHANGED      capChg;
    DPS_NEW_JOB_OK              newJobOk;
}DPS_DEVICE_STATUS_INFO;
//-------------------------------------------------------------------------------------------------
extern void DPS_Close(void);
extern void DPS_Init(void);
extern void DPS_SetVendorInfo(
    DPS_VENDOR_INFO     Info,
    char*               Str);
extern void DPS_RegEventHandler(
    DPS_EVENT_HANDLER EventHandler);
extern void DPS_SetJobConfig(
    DPS_CAPITEM     CapItem,
    UINT32          CapItemContent);
extern void DPS_PrintImage(
    DPS_PRINT_DATA_INFO     *pPrintData,
    UINT16                  TotalImages,
    BOOL                    IsPrintAll,
    UINT16                  PrintAllCopy);
extern void DPS_PrintDPOF(void);
extern void DPS_PrintDPOFRestart(void);
extern void DPS_PrintAbort(void);
extern void DPS_PrintContinue(void);
extern UINT32 DPS_GetCapability(
    DPS_CAPITEM     CapItem);
extern void DPS_GetCapabilityAll(
    DPS_CAPABILITY_INFO*    pDPSGetCap);
extern UINT32 DPS_GetLayout(
    UINT32  PaperSize);
extern UINT32 DPS_GetPaperType(
    UINT32  PaperSize);
extern BOOL DPS_UpdateDeviceStatus(void);
extern DPS_DEVICE_STATUS_INFO* DPS_GetDeviceStatus(void);
extern BOOL DPS_UpdateJobStatus(void);
extern void DPS_GetJobStatus(
    UINT16  *pCurrent,
    UINT16  *pTotal);
//-------------------------------------------------------------------------------------------------
//For Backward Compatible - begin
//-------------------------------------------------------------------------------------------------
#define USICD_CLOSE_SESSION         DPS_EVENT_END

#define USICD_PRINTIMG_OK           DPS_EVENT_PRINT_FINISH
#define USICD_PRINTIMG_FAIL         DPS_EVENT_UNKNOWN_ERROR_STATUS

#define USICD_PRINT_NOTIFYJOB       DPS_EVENT_NOTIFY_JOB

#define USICD_CONT_JOB_OK           DPS_EVENT_CONTINUE_JOB_OK
#define USICD_CONT_JOB_FAIL         DPS_EVENT_CONTINUE_JOB_FAIL

#define USICD_DISCONN_ENABLE        DPS_EVENT_DISCONNET_ENABLE

#define USICD_CAP_CHANGED           DPS_EVENT_CAPABILITY_CHANGED

#define USICD_CANCEL_OK             DPS_EVENT_ABORT_JOB_OK

#define USICD_WARNING_PAPER_ERROR   DPS_EVENT_WARNING_PAPER
#define USICD_WARNING_PAPER_OUT     DPS_EVENT_WARNING_PAPER_EMPTY
#define USICD_WARNING_PAPER_LOAD    DPS_EVENT_WARNING_PAPER_LOAD
#define USICD_WARNING_PAPER_JAM     DPS_EVENT_WARNING_PAPER_JAM
#define USICD_WARNING_PAPER_NOMSG   DPS_EVENT_WARNING_PAPER_MEDIA
#define USICD_WARNING_PAPER_NOSUP   DPS_EVENT_WARNING_PAPER_NOT_FOUND

#define USICD_WARNING_INK_ERROR     DPS_EVENT_WARNING_INK
#define USICD_WARNING_INK_OUT       DPS_EVENT_WARNING_INK_EMPTY
#define USICD_WARNING_INK_LOW       DPS_EVENT_WARNING_INK_LOW

#define USICD_WARNING_PTR_ERROR     DPS_EVENT_WARNING_HARDWARE
#define USICD_WARNING_PTR_UC        DPS_EVENT_WARNING_HARDWARE_UNAVAILABLE
#define USICD_WARNING_PTR_OPEN      DPS_EVENT_WARNING_HARDWARE_COVER_OPEN
#define USICD_WARNING_PTR_HEAD      DPS_EVENT_WARNING_HARDWARE_MARKING_AGENT
#define USICD_WRANING_PTR_INK       DPS_EVENT_WARNING_HARDWARE_INK_OPEN
#define USICD_WARNING_PTR_CART      DPS_EVENT_WARNING_HARDWARE_NO_INK

#define USICD_WARNING_FILE_ERROR    DPS_EVENT_WARNING_FILE

#define USICD_ERROR_PAPER_ERROR     DPS_EVENT_ERROR_PAPER
#define USICD_ERROR_PAPER_OUT       DPS_EVENT_ERROR_PAPER_EMPTY
#define USICD_ERROR_PAPER_LOAD      DPS_EVENT_ERROR_PAPER_LOAD
#define USICD_ERROR_PAPER_JAM       DPS_EVENT_ERROR_PAPER_JAM
#define USICD_ERROR_PAPER_NOMSG     DPS_EVENT_ERROR_PAPER_MEDIA
#define USICD_ERROR_PAPER_NOSUP     DPS_EVENT_ERROR_PAPER_NOT_FOUND

#define USICD_ERROR_INK_ERROR       DPS_EVENT_ERROR_INK
#define USICD_ERROR_INK_OUT         DPS_EVENT_ERROR_INK_EMPTY
#define USICD_ERROR_INK_LOW         DPS_EVENT_ERROR_INK_LOW

#define USICD_ERROR_PTR_ERROR       DPS_EVENT_ERROR_HARDWARE
#define USICD_ERROR_PTR_UC          DPS_EVENT_ERROR_HARDWARE_UNAVAILABLE
#define USICD_ERROR_PTR_OPEN        DPS_EVENT_ERROR_HARDWARE_COVER_OPEN
#define USICD_ERROR_PTR_HEAD        DPS_EVENT_ERROR_HARDWARE_MARKING_AGENT
#define USICD_ERROR_PTR_INK         DPS_EVENT_ERROR_HARDWARE_INK_OPEN
#define USICD_ERROR_PTR_CART        DPS_EVENT_ERROR_HARDWARE_NO_INK

#define USICD_ERROR_FILE_ERROR      DPS_EVENT_ERROR_FILE
//-------------------------------------------------------------------------------------------------
#define DPS_PRT_SERV_STS_PRINTING   DPS_PRINT_SERVICE_STATUS_PRINTING
#define DPS_PRT_SERV_STS_IDLE       DPS_PRINT_SERVICE_STATUS_IDLE
#define DPS_PRT_SERV_STS_PAUSED     DPS_PRINT_SERVICE_STATUS_PAUSED

#define DPS_JOB_NOT_END             DPS_JOB_END_REASON_NOT_END
#define DPS_JOB_END_NORMALLY        DPS_JOB_END_REASON_NORMALLY
#define DPS_JOB_END_ABORT1          DPS_JOB_END_REASON_ABORT_IMMEDIATELY
#define DPS_JOB_END_ABORT2          DPS_JOB_END_REASON_ABORT_AFTER_CURRENT_PAGE
#define DPS_JOB_END_OTHER           DPS_JOB_END_REASON_OTHER

#define DPS_NO_ERROR                DPS_ERROR_STATUS_NO_ERROR
#define DPS_RECOVERABLE_ERROR       DPS_ERROR_STATUS_WARNING
#define DPS_FATAL_ERROR             DPS_ERROR_STATUS_FATAL_ERROR

#define DPS_NO_REASON               DPS_ERROR_REASON_NO_REASON
#define DPS_PAPER_ERROR             DPS_ERROR_REASON_PAPER
#define DPS_INK_ERROR               DPS_ERROR_REASON_INK
#define DPS_HARDWARE_ERROR          DPS_ERROR_REASON_HARDWARE
#define DPS_FILE_ERROR              DPS_ERROR_REASON_FILE

#define DPS_DISCONN_FALSE           DPS_DISCONNECT_ENABLE_FALSE
#define DPS_DISCONN_TRUE            DPS_DISCONNECT_ENABLE_TRUE

#define DPS_CAPCHG_FALSE            DPS_CAPABILITY_CHANGED_FALSE
#define DPS_CAPCHG_TRUE             DPS_CAPABILITY_CHANGED_TRUE

#define DPS_NEW_JOBOK_FALSE         DPS_NEW_JOB_OK_FALSE
#define DPS_NEW_JOBOK_TRUE          DPS_NEW_JOB_OK_TRUE
//-------------------------------------------------------------------------------------------------
#define pictB_registerCBFunction(a)             DPS_RegEventHandler(a)
#define pictB_setPTRCapability(a,b)             DPS_SetJobConfig(a,b)

#define pictB_printDPOF()                       DPS_PrintDPOF()
#define pictB_printImageAbort()                 DPS_PrintAbort()
#define pictB_printImageContinue()              DPS_PrintContinue()

#define pictB_getPTRCapability(a)               DPS_GetCapability(a)
#define pictB_getCapabilityItem(a)              DPS_GetCapabilityAll(a)

#define pictB_getLayoutsSpecific(a)             DPS_GetLayout(a)
#define pictB_getPaperTypesSpecific(a)          DPS_GetPaperType(a)

#define pictB_getPTRDeviceStatus()              DPS_UpdateDeviceStatus()
#define pictB_getPtrDevSts()                    DPS_GetDeviceStatus()

#define pictB_getPTRJobStatus()                 DPS_UpdateJobStatus()
#define pictB_getPTRnotifyJob(a,b)              DPS_GetJobStatus(a,b)
//-------------------------------------------------------------------------------------------------
#define pictB_getPTRStatus()                    DPS_UpdateDeviceStatus()
#define pictB_setPTRQuality(a)                  DPS_SetJobConfig(DPS_CAPITEM_QUALITY,a)
#define pictB_setPTRPaperSize(a)                DPS_SetJobConfig(DPS_CAPITEM_PAPERSIZE, a)
#define pictB_setPTRLayout(a)                   DPS_SetJobConfig(DPS_CAPITEM_LAYOUTS, a)
#define pictB_setPTRFixedSize(a)                DPS_SetJobConfig(DPS_CAPITEM_FIXSIZE, a)
//-------------------------------------------------------------------------------------------------
#define DPS_INIT_PARAMS     DPS_CAPABILITY_INFO

#define PB_PRINT_DATA       DPS_PRINT_DATA_INFO
#define PPB_PRINT_DATA      (DPS_PRINT_DATA_INFO*)

#define PB_PRINT_INFO       DPS_PRINT_SETTING_INFO
#define PPB_PRINT_INFO      (DPS_PRINT_SETTING_INFO*)

#define PTR_DEV_STS         DPS_DEVICE_STATUS_INFO
#define PPTR_DEV_STS        (DPS_DEVICE_STATUS_INFO*)
//-------------------------------------------------------------------------------------------------
__inline void pictB_printOneImage(
    UINT16  DirId,
    UINT16  FileId,
    UINT8   Quantity)
{
    DPS_PRINT_DATA_INFO     TempPrintData;
    TempPrintData.dirID = DirId;
    TempPrintData.fileID = FileId;
    TempPrintData.quantity = Quantity;
    TempPrintData.cropX = 0;
    TempPrintData.cropY = 0;
    TempPrintData.cropW = 0;
    TempPrintData.cropH = 0;

    DPS_PrintImage(
        &TempPrintData,
        1,
        FALSE,
        0);
}
//-------------------------------------------------------------------------------------------------
__inline void pictB_printMoreImagesEachSetting(
    DPS_PRINT_DATA_INFO     *pPrintData,
    UINT16                  TotalImages)
{
    DPS_PrintImage(
        pPrintData,
        TotalImages,
        FALSE,
        0);
}
//-------------------------------------------------------------------------------------------------
__inline void pictB_printMoreImages(
    DPS_PRINT_DATA_INFO     *pPrintData,
    UINT16                  TotalImages)
{
    DPS_PrintImage(
        pPrintData,
        TotalImages,
        FALSE,
        0);
}
//-------------------------------------------------------------------------------------------------
__inline void pictB_printAllImages(
    DPS_PRINT_DATA_INFO     *pPrintData,
    UINT16                  TotalImages,
    UINT16                  Copies)
{
    DPS_PrintImage(
        pPrintData,
        TotalImages,
        TRUE,
        Copies);
}
//-------------------------------------------------------------------------------------------------
//For Backward Compatible - end
//-------------------------------------------------------------------------------------------------

#endif

