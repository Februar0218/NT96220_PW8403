# ifndef _KERCFG_H
# define _KERCFG_H

    COMMENT /* Copyright (c) 2005  Novatek Microelectronics Corporation. All Rights Reserved.

    COMMENT File Name:
    COMMENT     KerCfg.h

    COMMENT Abstract:
    COMMENT     Definitions for DRAM Module. (16MB or 32MB)
    COMMENT     This is a shared header file for ASM and C
    COMMENT     For more information, please refer to http://www.arm.com/support/faqdev/1208.html
    COMMENT     Only support "# define" now

    COMMENT Notes:
    COMMENT     The total size of heap and stack must match the cache region size boundary.
    COMMENT     For example: 4KB 8KB 16KB 32KB 64KB 128KB 256KB 512KB 1MB ... */

    COMMENT /* STACK */
# define StackSize_SVC          0x1000      /* 4KB */
# define StackSize_ABT          0x1000      /* 4KB */
# define StackSize_UNDEF        0x1000      /* 4KB */
# define StackSize_FIQ          0x1000      /* 4KB */
# define StackSize_IRQ          0x1000      /* 4KB */
# define StackSize_USR          0x1000      /* 4KB */

    COMMENT /* Now DRAM_END is defined in MakeConfig.txt */
# define StackEnd_USR           DRAM_END
# define StackEnd_IRQ           (StackEnd_USR - StackSize_USR)
# define StackEnd_FIQ           (StackEnd_IRQ - StackSize_IRQ)
# define StackEnd_UNDEF         (StackEnd_FIQ - StackSize_FIQ)
# define StackEnd_ABT           (StackEnd_UNDEF - StackSize_UNDEF)
# define StackEnd_SVC           (StackEnd_ABT - StackSize_ABT)

    COMMENT /* C RUNTIME */
# define HeapSize_CRuntime      0x2000      /* 8KB */
# define HeapEnd_CRuntime       (StackEnd_SVC - StackSize_SVC)

    END

# endif
