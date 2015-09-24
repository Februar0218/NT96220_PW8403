#ifndef _FLASHLIGHT_API_H_
#define _FLASHLIGHT_API_H_


typedef struct FLASH_PARAM_STRUCT
{
    UINT32 Enable;
    UINT32 RedeyeEnable;
    UINT32 FakeFlash;
    UINT32 WidthMainFlash;
    UINT32 WidthPreFlash;
    UINT32 DelayLineMainFlash;
    UINT32 DelayLinePreFlash;
    UINT32 YPreFlash;
    UINT32 YNoFlash;
    UINT32 ISO_MainFlash;
    UINT32 ExpoTime_MainFlash;
} FLASH_PARAM;

extern void flash_RedEyeReduct(void);
extern void flash_Evaluate(void);
extern void flash_PreFlashFlow(void);
extern FLASH_PARAM* GetFlashParam(void);
extern void Flash_GetColorGian(UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);
//#NT#2012/05/20#KS Hung -begin
extern UINT32 Flash_WriteDebugInfo(UINT32 Addr);
//#NT#2012/05/20#KS Hung -end

#endif