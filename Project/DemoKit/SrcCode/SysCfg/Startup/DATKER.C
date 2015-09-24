#include "Kernel.h"
#include "SysCfg.h"

// Task
struct tcb  gtcbarr[STD_TSK_NUM+USER_TSK_NUM];  // Array of tcb
struct tcb* gtcbadr[STD_TSK_NUM+USER_TSK_NUM];  // Array of pointers to tcb

// Semaphore
struct sem  gsemarr[STD_SEM_NUM+USER_SEM_NUM];  // Array of sem
struct sem* gsemadr[STD_SEM_NUM+USER_SEM_NUM];  // Array of pointers to sem

// Flag
struct flg  gflgarr[STD_EVF_NUM+USER_EVF_NUM];  // Array of flg
struct flg* gflgadr[STD_EVF_NUM+USER_EVF_NUM];  // Array of pointers to flg

// Mailbox
struct mbx  gmbxarr[STD_MBX_NUM+USER_MBX_NUM];  // Array of mbx
struct mbx* gmbxadr[STD_MBX_NUM+USER_MBX_NUM];  // Array of pointers to mbx

// Fixed memory pool
struct fpl  gfplarr[STD_FPL_NUM+USER_FPL_NUM];  // Array of fpl
struct fpl* gfpladr[STD_FPL_NUM+USER_FPL_NUM];  // Array of pointers to fpl

// Fixed memory pool start
VP          gfremem = (void*)FRE_MEM;

// Fixed memory pool end
VP          gendmem = (void*)END_MEM;

// Total tcb number
const int   gtcbnum = STD_TSK_NUM + USER_TSK_NUM;

// Total sem number
const int   gsemnum = STD_SEM_NUM + USER_SEM_NUM;

// Total flg number
const int   gflgnum = STD_EVF_NUM + USER_EVF_NUM;

// Total mbx number
const int   gmbxnum = STD_MBX_NUM + USER_MBX_NUM;

// Total fpl number
const int   gfplnum = STD_FPL_NUM + USER_FPL_NUM;

// Total buf number
const int   gbufnum = BUF_NUM;

