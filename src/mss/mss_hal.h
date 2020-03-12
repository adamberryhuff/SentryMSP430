/******************************************************************************
* Copyright (c) 2012-2013, Leo Hendrawan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright
*      notice, this list of conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright
*      notice, this list of conditions and the following disclaimer in the
*      documentation and/or other materials provided with the distribution.
*    * Neither the name of the MSS PROJECT nor the names of its contributors
*      may be used to endorse or promote products derived from this software
*      without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE MSS PROJECT OR ITS
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
******************************************************************************/

/**************************************************************************//**
* 
* @file     mss_hal.h
* 
* @brief    MSS device specific internal module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   target device: MSP430G2452 & MSP430G2553 on MSP-EXP430G2 Launchpad
* 
******************************************************************************/

#ifndef _MSS_HAL_H_
#define _MSS_HAL_H_

//*****************************************************************************
// Include section
//*****************************************************************************


//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/** mss_int_flag_t
 *  interrupt flag buffer data type - should be able to accommodate the global
 *  maskable interrupt flags
 */
typedef uint16_t mss_int_flag_t;

/** MSS_ENTER_CRITICAL_SECTION
 *  macro function for entering critical section (basically saving the
 *  current global maskable interrupt flag, and then disabling it)
 */
#define MSS_ENTER_CRITICAL_SECTION(int_flag)    do {                  \
                               int_flag = __get_SR_register() & GIE;  \
                               __bic_SR_register(GIE);                \
                               } while(0)

/** MSS_LEAVE_CRITICAL_SECTION
 *  macro function for leaving critical section (restoring the global
 *  maskable interrupt flag from the last @ref MSS_ENTER_CRITICAL_SECTION
 */
#define MSS_LEAVE_CRITICAL_SECTION(int_flag)    do {                  \
                               __bis_SR_register(int_flag);           \
                               }while(0)

/** MSS_MALLOC
 *  macro function for dynamic memory allocation (usually malloc() from
 *  stdlib.h can be used)
 */
#define MSS_MALLOC(x)                   malloc(x)

/** MSS_FREE
 *  macro function for dynamic memory deallocation (usually free() from
 *  stdlib.h can be used)
 */
#define MSS_FREE(x)                     free(x)

#if (MSS_TASK_USE_TIMER == TRUE)
/** MSS_TIMER_TICK_MS
 *  time for one MSS timer tick in milliseconds - depending on the
 *  implementation in mss_hal.c
 */
#define MSS_TIMER_TICK_MS              (1)
#endif

//*****************************************************************************
// External function declarations
//*****************************************************************************

#endif /* _MSS_HAL_H_*/
