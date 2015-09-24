/*
    System call IDs.

    @file       Support.h
    @ingroup    mIKernelCore
    @note       Not referenced now!

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _SC_ID_H
#define _SC_ID_H

/*
 *  Use only in kernel
*/
#define S_CRETSK         0          /* Create task                          */
#define S_STATSK         1          /* Start Task                           */
#define S_EXTTSK         2          /* Exit task                            */
#define S_TERTSK         3          /* Terminate task                       */
#define S_CHGPRI         4          /* Change process priority              */
#define S_ROTRDQ         5          /* Rotate ready queue                   */
#define S_RELWAI         6          /* Task release wait                    */
#define S_GETTID         7          /* Get own PID                          */
#define S_TSKSTS         8          /* Get task status                      */
#define S_SUSTSK         9          /* Suspend task                         */
#define S_RSMTSK        10          /* Resume task                          */
#define S_FRSMTS        11          /* Forced resume task                   */
#define S_SLPTSK        12          /* Wait for wakeup (sleep)              */
#define S_WAITSK        13          /* Wait for wakeup (time out)           */
#define S_WUPTSK        14          /* Wakeup task                          */
#define S_CANWUP        15          /* Cancel wakeup task                   */
#define S_CRESEM        16          /* Create semaphore                     */
#define S_SIGSEM        17          /* Signal semaphore                     */
#define S_WAISEM        18          /* Wait semaphore                       */
#define S_POLSEM        19          /* Polling semaphore                    */
#define S_SEMSTS        20          /* Get semaphore status                 */
#define S_CREFLG        21          /* Create event flags                   */
#define S_SETFLG        22          /* Set event flags                      */
#define S_CLRFLG        23          /* Clear event flags                    */
#define S_WAIFLG        24          /* Wait event flags                     */
#define S_POLFLG        25          /* Polling event flags                  */
#define S_FLGSTS        26          /* Get event flags status               */
#define S_CREMBX        27          /* Create mailbox                       */
#define S_SNDMSG        28          /* Send msg to mailbox                  */
#define S_RCVMSG        29          /* Receive msg from mailbox             */
#define S_POLMSG        30          /* Polling msg from mailbox             */
#define S_MBXSTS        31          /* Get mailbox status                   */
#define S_SDFINT        32          /* Define hardware interrupt for ISCH   */
#define S_DEFINT        33          /* Define hardware interrupt            */
#define S_RETINT        34          /* Return from interrupt                */
#define S_RETWUP        35          /* Wakeup task and return from interrupt*/
#define S_CREMPF        36          /* Define fixpool                       */
#define S_GETBLF        37          /* Get block from fixpool               */
#define S_POLBLF        38          /* Polling block from fixpool           */
#define S_RELBLF        39          /* Release block to fixpool             */
#define S_REFMPF        40          /* Get fixpoll status                   */
#define S_CREMPL        41          /* Define varpool                       */
#define S_GETBLK        42          /* Get memory from varpool              */
#define S_POLBLK        43          /* Polling memory from varpool          */
#define S_RELBLK        44          /* Release memory to varpool            */
#define S_REFMPL        45          /* Get varpool status                   */
#define S_DEFCYC        46          /* Define cyclic handler                */
#define S_ACTCYC        47          /* Activate cyclic hdl                  */
#define S_CYHSTS        48          /* Get cyclic hdl status                */
#define S_DEFALM        49          /* Define alarm handler                 */
#define S_ALHSTS        50          /* Get alarm hdl status                 */
#define S_GETVER        51          /* Get OS version                       */
#define S_ENATRA        52          /* Enable system trace                  */
#define S_DISTRA        53          /* Disable system trace                 */
#define S_DELTSK        54          /* Delete task                          */
#define S_SETATR        55          /* Set task attribute                   */

/*
 *  Use in user or interrupt
*/
#define X_EOIPIC        64          /* Send EOI to PIC                      */
#define X_SETCLK        65          /* Set system clock value               */
#define X_GETCLK        66          /* Get system clock value               */
#define X_ENABLE        67          /* Enable interrupts                    */
#define X_DISABL        68          /* Disable interrupts                   */
#define X_TMRHDL        69          /* Exec kernel timer handler            */
#define X_SETTMO        70          /* Set interrupt timeout                */
#define X_DISINT        71          /* Disable interrupt                    */
#define X_ENAINT        72          /* Enable interrupt                     */
#define X_SETTIM        73          /* Set system clock counter             */
#define X_GETTIM        74          /* Get system clock counter             */

/*
 *  Use only in interrupt
*/
#define I_CHGPRI        80          /* Change priority                      */
#define I_ROTRDQ        81          /* Rotate ready queue                   */
#define I_RELWAI        82          /* Release wait task                    */
#define I_SUSTSK        83          /* Suspend task                         */
#define I_RSMTSK        84          /* Resume task                          */
#define I_WUPTSK        85          /* Wakeup task                          */
#define I_SIGSEM        86          /* Signal semaphore                     */
#define I_SETFLG        87          /* Set event flags                      */
#define I_SNDMSG        88          /* Send msg                             */

#endif
