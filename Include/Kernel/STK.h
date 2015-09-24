/*
    Process registers initial stack.

    @file       Stk.h
    @ingroup    mIKernelTask
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _STK_H
#define _STK_H

/*
! ! ! C A U T I O N ! ! !
The following stack structure does NOT
include VERSATILE and COPROCESSOR registers.
This means upon sta_tsk of a task, versatile
and coprocessor registers will NOT be
initialized in any way, regradless of
task-attribute assignment by cre_tsk.

! ! ! C A U T I O N ! ! !
DO NOT change the order of members !!
These must corespond with task_sch.
*/
struct  stk
{

    unsigned long   R4;                 /* 0 Registers to be saved by scheduler */
    unsigned long   R5;                 /* 4                                    */
    unsigned long   R6;                 /* 8                                    */
    unsigned long   R7;                 /* 12 c                                 */
    unsigned long   R8;                 /* 16 10                                */
    unsigned long   R9;                 /* 20 14                                */
    unsigned long   R10;                /* 24 18                                */
    unsigned long   R11;                /* 28 1c                                */
    unsigned long   R12;                /* 32 20                                */
    unsigned long   CPSR;               /* 36 24                                */
    void           *pci;                /* 40 28 User return address            */
    unsigned long   p1;                 /* 44 2c First argument                 */
};

/*
This value is set in sta_tsk to the stack of new task.
*/
// 0711 penny mark_s
//#define DEFAULT_SR_VALUE        0x00DF
// 0711 penny mark_end
#define DEFAULT_SR_VALUE        0x001F      // defult enter task enable IRQ & FIQ

#endif
