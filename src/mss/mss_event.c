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
* @file     mss_event.c
* 
* @brief    mcu simple scheduler event module
*
* @version  0.2.1
*
* @author   Leo Hendrawan
*
* @remark   this will be included in compilation only if MSS_TASK_USE_EVENT
*           defined as TRUE
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss.h"
#include "mss_int.h"

#if (MSS_TASK_USE_EVENT == TRUE)

//*****************************************************************************
// Global variables 
//*****************************************************************************


//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_task_event
 *  mss task event bits
 */
static mss_event_t task_event[MSS_NUM_OF_TASKS];

//*****************************************************************************
// Internal function declarations
//*****************************************************************************


//*****************************************************************************
// External functions
//*****************************************************************************

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
void mss_event_init(void)
{
  uint8_t i;

  // initialize the task control blocks
  for(i=0 ; i<MSS_NUM_OF_TASKS ; i++)
  {
    task_event[i] = 0;
  }
}

/**************************************************************************//**
*
* mss_event_set
*
* @brief      set an event to a mss task (will activate the task)
*
* @param[in]  task_id    task id number
* @param[in]  event      event to be set
*
* @return     -
*
******************************************************************************/
void mss_event_set(uint8_t task_id, mss_event_t event)
{
  mss_int_flag_t int_flag;

  // check task id
  MSS_DEBUG_CHECK(task_id < MSS_NUM_OF_TASKS);

  MSS_ENTER_CRITICAL_SECTION(int_flag);
  
  // set task event and activate task
  task_event[task_id] |= event;
  mss_activate_task_int(task_id);
  
  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

/**************************************************************************//**
*
* mss_event_get
*
* @brief      get an event of a running task id, this will automatically clear
*             the event variable
*
* @param[in]  -
*
* @return     event bits of the running task id
*
******************************************************************************/
mss_event_t mss_event_get(void)
{
  mss_event_t ret_event;
  mss_int_flag_t int_flag;

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // copy event to return value and clear event variable
  ret_event = task_event[mss_running_task_id];
  task_event[mss_running_task_id] = 0;

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return ret_event;
}

//*****************************************************************************
// Internal functions
//*****************************************************************************


#endif /* (MSS_TASK_USE_EVENT == TRUE) */
