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
* @file     mss.h
* 
* @brief    mcu simple scheduler main header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this is the main header file which is the only header file of MSS
*           which shall be included by the application code
* 
******************************************************************************/

#ifndef _MSS_H_
#define _MSS_H_

/**
 * @mainpage MSS (MCU Simple Scheduler)
 *
 * @author   Leo Hendrawan
 *
 * MSS (MCU Simple Sceheduler) is a light-weight, preemptable, and portable
 * scheduler system offering multi-tasking capability for small 8/16 bit
 * microcontrollers with other basic features such as timers, event flags,
 * message queues, and semaphores.
 * Currently it is available for the following platform:
 *  - MSP430G2452/MSP430G2553 (MSP430 Value Line) on MSP-EXP430G2 Launchpad 
 *    development kit
 *  - MSP430FR5739 on MSP-EXP430FR5739 development kit
 *
 * Join and post questions/feedbacks for MSS at the 
 * <a href="https://groups.google.com/forum/?fromgroups&hl=en#!forum/mss-disc">
 * MSS Discussion Group</a>.
 *
 */

//*****************************************************************************
// Include section
//*****************************************************************************

#include "dev_types.h"
#include "mss_cfg.h"
#include "mss_hal.h"

#if (MSS_TASK_USE_TIMER == TRUE)
#include "mss_timer.h"
#endif

#if (MSS_TASK_USE_EVENT == TRUE)
#include "mss_event.h"
#endif

#if (MSS_TASK_USE_SEMA == TRUE)
#include "mss_sema.h"
#endif

#if (MSS_TASK_USE_MQUE == TRUE)
#include "mss_mque.h"
#endif

#if (MSS_TASK_USE_MEM == TRUE)
#include "mss_mem.h"
#endif

/**
 * @defgroup MSS_API  MSS API
 * @{
 */
/** @} MSS_API */

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_General_API  MSS General API
 * @brief     Core/General MSS API definitions, data types, and functions
 *            (always enabled)
 * @{
 */

//*****************************************************************************
// Global variable declarations 
//*****************************************************************************

//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/**
 * @name MSS Task Context
 * @{
 */

/** mss_task_ctx_t
 *  mss timer task context data type - should be declared as static/global
 *  variable with initial value of MSS_TASK_CTX_STATE_INIT_VAL
 */
typedef uint16_t mss_task_ctx_t;

/** MSS_TASK_CTX_STATE_INIT_VAL
 *  initial value of mss task context
 */
#define MSS_TASK_CTX_STATE_INIT_VAL      (0)

// implementation of mss coroutine in C
// derived from: http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html
// and similar to the implementation of protothreads:
// http://dunkels.com/adam/pt/

/** MSS_BEGIN
 *  continue MSS task from last executed context. This shall be the first
 *  instruction in a MSS task function
 */
#define MSS_BEGIN(ctx)    switch(ctx) { case MSS_TASK_CTX_STATE_INIT_VAL:

/** MSS_RETURN
 *  suspend MSS task (give up CPU intentionally to other task) and save the
 *  task context. This function might be encapsulated by other macro
 *  function such as @ref MSS_TIMER_WAIT_EXPIRES
 */
#define MSS_RETURN(ctx)   do{ctx=__LINE__; return; case __LINE__:;}while(0)

/** MSS_FINISH
 *  finishing mss task context. This shall be the last instruction in a MSS
 *  task function
 */
#define MSS_FINISH()      }

/** @} MSS Task Context */

//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name MSS General API Functions
 * @{
 */

/**************************************************************************//**
*
* mss_init
*
* @brief   initialize mcu simple scheduler - shall be called before running
*          MSS with mss_run
*
* @param   -
*
* @return  -
*
******************************************************************************/
void mss_init(void);

/**************************************************************************//**
*
* mss_run
*
* @brief   run the mss - shall never return
*
* @param   -
*
* @return  -
*
******************************************************************************/
void mss_run(void);

/**************************************************************************//**
*
* mss_activate_task
*
* @brief      put task into active state to be executed by the scheduler
*
* @param[in]  task_id    task id number
*
* @return     -
*
******************************************************************************/
void mss_activate_task(uint8_t task_id);

/**************************************************************************//**
*
* mss_get_running_task_id
*
* @brief      get the ID of the current running task
*
* @param      -
*
* @return     task id of current running task
*
******************************************************************************/
uint8_t mss_get_running_task_id(void);

/** @} MSS General API Functtions */

/** @} MSS_General_API */

struct TaskInstance {
  mss_task_ctx_t State;
  mss_timer_t Timer;
};

#endif /* _MSS_H_*/
