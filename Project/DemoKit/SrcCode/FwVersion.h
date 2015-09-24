#ifndef _FW_VERSION_H
#define _FW_VERSION_H


//-----------------------------------------------------------------------------
// DSC Firmware Version String
//-----------------------------------------------------------------------------
#if (PRJ == HA60)
#define DSC_FW_VERSION_STR          "HA60FW00"
#elif (PRJ == ADPK_DSC_DEMO)
#define DSC_FW_VERSION_STR          "FW00"
#elif (PRJ == TDV5160)
#define DSC_FW_VERSION_STR          "TDV5160FW01"
#elif (PRJ == APC3)
#define DSC_FW_VERSION_STR          "APC3FW01"
#else
#define DSC_FW_VERSION_STR          "NTK"
#endif


#endif

