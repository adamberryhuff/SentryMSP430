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
* @file     mss_int.h
* 
* @brief    mcu simple scheduler internal module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   
* 
******************************************************************************/

#ifndef _MSS_INT_H_
#define _MSS_INT_H_

//*****************************************************************************
// Include section
//*****************************************************************************

#include "llist.h"
#include "mss_hal_int.h"

//*****************************************************************************
// Special data types
//*****************************************************************************

/** mss_task_bits_t
 *  task bits data type - this data type is mainly used to represent a task
 *  (state), and therefore the @ref MSS_NUM_OF_TASKS can not exceed the number
 *  of bits in this data type
 */
#if (MSS_NUM_OF_TASKS <= 8)
typedef uint8_t mss_task_bits_t;
#elif (MSS_NUM_OF_TASKS <= 16)
typedef uint16_t mss_task_bits_t;
#elif (MSS_NUM_OF_TASKS <= 32)
typedef uint32_t mss_task_bits_t;
#else
#error maximum number of tasks shall be only 32!
#endif

/** MSS_TASK_BIT_POS
 *  array of constant containing task bit position values - used a lot for
 *  setting or resetting a bit in the @ref mss_task_bits_t data type
 */
#if (MSS_NUM_OF_TASKS <= 8)
#define MSS_TASK_BIT_POS                          \
{                                                 \
  0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80  \
}
#elif (MSS_NUM_OF_TASKS <= 16)
#define MSS_TASK_BIT_POS           \
{                                  \
  0x0001, 0x0002, 0x0004, 0x0008,  \
  0x0010, 0x0020, 0x0040, 0x0080,  \
  0x0100, 0x0200, 0x0400, 0x0800,  \
  0x1000, 0x2000, 0x4000, 0x8000   \
}
#elif (MSS_NUM_OF_TASKS <= 32)
#define MSS_TASK_BIT_POS                          \
{                                                 \
  0x00000001, 0x00000002, 0x00000004, 0x00000008, \
  0x00000010, 0x00000020, 0x00000040, 0x00000080, \
  0x00000100, 0x00000200, 0x00000400, 0x00000800, \
  0x00001000, 0x00002000, 0x00004000, 0x00008000, \
  0x00010000, 0x00020000, 0x00040000, 0x00080000, \
  0x00100000, 0x00200000, 0x00400000, 0x00800000, \
  0x01000000, 0x02000000, 0x04000000, 0x08000000, \
  0x10000000, 0x20000000, 0x40000000, 0x80000000  \
}
#endif

//*****************************************************************************
// Global variable declarations 
//*****************************************************************************

/** mss_running_task_id
 *  task id number of mss task which is currently being executed
 */
extern uint8_t mss_running_task_id;

/** mss_running_task_reactivated
 *  flag indicating whether the mss task which is currently being executed needs
 *  to be kept in active state after execution
 */
extern bool mss_running_task_reactivated;

/** mss_ready_task_bits
 *  flag bits indicating whether the mss task is in ready/suspend state
 */
extern mss_task_bits_t mss_ready_task_bits;

/** mss_bitpos_to_bit
 *  table for converting bit position to mss_task_bits_t bit value
 */
extern const mss_task_bits_t mss_bitpos_to_bit[];

#if (MSS_TASK_USE_TIMER == TRUE)
/** mss_timer_tick_cnt
 *  mss hardware timer tick counter
 */
extern mss_timer_tick_t mss_timer_tick_cnt;

#endif /* (MSS_TASK_USE_TIMER == TRUE) */

//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/** mss_task_t
 *  mss task data type
 */ 
typedef void (*mss_task_t) (void*);

/** mss_tcb_t
 *  mss task control block data type
 */
typedef struct _mss_tcb_t  mss_tcb_t;

/** MSS_INVALID_TASK_ID
 *  invalid task id
 */
#define MSS_INVALID_TASK_ID            (0xFF)

/** MSS_SLEEP_NO_TIMEOUT
 *  sleep without timeout
 */
#define MSS_SLEEP_NO_TIMEOUT           ((mss_timer_tick_t)-1)

/** MSS_ASSERT
 *  mss assert function
 */
#if (MSS_DEBUG_MODE == TRUE)
#define MSS_DEBUG_CHECK(cond)            {if(!(cond)) while(1);}
#else
#define MSS_DEBUG_CHECK(cond)
#endif /* (MSS_DEBUG_MODE == TRUE) */

//*****************************************************************************
// External function declarations
//*****************************************************************************

/**************************************************************************//**
*
* mss_scheduler
*
* @brief   mss scheduler function
*
* @param   -
*
* @return  -
*
******************************************************************************/
void mss_scheduler(void);

#if (MSS_TASK_USE_TIMER == TRUE)
/**************************************************************************//**
*
* mss_timer_init
*
* @brief      initialize mss timer module
*
* @param      -
*
* @return     -
*
******************************************************************************/
void mss_timer_init(void);

/**************************************************************************//**
*
* mss_timer_tick
*
* @brief      mss timer tick function - this function is to be called
*             periodically by timer ISR function in mss_hal.c
*
* @param      -
*
* @return     if true, there is a task activated, if false no task is activated
*
******************************************************************************/
bool mss_timer_tick(void);

/**************************************************************************//**
*
* mss_timer_get_next_tick
*
* @brief      get the next timer tick
*
* @param      -
*
* @return     MSS_SLEEP_NO_TIMEOUT if no active timer exists, otherwise
*             the number of timer ticks until the next timer expires
*
******************************************************************************/
mss_timer_tick_t mss_timer_get_next_tick(void);
#endif /* (MSS_TASK_USE_TIMER == TRUE) */

#if (MSS_TASK_USE_EVENT == TRUE)
/**************************************************************************//**
*
* mss_event_init
*
* @brief      initialize event module
*
* @param      -
*
* @return     -
*
******************************************************************************/
void mss_event_init(void);
#endif /* (MSS_TASK_USE_EVENT == TRUE) */

/**************************************************************************//**
*
* mss_get_highest_prio_task
*
* @brief      get the highest priority task (LSB bit position of the
*             task ready bits)
*
* @param[in]  value    the task bits input
*
* @return     LSB bit position or MSS_INVALID_TASK_ID if not bit is set
*
* @remark     the reason that this function is placed under the hardware
*             dependent (hal) code is to enable optimization since this
*             function is used quite often and can affect the scheduler
*             performance
*
******************************************************************************/
uint8_t mss_get_highest_prio_task(mss_task_bits_t ready_bits);

/**************************************************************************//**
*
* mss_activate_task_int
*
* @brief      internal MSS function to put task into active state - not reentrant
*
* @param[in]  task_id    task id number
*
* @return     -
*
******************************************************************************/
void mss_activate_task_int(uint8_t task_id);

/**************************************************************************//**
*
* mss_hal_init
*
* @brief      initialize mss HAL unit, shall do the following tasks:
*             - setup CPU frequency (if necessary)
*             - if @ref MSS_TASK_USE_TIMER set to TRUE, this function shall
*               setup timer interrupt which shall increment the timer counter
*               mss_timer_tick_cnt and call @ref mss_timer_tick() function 
*               periodically
*             - if @ref MSS_PREEMPTIVE_SCHEDULING is set to TRUE, setup the
*               software interrupt or hardware interrupt which is used to call
*               mss_scheduler during preemption
*
* @param      -
*
* @return     -
*
* @remark     the global interrupt shall not be enabled here
*
******************************************************************************/
void mss_hal_init(void);

/**************************************************************************//**
*
* mss_hal_sleep
*
* @brief      do sleep if no task is active - should return/wake up upon
*             interrupt events
*
* @param[in]  sleep_timeout   sleep timeout in ticks (if MSS_SLEEP_NO_TIMEOUT,
*                             no sleep timeout) - can be used as delay to call
*                             @ref mss_timer_tick() until the next timer tick
*                             of the youngest timer when the device is
*                             sleeping
*
* @return     -
*
* @remark     this function shall enable interrupt to wake up the CPU from
*             sleep mode and then disable the interrupt before leaving
*             the function
*
******************************************************************************/
void mss_hal_sleep(mss_timer_tick_t sleep_timeout);

#if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
/**************************************************************************//**
*
* mss_hal_trigger_sw_int
*
* @brief      trigger software interrupt - the triggered software interrupt
*             shall call @ref mss_scheduler to do the preemption
*
* @param      -
*
* @return     -
*
******************************************************************************/
void mss_hal_trigger_sw_int(void);
#endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */

#endif /* _MSS_INT_H_*/
