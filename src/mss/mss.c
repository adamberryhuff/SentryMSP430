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
* @file     mss.c
* 
* @brief    mcu simple scheduler main source file
*
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark 
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "../includes.h"
#include "mss.h"
#include "mss_int.h"

//*****************************************************************************
// Global variables 
//*****************************************************************************

/** mss_running_task_id
 *  task id of currently running mss task
 */
uint8_t mss_running_task_id = MSS_INVALID_TASK_ID;

/** mss_task_reactivated
 *  flag to indicate whether the running/ready mss task is reactivated
 */
mss_task_bits_t mss_task_reactivated = 0;

/** mss_ready_task_bits
 *  flag bits indicating whether the mss task is in ready/idle state
 */
mss_task_bits_t mss_ready_task_bits = MSS_READY_TASK_BITS_INIT;

/** mss_bitpos_to_bit
 *  table for converting bit position to mss_task_bits_t bit value
 */
const mss_task_bits_t mss_bitpos_to_bit[] = MSS_TASK_BIT_POS;

#if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
/** mss_task_preempted
 *  flag to indicate whether a task is currently preempted by another task
 */
mss_task_bits_t mss_task_preempted = 0;
#endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */

//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_task_list_entry_t
 *  list of mss tasks list entry
 */
typedef struct {
  mss_task_t task;
  void* param;
} mss_task_list_entry_t;

/** mss_task_list
 *  list of mss tasks
 */
static mss_task_list_entry_t mss_task_list[MSS_NUM_OF_TASKS] = MSS_TASK_LIST;

//*****************************************************************************
// Internal function declarations
//*****************************************************************************


//*****************************************************************************
// External functions
//*****************************************************************************

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
void mss_init(void)
{
  // initialize mss HAL module
  mss_hal_init();

#if (MSS_TASK_USE_EVENT == TRUE)
  // initialize MSS event
  mss_event_init();
#endif
  
#if (MSS_TASK_USE_TIMER == TRUE)
  // initialize MSS timer
  mss_timer_init();
#endif
}

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
void mss_run(void)
{
  mss_int_flag_t int_flag;

  // enable global interrupt
  MSS_ENABLE_GLOBAL_INTERRUPT();

  while(1)
  {
    // run scheduler until no task is active
    mss_scheduler();

    // mss_scheduler enables interrupt at the end, re-check whether it is
    // really necessary to go sleep
    MSS_ENTER_CRITICAL_SECTION(int_flag);

    if(mss_ready_task_bits == 0)
    {
      // sleep if no task is active
    #if (MSS_TASK_USE_TIMER == TRUE)
      mss_hal_sleep(mss_timer_get_next_tick());
    #else
      mss_hal_sleep(MSS_SLEEP_NO_TIMEOUT);
    #endif /* (MSS_TASK_USE_TIMER == TRUE) */
    }

    MSS_LEAVE_CRITICAL_SECTION(int_flag);
  }
}

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
void mss_scheduler(void)
{
  uint8_t highest_prio;
  mss_int_flag_t int_flag;

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  do
  {
    // get the highest priority task
    highest_prio = mss_get_highest_prio_task(mss_ready_task_bits);

    // set running task id
    mss_running_task_id = highest_prio;

#if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
    // only execute if the task is not being preempted
    // otherwise it is left somewhere beneath the memory stack
    if(mss_task_preempted & mss_bitpos_to_bit[highest_prio])
    {
      // leave the scheduler function
      break;
    }
#endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */

    if(highest_prio != MSS_INVALID_TASK_ID)
    {
      MSS_LEAVE_CRITICAL_SECTION(int_flag);

      // execute task
      mss_task_list[highest_prio].task(mss_task_list[highest_prio].param);

      MSS_ENTER_CRITICAL_SECTION(int_flag);

      // set running task id to none
      mss_running_task_id = MSS_INVALID_TASK_ID;

#if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
      // always clear the preemptive bit after a task has been executed
      mss_task_preempted &= ~mss_bitpos_to_bit[highest_prio];
#endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */

      // check if task reactivated
      if(mss_task_reactivated & mss_bitpos_to_bit[highest_prio])
      {
        // clear flag
        mss_task_reactivated &= ~mss_bitpos_to_bit[highest_prio];
      }
      else
      {
        // clear ready bit of the task
        mss_ready_task_bits &= ~mss_bitpos_to_bit[highest_prio];
      }
    }
  }while(highest_prio != MSS_INVALID_TASK_ID);

  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

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
void mss_activate_task(uint8_t task_id)
{
  mss_int_flag_t int_flag;

  MSS_ENTER_CRITICAL_SECTION(int_flag);
  
  // check task id
  MSS_DEBUG_CHECK(task_id < MSS_NUM_OF_TASKS);
  
  // activate task
  mss_activate_task_int(task_id);
  
  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

/**************************************************************************//**
*
* mss_activate_task_int
*
* @brief      internal MSS function to put task into active state -
*             not reentrant
*
* @param[in]  task_id    task id number
*
* @return     -
*
******************************************************************************/
void mss_activate_task_int(uint8_t task_id)
{
  // check if task to be activated is currently running
  if((task_id == mss_running_task_id)
#if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
      || (mss_task_preempted & mss_bitpos_to_bit[task_id])
#endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */
    )
  {
  	// mark that the task shall be re-executed
    mss_task_reactivated |= mss_bitpos_to_bit[task_id];
  }
  else
  {
  	// mark that the new task shall be ready to be executed
    mss_ready_task_bits |= mss_bitpos_to_bit[task_id];

  #if (MSS_PREEMPTIVE_SCHEDULING == TRUE)
    if(mss_running_task_id != MSS_INVALID_TASK_ID)
    {
      if(task_id < mss_running_task_id)
      {
        // do the preemption, first set the preemption bit
        mss_task_preempted |= mss_bitpos_to_bit[mss_running_task_id];
      }

      // don't call the scheduler directly, let the software interrupt
      // does the job
      mss_hal_trigger_sw_int();
    }

  #endif /* (MSS_PREEMPTIVE_SCHEDULING == TRUE) */
  }
}

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
uint8_t mss_get_running_task_id(void)
{
  return mss_running_task_id;
}

//*****************************************************************************
// Internal functions
//*****************************************************************************
