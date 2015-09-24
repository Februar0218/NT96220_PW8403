/**
    CPU module driver.

    This file is the driver of CPU module.

    @file       Cache.h
    @ingroup    mIHALCore
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _CACHE_H
#define _CACHE_H

#include "Type.h"

/**
    @addtogroup mIHALCore
*/
//@{

/**
    @name   Cache property

    Definition for cache property
*/
//@{
#define UNCACHEABLE                 0       ///< Un-cacheable
#define CACHEABLE                   1       ///< Cacheable
//@}

/**
    @name   Buffer property

    Definition for buffer property
*/
//@{
#define UNBUFFERABLE                0       ///< For data cache only, Un-bufferable
#define BUFFERABLE                  1       ///< For data cache only, bufferable
//@}

/**
    @name   Region Size

    Definition for memory region size
*/
//@{
#define MEM_REGION_4K               0x0B    ///< 4K bytes
#define MEM_REGION_8K               0x0C    ///< 8K bytes
#define MEM_REGION_16K              0x0D    ///< 16K bytes
#define MEM_REGION_32K              0x0E    ///< 32K bytes
#define MEM_REGION_64K              0x0F    ///< 64K bytes
#define MEM_REGION_128K             0x10    ///< 128K bytes
#define MEM_REGION_256K             0x11    ///< 256K bytes
#define MEM_REGION_512K             0x12    ///< 512K bytes
#define MEM_REGION_1M               0x13    ///< 1M bytes
#define MEM_REGION_2M               0x14    ///< 2M bytes
#define MEM_REGION_4M               0x15    ///< 4M bytes
#define MEM_REGION_8M               0x16    ///< 8M bytes
#define MEM_REGION_16M              0x17    ///< 16M bytes
#define MEM_REGION_32M              0x18    ///< 32M bytes
#define MEM_REGION_64M              0x19    ///< 64M bytes
#define MEM_REGION_128M             0x1A    ///< 128M bytes
#define MEM_REGION_256M             0x1B    ///< 256M bytes
#define MEM_REGION_512M             0x1C    ///< 512M bytes
#define MEM_REGION_1G               0x1D    ///< 1G bytes
#define MEM_REGION_2G               0x1E    ///< 2G bytes
#define MEM_REGION_4G               0x1F    ///< 4G bytes
//@}

/**
    @name   Access Right

    Definition for region access right
*/
//@{
#define NO_ACCESS                   0x0     ///< No access for any mode
#define PRIV_ONLY                   0x1     ///< Privilege mode R/W enable only, User mode not accessible
#define USER_RONLY                  0x2     ///< Privilege mode R/W enable, User mode read only
#define RW_ALL                      0x3     ///< R/w enable for all modes
//@}

// Cache region API
extern void     CPUSetInstrRegionBaseSize(UINT32 region, UINT32 addr, UINT32 size, UINT32 enable);
extern void     CPUSetDataRegionBaseSize(UINT32 region, UINT32 addr, UINT32 size, UINT32 enable);

//#NT#2009/11/09#HH Chuang -begin
//#NT#support cache flush in DMA driver
extern BOOL     CPUChkDCacheEnabled(UINT32 addr);
//#NT#2009/11/09#HH Chuang -end
extern void     CPUDisableAllDataRegion(void);
extern void     CPUDisableAllInstrRegion(void);

extern void     CPUSetInstrRegionProtect(UINT32 region, UINT32 mode);
extern void     CPUSetDataRegionProtect(UINT32 region, UINT32 mode);


// Cache setting API
extern void     CPUSetICacheCacheable(UINT32 region, UINT32 cacheable);
extern void     CPUSetDCacheCacheable(UINT32 region,UINT32 cacheable);

extern void     CPUSetWriteBufable(UINT32 region, UINT32 bufferable);

extern void     CPUSetICacheEnable(UINT32 enable);
extern void     CPUSetDCacheEnable(UINT32 enable);

extern void     CPUSetBTBEnable(UINT32 enable);
extern void     CPUSetAlignCheckEnable(UINT32 enable);
extern void     CPUSetExtEnable(UINT32 enable);

extern void     CPUSetMMUEnable(UINT32 enable);

// Cache clean/invalidate API
extern void     CPUInvalidateICacheAll(void);
extern void     CPUInvalidateICache(UINT32 addr);
extern void     CPUInvalidateICacheBlock(UINT32 start, UINT32 end);

extern void     CPUInvalidateDCacheAll(void);
extern void     CPUInvalidateDCache(UINT32 addr);
extern void     CPUInvalidateDCacheBlock(UINT32 start, UINT32 end);

extern void     CPUCleanDCacheAll(void);
extern void     CPUCleanDCache(UINT32 addr);
extern void     CPUCleanDCacheBlock(UINT32 start, UINT32 end);

extern void     CPUCleanInvalidateDCacheAll(void);
extern void     CPUCleanInvalidateDCache(UINT32 addr);
extern void     CPUCleanInvalidateDCacheBlock(UINT32 start, UINT32 end);

extern void     CPUFlushBTBAll(void);
extern void     CPUDrainWriteBuffer(void);

// Cache lock/unlock API
extern void     CPUPrefetchICache(UINT32 addr);
extern void     CPUPrefetchDCache(UINT32 addr);

extern void     CPULockICache(void);
extern void     CPUUnlockICache(void);

extern void     CPULockDCache(void);
extern void     CPUUnlockDCache(void);

//Idle/Sleep API
extern void     CPUEnterIdleMode(void);
extern void     CPUEnterSleepMode(void);

#define FA510_CACHE_SIZE             4096
#define FA510CleanInvalidateDcache(b, s)    (s >= FA510_CACHE_SIZE)?CPUCleanInvalidateDCacheAll():CPUCleanInvalidateDCacheBlock((UINT32)b, (UINT32)(b+s))
#endif

//@}
