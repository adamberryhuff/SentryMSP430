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
* @file     mss_sema.c
* 
* @brief    mcu simple scheduler semaphore module
*
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_SEMA
*           defined as TRUE
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss.h"
#include "mss_int.h"

#if (MSS_TASK_USE_SEMA == TRUE)

//*****************************************************************************
// Global variables 
//*****************************************************************************

//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_sema_t
 *  mss semaphore table data type
 */
struct mss_sema_tbl_t{
  uint16_t value;
  mss_task_bits_t waiting_tasks;
  mss_task_bits_t signaling_tasks;
};

// number of created semaphores
static uint8_t num_of_sema = 0;

// semaphore table
static struct mss_sema_tbl_t sema_tbl[MSS_MAX_NUM_OF_SEMA];

//*****************************************************************************
// Internal function declarations
//*****************************************************************************


//*****************************************************************************
// External functions
//*****************************************************************************

/**************************************************************************//**
*
* mss_sema_create
*
* @brief      create a new semaphore
*
* @param[in]  init_val    initial semaphore value
*
* @return     handle to created semaphore, MSS_SEMA_INVALID_HDL if failure
*
******************************************************************************/
mss_sema_t mss_sema_create(uint16_t init_val)
{
  mss_sema_t ret_hdl = MSS_SEMA_INVALID_HDL;

  // check if there is a free semaphore block
  if(num_of_sema < MSS_MAX_NUM_OF_SEMA)
  {
    // set initialization value of semaphore block
	sema_tbl[num_of_sema].value = init_val;
	sema_tbl[num_of_sema].waiting_tasks = 0;
	sema_tbl[num_of_sema].signaling_tasks = 0;

	// return handle and increment number of mque block
	ret_hdl = &sema_tbl[num_of_sema++];
  }

  return ret_hdl;
}

/**************************************************************************//**
*
* mss_sema_wait
*
* @brief      decrement (lock/down) a semaphore. A task can only lock a
*             semaphore once. When semaphore is not available, the task
*             will be put into waiting task list.
*
* @param[in]  hdl        semaphore handle
*
* @return     if true semaphore is successfully decremented, if false semaphore
*             is not available
*
******************************************************************************/
bool mss_sema_wait(mss_sema_t hdl)
{
  bool ret = false;
  uint16_t task_bit;
  mss_int_flag_t int_flag;

  // check task id
  MSS_DEBUG_CHECK(hdl != MSS_SEMA_INVALID_HDL);

  // turn running task id into bit position
  task_bit = mss_bitpos_to_bit[mss_running_task_id];

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // check if semaphore is available
  if(hdl->value > 0)
  {
	// check if calling task hasn't locked the semaphore before
	if(!(hdl->signaling_tasks & task_bit))
	{
      // decrement semaphore value
	  hdl->value--;

      // set the corresponding task bit in the signaling task list
	  hdl->signaling_tasks |= task_bit;

	  // return TRUE as locking semaphore succeeds
	  ret = true;
	}
  }
  else
  {
    // semaphore is not available, put task into waiting list if necessary
    hdl->waiting_tasks |= task_bit;
  }

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return ret;
}

/**************************************************************************//**
*
* mss_sema_post
*
* @brief      increment (unlock/up) a semaphore. The waiting task with the
*             highest priority will be activated upon incrementing the
*             semaphore value
*
* @param[in]  hdl        semaphore handle
*
* @return     -
*
******************************************************************************/
void mss_sema_post(mss_sema_t hdl)
{
  uint16_t task_bit;
  uint8_t waiting_task_id;
  mss_int_flag_t int_flag;

  // check task id
  MSS_DEBUG_CHECK(hdl != MSS_SEMA_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // turn running task id into bit position
  task_bit = mss_bitpos_to_bit[mss_running_task_id];
  
  // check if task has locked the semaphore before
  if(hdl->signaling_tasks & task_bit)
  {
    // increment the semaphore value
    hdl->value++;

    // remove task from the signaling list
    hdl->signaling_tasks &= ~task_bit;

    // check if there is waiting task(s)
    if(hdl->waiting_tasks)
    {
      // search the task with highest priority in the waiting task list
      waiting_task_id = mss_get_highest_prio_task(hdl->waiting_tasks);

      // activate the waiting task with highest priority
      mss_activate_task_int(waiting_task_id);

      // remove task from waiting task list
      hdl->waiting_tasks &= ~mss_bitpos_to_bit[waiting_task_id];
    }
  }

  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

//*****************************************************************************
// Internal functions
//*****************************************************************************

#endif /* (MSS_TASK_USE_SEMA == TRUE) */
