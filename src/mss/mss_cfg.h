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
* @file     mss_cfg.h
* 
* @brief    mcu simple scheduler configuration header file
* 
* @version  0.2.1
* 
* @remark   
* 
******************************************************************************/

#ifndef _MSS_CFG_H_
#define _MSS_CFG_H_

//*****************************************************************************
// Include section
//*****************************************************************************

// include header file containing declaration of task functions

//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/** MAX_NUM_OF_TASKS
 *  maximum number of MSS tasks. This shall not exceed the number of bits which
 *  the @ref mss_task_bits_t has.
 */
#define MSS_NUM_OF_TASKS                 (1)

/** MSS_TASK_LIST
 *  list of task function and parameter pointers of each MSS task. The task 
 *  function pointer shall be a valid pointer to void (*) (void*) type function
 *  while the parameter pointers can take a NULL pointer. 
 */
#define MSS_TASK_LIST                \
        {                            \
          {ControlTask, NULL},    \
        }

/** MSS_READY_TASK_BITS_INIT
 *  initialization value of MSS task ready bits. Each bit set represent the 
 *  task which is ready to be executed right at the beginning.
 */
#define MSS_READY_TASK_BITS_INIT         (0x0001)

/** MSS_PREEMPTIVE_SCHEDULING
 *  set to TRUE to activate preemptive scheduling, otherwise the scheduler
 *  will work cooperatively.
 */
#define MSS_PREEMPTIVE_SCHEDULING        (FALSE)

/** MSS_TASK_USE_EVENT
 *  set to TRUE to activate the MSS event flag module. If it is not used,
 *  this option can be set as FALSE to save some memory space.
 */
#define MSS_TASK_USE_EVENT               (TRUE)

/** MSS_TASK_USE_TIMER
 *  set to TRUE to activate the MSS timer module. If it is not used,
 *  this option can be set as FALSE to save some memory space.
 */
#define MSS_TASK_USE_TIMER               (TRUE)

/** MSS_TASK_USE_MQUE
 *  set to TRUE to activate the MSS message queue module. If it is not used,
 *  this option can be set as FALSE to save some memory space.
 */
#define MSS_TASK_USE_MQUE                (FALSE)

/** MSS_TASK_USE_SEMA
 *  set to TRUE to activate the MSS semaphore module. If it is not used,
 *  this option can be set as FALSE to save some memory space.
 */
#define MSS_TASK_USE_SEMA                (FALSE)

/** MSS_TASK_USE_MEM
 *  set to TRUE to activate the MSS memory block. If it is not used,
 *  this option can be set as FALSE to save some memory space.
 */
#define MSS_TASK_USE_MEM                 (FALSE)

/** MSS_MAX_NUM_OF_TIMER
 *  maximum number of timer used in the MSS application. 
 *  If @ref MSS_TASK_USE_TIMER is set as FALSE, this value will be 
 *  automativally set to zero
 */
#if (MSS_TASK_USE_TIMER == TRUE)
  #define MSS_MAX_NUM_OF_TIMER           (1)  
#else
  #define MSS_MAX_NUM_OF_TIMER           (0)
#endif

#if (MSS_TASK_USE_TIMER == TRUE)
/** mss_timer_tick_t
 *  mss timer tick data type - can be changed according to the application
 *  however notice that a timer can be only started with maximum tick value
 *  of half of the data type (e.g. 32767 for uint16_t, or
 *  2147483647 for uint32_t)
 */
typedef uint16_t mss_timer_tick_t;
#endif

#if (MSS_TASK_USE_EVENT == TRUE)
/** mss_event_t
 *  mss event data type - can be changed if necessary
 */
typedef uint8_t  mss_event_t;
#endif

/** MSS_MAX_NUM_OF_MQUE
 *  maximum number of message queues used in the MSS application. 
 *  If @ref MSS_TASK_USE_MQUE is set as FALSE, this value will be 
 *  automativally set to zero
 */
#if (MSS_TASK_USE_MQUE == TRUE)
  #define MSS_MAX_NUM_OF_MQUE            (1)
#else
  #define MSS_MAX_NUM_OF_MQUE            (0)
#endif

/** MSS_MAX_NUM_OF_SEMA
 *  maximum number of semaphores used in the MSS application. 
 *  If @ref MSS_TASK_USE_SEMA is set as FALSE, this value will be 
 *  automativally set to zero
 */
#if (MSS_TASK_USE_SEMA == TRUE)
  #define MSS_MAX_NUM_OF_SEMA            (1)
#else
  #define MSS_MAX_NUM_OF_SEMA            (0)
#endif

/** MSS_MAX_NUM_OF_MEM
 *  maximum number of memory blocks used in the MSS application. 
 *  If @ref MSS_TASK_USE_MEM is set as FALSE, this value will be 
 *  automativally set to zero
 */
#if (MSS_TASK_USE_MEM == TRUE)
  #define MSS_MAX_NUM_OF_MEM             (1)
#else
  #define MSS_MAX_NUM_OF_MEM             (0)
#endif

/** MSS_DEBUG_MODE
 *  set to TRUE to activate MSS debug mode. If not used, can be set to FALSE
 *  in order to save some memory space
 */
#define MSS_DEBUG_MODE                   (FALSE)

//*****************************************************************************
// External function declarations
//*****************************************************************************

#endif /* _MSS_CFG_H_*/
