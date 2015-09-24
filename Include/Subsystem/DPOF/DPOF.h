/*
    DPOF module.

    This file is the driver of CPU module.

    @file       DPOF.c
    @ingroup    mIDPOF
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _DPOF_H
#define _DPOF_H


/**
    @addtogroup mISYSDPOF
*/
//@{
//#NT#2008/10/29#Marx Chiu -begin
#define DPOF_MRK_FULL_PATH      "A:\\MISC\\AUTPRINT.MRK"
//#NT#2008/10/29#Marx Chiu -end

/**
    @name   DPOF Module Status

    Definition for DPOF Module Opened or Not
*/
//@{
#define DPOF_STATUS_UNINITIALED         0   ///< DPOF module not opened yet
#define DPOF_STATUS_INITIALED           1   ///< DPOF module opened
//@}

/**
    @name   Parse-choice for DPOF File

    Parse DPOF File and check whether it exists or not
*/
//@{
#define DPOF_READ_ONLY                  2   ///< Read DPOF file only
#define DPOF_READ_AND_CHECK             1   ///< Read DPOF file and check each image exist or not
//@}

/**
    @name   DPOF File Modification

    Definition for update DPOF File or not
*/
//@{
#define DPOF_NO_UPDATE                  2   ///< Not Update DPOF file
#define DPOF_UPDATE                     1   ///< Update DPOF file
//@}

/**
    @name   Returned-Value of DPOF APIs

    Definition for DPOF APIs' returned value
*/
//@{
#define DPOF_OK                         0x0000  ///< Success
#define DPOF_FAIL                       0x0001  ///< Fail
#define DPOF_NO_FILE                    0x0011  ///< No such file exists
#define DPOF_NOT_JPEG                   0x0101  ///< Not a jpeg file
#define DPOF_DSC_UNKNOW                 0xFF    ///< DPOF module not opened
//@}

/**
    @name   Update DPOF Setting

    Definition for DPOF setting update type.
*/
//@{
#define DPOF_DSC_UPDATENOW              1       ///< Update DPOF file immediately
#define DPOF_DSC_UPDATELATER            0       ///< Update DPOF file later
//@}


/**
    @name   DPOF Header Element

    Enumerate DPOF header elements.
*/
//@{
typedef enum
{
    DPOF_HDR_REV,   ///<:Revision of DPOF header
    DPOF_HDR_CRT    ///<:Creator of DPOF header
}DPOF_HDR_ITEM;
//@}

/**
    @name   DPOF Access Type

    Enumerate DPOF Access Type.
*/
//@{
typedef enum
{
    DPOF_GET,   ///<:Get the data
    DPOF_SET    ///<:Set the data
}DPOF_ACCESS_TYPE;
//@}

/**
    @name   DPOF Date Format

    Enumerate DPOF Date Format.
*/
//@{
typedef enum
{
    DPOF_DSC_DATE_YMD,  ///<:Year/Month/Date
    DPOF_DSC_DATE_DMY,  ///<:Date/Month/Year
    DPOF_DSC_DATE_MDY   ///<:Month/Date/Year
}DPOF_DSC_DATE_FORMAT;
//@}

/**
    @name   DPOF Date-Time Format

    Enumerate DPOF Date-Time Format.
*/
//@{
typedef enum
{
    DPOF_DSC_DATE_TIME, ///<:Both Date and Hour
    DPOF_DSC_DATE,      ///<:Date only
    DPOF_DSC_TIME       ///<:Time only
}DPOF_DSC_FORMAT;
//@}


/**
    Structure for DPOF Setting-Object

*/
//@{
typedef struct
{
    UINT8   uiQty;  ///< Qualtity of an image
    UINT8   uiDSC;  ///< Date-Print or not
    UINT16  rev;    ///< reserved
}DPOF_INFO_OBJ, *PDPOF_INFO_OBJ;
//@}


extern void DPOFOpen(UINT8 mode, UINT32 buffAddr);
extern void DPOFClose(UINT8 mode);
extern UINT16 DPOF_DeleteJob(UINT16 DeleteID, UINT16 DirID);
extern UINT16 DPOF_SetQty(UINT16 SearchID, UINT8 Qty, UINT16 DirID);
extern UINT16 DPOF_SetDate(UINT8 YesOrNo);
extern void DPOF_DeleteAll_and_DeleteMISC(void);
extern UINT16 DPOF_SetAll(UINT8 Confirm, UINT8 Qty, UINT8 DSC);
extern UINT8 DPOF_GetAll(void);
extern UINT8 DPOF_GetQty(UINT16 SearchID, UINT16 DirID);
extern UINT16 DPOF_SetOneDateStr(UINT16 fileID, UINT8 Confirm, UINT16 DirID, char *pDscStr);
extern UINT16 DPOF_GetTotalJobs(void);
extern UINT8 DPOF_GetDPOFInfo(UINT16 fileID, UINT16 DirID, DPOF_INFO_OBJ *pDPOFInfo);
extern UINT8 DPOF_AccessHeaderItem(DPOF_HDR_ITEM item, DPOF_ACCESS_TYPE accType, char *pRetVal, INT8 length);
extern void DPOF_SetDSCFormat(DPOF_DSC_FORMAT dscFormat);
extern DPOF_DSC_FORMAT DPOF_GetDSCFormat(void);
extern void DPOF_SetDSCDateFormat(DPOF_DSC_DATE_FORMAT dateFormat);
extern DPOF_DSC_DATE_FORMAT DPOF_GetDSCDateFormat(void);
extern UINT16  DPOF_IsCurrExist(UINT16 fileID, UINT16 DirID);
extern UINT16 DPOF_GetTotalDatePrt(void);
extern UINT16 DPOF_GetTotalPrints(void);
extern UINT16 DPOF_SetCrop(UINT16 fileId, UINT16 dirId, UINT16 cropX, UINT16 cropY, UINT16 cropW, UINT16 cropH);
extern UINT32 DPOF_IsOpened(void);
//#NT#2008/10/29#Marx Chiu -begin
extern BOOL DPOF_GetHandleNum(UINT32*     pHandleNum);
//#NT#2008/10/29#Marx Chiu -end

//@}

#endif //_DPOF_H

