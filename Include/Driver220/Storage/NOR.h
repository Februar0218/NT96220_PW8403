#ifndef _NORDEF_H
#define _NORDEF_H

#include "Strgdef.h"

#define NOR_BOOT_TOP            1
#define NOR_BOOT_BOTTOM         -1
#define NOR_BASE_ADDRESS        0x20000000
#define NOR_HIGH_ADDR_PROBLEM   1

//#NT#2010/06/30#HH Chuang -begin
//#NT#detect invalid nor flash access
/**
    NOR operation

    @note For NOR_INVALID_WRITE_CB()
*/
typedef enum
{
    NOR_OPERATION_READ,         ///< read operation
    NOR_OPERATION_WRITE,        ///< write operation
    NOR_OPERATION_ERASE,        ///< erase operation
    ENUM_DUMMY4WORD(NOR_OPERATION)
} NOR_OPERATION;

/**
    NOR invalid write callback

    Callback routine to be invoked when invalid erase/write access is detected by driver.

    @note nor_setProtectRange()

    @param[in] first parameter  Operating type
                                - @b NOR_OPERATION_ERASE: erase
                                - @b NOR_OPERATION_WRITE: write
    @param[in] second parameter Starting address
    @param[in] third parameter  Access length

    @return void
*/
typedef void (*NOR_INVALID_WRITE_CB)(NOR_OPERATION, UINT32, UINT32);
//#NT#2010/06/30#HH Chuang -end

typedef struct
{
    UINT32  ulTotalSize;                   //How many bytes the memory device has.
    UINT32  ulNumberOfSector;              //How many sectors the memory device have (minimum earsed unit).
    UINT32  ulSectorSize;                  //How many bytes the sector has. (Minimum erase unit)
    UINT32  ulBlockSize;                   //How many bytes a block has. (Minimum PStore access unit)
    signed char   cBootDirection;          //Boot sector is located in the top sector or bottom sector (1 for boot from top, -1 for boot from bottom).
    UINT8   ucSiliconID ;
    UINT8   ucDeviceID;
}NORSTRG, *PNORSTRG;


//#NT#2011/12/16#Shirley Cheng -begin
/**
    NOR WRITEREAD direction

    NOR WRITEREAD direction, use for WRITEREAD_CMD(parameter uiByte) of nor_ctrlOp().
*/
//@{
#define NOR_WRITEREAD_CMDREAD    0x80000000
#define NOR_WRITEREAD_CMDWRITE   0x0
//@}


/**
    NOR Control Parameter Definition

    This definition is used as the second operand in the nor_ctrlOp API to specify the nor_ctrlOp control parameters.
*/
//@{
typedef struct
{
    union
    {
        /**
            WRITEREAD CMD

            This structure is used for display control operation code "NORCTRL_WRITEREAD_CMD"
            to write/read the flash cmd.
            note: This operation is single BYTE cmd execution, if > 0xFF cmd/data must be transfer, please use other operation API. 
        */
        struct
        {
            UINT8       uiCnt;     ///< WRITEREAD counter, valid value = 0x0~0xFF, please use other opeartion API.
            UINT32      *uiByte;   ///< data pointer of transfer data, only 8-bit is valid.
                                   ///< bit 31 is indicate Read or Write, bit31==1:read, bit31==0:write
            BOOL        bCsDeSel;  ///< TRUE: CS de-select when finish these CMD,FALSE: CS not de-select when finish these CMD
        } WRITEREAD_CMD;

    }SEL;
}NORCTRL_PARAM,*PNORCTRL_PARAM;
//@}

/**
    nor flash control operation ID definition

    This field is used as the first operand in the nor_ctrlOp API to specify 
    which operation would be performed.
*/
typedef enum
{
    /* CMD  */
    NORCTRL_WRITEREAD_CMD,         ///< Set the WRITEREAD CMD to flash

    ENUM_DUMMY4WORD(NORCTRL_OP)
}NORCTRL_OP;
//#NT#2011/12/16#Shirley Cheng -end

extern PSTRG_TAB nor_getStorageObject(void);

extern BOOL nor_getDeviceInfo(PNORSTRG pNorInfo);
extern BOOL nor_eraseSector(UINT32 SectorAddr);
extern BOOL nor_programSector(UINT32 sector_addr, UINT32 sector_size, UINT8 *data);
extern BOOL nor_readByBytes(UINT32 addr, UINT32 size, UINT8 *data);
extern ER   nor_open(void);
extern ER   nor_close(void);
extern BOOL nor_eraseChip(void);
extern void nor_setGPIOForCS(UINT32 ulGPIO);
//For storage object
ER nor_writeSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
ER nor_readSectors(INT8 *pcBuf, UINT32 ulSecNum, UINT32 ulSctCnt);
ER nor_format(void);

extern void nor_printerRetry(UINT32* ulE, UINT32* ulP, UINT32* ulR);
extern void nor_ClearRetry(void);

//#NT#2010/06/30#HH Chuang -begin
//#NT#detect invalid nor flash access
extern void nor_setProtectRange(UINT32 uiByteAddr, UINT32 uiByteSize, NOR_INVALID_WRITE_CB pInvalidWriteHdl);
//#NT#2010/06/30#HH Chuang -end

//#NT#2011/12/16#Shirley Cheng -begin
extern ER   nor_ctrlOp(NORCTRL_OP nor_op, PNORCTRL_PARAM nor_param);
//#NT#2011/12/16#Shirley Cheng -end
#endif
