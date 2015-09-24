#ifndef _PLATFORM_H
#define _PLATFORM_H

#if defined(__arm )
#define _ALIGNED(x) __align(x)
#define _PACKED_BEGIN __packed
#define _PACKED_END
#define _INLINE static __inline
#define _ASM_NOP __asm{nop;}
#define _MIPS_TODO 1
#elif defined( __GNUC__)
#define _ALIGNED(x) __attribute__((aligned(x)))
#define _PACKED_BEGIN
#define _PACKED_END __attribute__ ((packed))
#define _INLINE static inline
#define _ASM_NOP __asm__("nop");
#define _MIPS_TODO 0
#endif

#endif
