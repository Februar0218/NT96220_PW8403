#ifndef   _PTP_API_H
#define   _PTP_API_H
#include "PTP_ObjectManage_FS.h"

typedef enum {
    PTP_CMD_OPEN_SESSION,
    PTP_CMD_GET_DEV_DESC,
    PTP_CMD_GET_DEV_VAL,
    PTP_CMD_CLOSE_SESSION,
    PTP_CMD_SENDOBJ_INFO,
    PTP_CMD_GETOBJ
} PTP_CMD_ENUM;

typedef enum {
    PTP_TYPE_PRINTER,
    PTP_TYPE_PC
} PTP_TYPE;

typedef enum {
    PTP_FOR_AP_TXRX,
    PTP_FOR_AP_FDB
} PTP_FOR_AP_TYPE;

typedef void (*PTP_DPS_CB)(PTP_CMD_ENUM status);
extern PTP_DPS_CB         g_fpPTPDPS;
extern PTP_TYPE           g_PCorPTP;

PPTP_OBJECTLINK PTP_AddObjToSICDList
(
    UINT32 objhandle,
    UINT8 *filename,
    PTP_OBJECTINFO *objinfo,
    PTP_OBJ_INFO_FILL_CB      ptp_obj_fill_objinfo_cb,
    PTP_OBJ_READ_DATA_CB      ptp_obj_read_data_cb,
    PTP_OBJ_WRITE_DATA_CB     ptp_obj_write_data_cb,
    PTP_OBJ_RWFIN_NOTIFY_CB   ptp_obj_rwfin_notify_cb,
    PTP_OBJ_DEL_DATA_CB       ptp_obj_del_data_cb
);
BOOL PTP_DelObjFromSICDList(UINT32 objhandle);
BOOL PTP_SendEvent(USHORT  IntEvent, UINT32 Param_cnt, UINT32 *P1);
BOOL PTP_RegNotifyCb(INT8   *filename, PTP_NOTIFY_CB    cb);
BOOL PTP_RegDPSStatusCb(PTP_DPS_CB    cb);
void PTP_SetPTPType(PTP_TYPE type);
void PTP_GetPTPType(PTP_TYPE *ptpType);
void PTP_Get_AP_TYPE(PTP_FOR_AP_TYPE mem_type,UINT8 **ptr,UINT32 *len);
BOOL PTP_Init(UINT32 mem_pool_addr,UINT32 mem_pool_len);
extern void USICD_GetPrintAllData(
    UINT32*     startaddr,
    UINT32*     totalImage);

extern void PTP_CommandProcess(UINT16 uiCmdCode);
extern void PTP_StopTransfer(void);
extern void PTP_ResetDevice(void);
extern void PTP_setSendObjBufAddrSize(UINT32 addr, UINT32 size);

#endif

