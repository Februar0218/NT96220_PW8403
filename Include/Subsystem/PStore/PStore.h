/**
    @file       PStore.h
    @ingroup    mISYSPStore

    @brief      PStore driver export variables and function prototypes.

    Copyright   Novatek Microelectronics Corp. 2006.  All rights reserved.

*/


#ifndef _PSTORE_H
#define _PSTORE_H

#include "OSCommon.h"
//#NT#2007/03/16#CL Wang -begin
#include "StrgDef.h"
//#NT#2007/03/16#CL Wang -begin

/**
    @addtogroup mISYSPStore
@{
*/


/**
    @name PStore read / write operation attribute.
    These bits can be OR'd.
*/
//@{
#define PS_RDONLY           0x1     ///<Read only
#define PS_WRONLY           0x2     ///<Write only
#define PS_RDWR             0x3     ///<Read and write
#define PS_CREATE           0x4     ///<Create a new section when the section not existence.  Can be OR'd with read and write flag.
//@}

#define PS_UPDATE_ALLHEAD   0x1
#define PS_UPDATE_ONEHEAD   0x2
#define PS_UPDATE_ZEROHEAD  0x3

#define PS_RESTORE_HEAD1    0x1
#define PS_RESTORE_HEAD2    0x2
/**
    @name PStore error code.
*/
//@{
#define E_PS_OK             E_OK
#define E_PS_PAR            E_PAR
#define E_PS_SECHDLER       NULL    ///< Section handle error
#define E_PS_SYS            E_SYS   ///< System error */
#define E_PS_CHKER          (-100)  ///< header of P-Store checksum error */
#define E_PS_EMPTY          (-101)  ///< P-Store is empty*/
#define E_PS_VERNEW         (-102)  ///< The New structure of P-Store is newer than the NAND*/
#define E_PS_VEROLD         (-103)  ///< The New structure of P-Store is older than the NAND*/
#define E_PS_NANDNOTOPEN    (-104)  ///< Device has not opened yet*/
#define E_PS_PSNOTOPEN      (-105)  ///< PStore has not opened yet*/
#define E_PS_PSOPEN         (-106)  ///< PStore has opened*/
#define E_PS_SECNOTOPEN     (-107)  ///< Section has not opened yet*/
#define E_PS_RDONLY         (-108)  ///< PStore is opened as read only*/
#define E_PS_WRONLY         (-109)  ///< PStore is opened as read only*/
#define E_PS_SECNOTFOUND    (-110)  ///< Section not found*/
#define E_PS_SECNOTCLOSE    (-111)  ///< Section not close*/
//#NT#2007/03/16#CL Wang -begin
#define E_PS_GETRIGHTFAIL   (-112)  ///< Get access right fail*/
//#NT#2007/03/16#CL Wang -end
//#NT#2007/03/16#CL Wang -begin
#define E_PS_NOSTRGOBJ      (-113)  ///< Storage object not fount*/
//#NT#2007/03/16#CL Wang -end

//#NT#2009/01/15#Brad Chen -begin
#define E_PS_NAMEFAIL      (-114)  ///< pstore Name Error */
//#NT#2009/01/15#Brad Chen -end

//#NT#2009/06/26#Brad Chen -begin
#define E_PS_HEADFAIL      (-115)  ///< pstore not  complete */
//#NT#2009/06/26#Brad Chen -end

//@}

typedef UINT32 PS_SECNAME_ID;

#define MAKE_PS_SECNAME_ID(a,b,c,d)     (((d)<<24)|((c)<<16)|((b)<<8)|((a)))

/**
    PStore header structure.
*/
typedef struct
{
    UINT32 uiSignature;             ///<Just for identification.
    UINT16 uiPSVersion;             ///<Version of PStore storage.
    UINT16 uiSectionNum;            ///<How many sections the PStore have.//Read only
    UINT32 uiPStoreSize;            ///<PStore size in byte unit (for NAND flash device, please make sure the PStoreSize that a multiple of block size).
//#NT#2007/11/24#CL Wang -begin
    UINT32 uiPSDrvVersion;
//#NT#2007/11/24#CL Wang -end
    UINT32 uiCheckSum;              ///<Please keep check sum item is the last element of PS structure.
} PSTORE, *PPSTORE;


/**
    Section descriptor structure.
    Each section structure is consist of 8 bytes, and please don't change the structure size(for section structure array).
*/
typedef struct
{
    PS_SECNAME_ID uiSectionName;    ///<Section name ID
    UINT32 uiReserved;              ///<Reserved for future use
} PSTORE_SECTION_DESC, *PPSTORE_SECTION_DESC;


/**
    Section handle structure.
*/
typedef struct
{
    UINT32 uiSectionName;       ///<uiSectionName = 0xFFFFFFFF, means a free section handle.
    UINT16 uiSectionID;
    UINT8  ucRWOperation;       ///<Bit flag of PS_RDONLY, PS_WRONLY, PS_RDWR, PS_CREATE
    UINT8  ucSectionUpdate;
} PSTORE_SECTION_HANDLE, *PPSTORE_SECTION_HANDLE;

extern INT32    PStore_Init(PSTORE *pNewPStoreStruct, PSSTRG_TAB *PPSStrg);
extern PSTORE_SECTION_HANDLE* PStore_OpenSection(PS_SECNAME_ID ulSecName, UINT8 RWOperation);
extern INT32    PStore_CloseSection(PSTORE_SECTION_HANDLE* pSectionHandle);

//#NT#2007/04/03#CL Wang -begin
//#For Delete and clear function
extern INT32    PStore_DeleteSection(PS_SECNAME_ID ulSecName);
extern INT32 PStore_ClearSection(PS_SECNAME_ID ulSecName, UINT32 ulStart, UINT32 ulEnd);
//#NT#2007/04/03#CL Wang -end

extern INT32    PStore_ReadSection( UINT8 *pcBuffer,UINT32 ulStartAddress, UINT32 ulDataLength, PSTORE_SECTION_HANDLE* pSectionHandle);
extern INT32    PStore_WriteSection(UINT8 *pcBuffer, UINT32 ulStartAddress, UINT32 ulDataLength, PSTORE_SECTION_HANDLE* pSectionHandle);

//#NT#2007/10/17#CL Wang -begin
//For debug purpose
extern void     PStore_DumpPStoreInfo(void);
extern INT32    PStore_DumpOneSectionData(UINT16 usSectionID);
//#NT#2007/10/17#CL Wang -end

extern INT32    PStore_Format(PSTORE *pPS_Struct);
extern INT32    PStore_Upgrade(PSTORE* pPStore, PSTRG_TAB pStorageObj, PSSTRG_TAB *PPSStrg);

//#NT#2008/07/29#CL Wang -begin
//extern INT32    PStore_GetHeadPara(PSTORE *pPS_Struct);
//extern INT32    PStore_SetHeadPara(PSTORE *pPS_Struct, PSTORE_SECTION_DESC *pSec_Struct);
//#NT#2008/07/29#CL Wang -end

extern void  PStore_DisablePS(void);
extern void  PStore_EnablePS(void);

//#NT#2008/01/15#Brad Chen -begin
//#check P_store Name
extern ER PStore_CheckName(UINT32 Name);
//#NT#2008/01/15#Brad Chen -end


//#NT#2009/06/26#Brad Chen -begin
//#check P_store Section
extern ER PStore_CheckSectionBlockNum(UINT32 SectCount,UINT32  * ErrorSection);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/06/26#Brad Chen -begin
//#set update header
extern ER PStore_EnableUpdateHeader(UINT32 Header);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/06/26#Brad Chen -begin
//#set restore header
extern ER PStore_RestoreHeader(UINT8 * pBuf , UINT32 RestoreType);
//#NT#2009/06/26#Brad Chen -end

//#NT#2009/07/01#Brad Chen -begin
//#check P_store single Section
extern ER PStore_CheckSingleSection(UINT32 SectIndex,UINT32  * ErrorSection);
//#NT#2009/07/01#Brad Chen -begin

//#NT#2009/11/06#Brad Chen -begin
//# Enable Write Section
extern ER PStore_EnableTotalWriteSection(UINT32 Enable);
//#NT#2009/11/06#Brad Chen -end

//#NT#2009/11/06#Brad Chen -begin
//# Enable Write Section
extern ER PStore_EnableWriteSingleSection(PSTORE_SECTION_HANDLE* pSectionHandle, UINT32 Enable);
//#NT#2009/11/06#Brad Chen -end

#endif // _PSTORE_H

//@}
