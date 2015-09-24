#ifndef _QSORT_H
#define _QSORT_H

#include <stddef.h>

// QuickSort
typedef int     cmp_t(const void *, const void *);
extern void     _qsort(void *a, size_t n, size_t es, cmp_t *cmp);

#endif
