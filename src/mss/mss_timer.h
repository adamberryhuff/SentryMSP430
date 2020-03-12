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
* @file     mss_timer.h
* 
* @brief    mcu simple scheduler timer module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_TIMER
*           defined as TRUE
* 
******************************************************************************/

#ifndef _MSS_TIMER_H_
#define _MSS_TIMER_H_

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_Timer_API  MSS Timer API
 * @brief     MSS Timer module API definitions, data types, and functions
 *            (enabled only if (MSS_TASK_USE_TIMER == TRUE))
 * @{
 */

//*****************************************************************************
// Include section
//*****************************************************************************


//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types
//*****************************************************************************

/**
 * @name MSS Timer Handles
 * @{
 */

/** mss_timer_t
 *  mss timer handle data type
 */
typedef struct mss_timer_tbl_t*   mss_timer_t;

/** MSS_TIMER_INVALID_HDL
 *  invalid timer handle
 */
#define MSS_TIMER_INVALID_HDL        (NULL)

/** @} MSS Timer Handles */

/**
 * @name MSS Timer States
 * @{
 */

/** mss_timer_state_t
 *  mss timer state data type
 */
typedef uint8_t mss_timer_state_t;

/** MSS_TIMER_STATE_IDLE
 *  mss timer is inactive idle. calling the timer with
 *  mss_set_timer() will change the state to MSS_TIMER_RUNNING
 */
#define MSS_TIMER_STATE_IDLE                       (0x00)

/** MSS_TIMER_STATE_RUNNING_ONE_SHOT
 *  mss timer is active and running in one shot mode. After the timer counter
 *  is reached, the state will turn to MSS_TIMER_STATE_EXPIRED_ONE_SHOT
 */
#define MSS_TIMER_STATE_RUNNING_ONE_SHOT           (0x01)

/** MSS_TIMER_STATE_EXPIRED_ONE_SHOT
 *  mss timer is expired in one shot mode (was previously running). Reading
 *  the timer state in MSS_TIMER_STATE_EXPIRED_ONE_SHOT will reset the state
 *  back to MSS_TIMER_STATE_IDLE
 */
#define MSS_TIMER_STATE_EXPIRED_ONE_SHOT           (0x02)

/** MSS_TIMER_STATE_RUNNING_PERIODIC
 *  mss timer is active and running in periodic mode. After the timer
 *  counter elapsed the periodic tick, the state will turn to
 *  MSS_TIMER_STATE_EXPIRED_PERIODIC while the timer keeps running
 */
#define MSS_TIMER_STATE_RUNNING_PERIODIC           (0x10)

/** MSS_TIMER_STATE_EXPIRED_PERIODIC
 *  mss timer is expired in periodic mode while the timer is still running.
 *  Reading the timer state in MSS_TIMER_STATE_EXPIRED_PERIODIC will
 *  change the state back to MSS_TIMER_STATE_RUNNING_PERIODIC
 */
#define MSS_TIMER_STATE_EXPIRED_PERIODIC           (0x20)

/** MSS_TIMER_STATE_OVERFLOW
 *  this is a special timer state when the timer runs in periodic timer mode.
 *  This state indicates that a timer has expired previously (state reached
 *  MSS_TIMER_STATE_EXPIRED) and the next periodic event has been reached
 *  without the last MSS_TIMER_STATE_EXPIRED being read by the owner task
 */
#define MSS_TIMER_STATE_OVERFLOW                   (0x40)

/** @} MSS Timer States */

//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name MSS Timer API Functions
 * @{
 */

/**************************************************************************//**
*
* mss_timer_create
*
* @brief      create a mss timer
*
* @param[in]  owner_task_id    task id of timer's owner task
*
* @return     MSS_TIMER_INVALID_HDL - failed to create a timer
*             other - valid mss timer handle
*
******************************************************************************/
mss_timer_t mss_timer_create(uint8_t owner_task_id);

/**************************************************************************//**
*
* mss_timer_get_tick_cnt
*
* @brief      get current timer tick counter
*
* @param      -
*
* @return     current timer tick counter
*
******************************************************************************/
mss_timer_tick_t mss_timer_get_tick_cnt(void);

/**************************************************************************//**
*
* mss_timer_start
*
* @brief      start a mss timer to run in one shot mode for a given tick 
*             period. The owner task will be activated when the timer expires
*             (from MSS_TIMER_STATE_RUNNING_ONE_SHOT to 
*             MSS_TIMER_STATE_EXPIRED_ONE_SHOT)
*
* @param[in]  hdl     timer handle
* @param[in]  tick    number of timer ticks to run (maximum value is half of 
*                     maximum value of the data type size 
*                     ((sizeof(mss_timer_tick_t)/2)-1) )
*
* @return     true if success, false if failed
*
******************************************************************************/
bool mss_timer_start(mss_timer_t hdl, mss_timer_tick_t tick);

/**************************************************************************//**
*
* mss_timer_periodic_start
*
* @brief      start a mss timer to run in periodic mode. The owner task will 
*             be activated when the timer expires (from 
*             MSS_TIMER_STATE_RUNNING_PERIODIC to 
*             MSS_TIMER_STATE_EXPIRED_PERIODIC)
*
* @param[in]  hdl     timer handle
* @param[in]  tick    number of timer ticks for the first timer event
*                     (maximum value is half of maximum value of the data type
*                     size ((sizeof(mss_timer_tick_t)/2)-1) )
* @param[in]  reload  number of ticks of reloading/periodic timer
*                     (maximum value is half of maximum value of the data type
*                     size ((sizeof(mss_timer_tick_t)/2)-1) )
*
* @return     true if success, false if failed
*
******************************************************************************/
bool mss_timer_periodic_start(mss_timer_t hdl, mss_timer_tick_t tick,
		                      mss_timer_tick_t reload);

/**************************************************************************//**
*
* mss_timer_stop
*
* @brief      stop a running mss timer (timer state is reset back to 
*             MSS_TIMER_STATE_IDLE)
*
* @param[in]  hdl     timer handle
*
* @return     true if success, false if failed
*
******************************************************************************/
void mss_timer_stop(mss_timer_t hdl);

/**************************************************************************//**
*
* mss_timer_get_state
*
* @brief      get the state of a mss timer. will automatically reset the timer
*             state to MSS_TIMER_STATE_IDLE if the current state is
*             MSS_TIMER_STATE_EXPIRED_ONE_SHOT or 
*             MSS_TIMER_STATE_RUNNING_PERIODIC if current state is 
*             MSS_TIMER_STATE_EXPIRED_PERIODIC
*
* @param[in]  hdl     timer handle
*
* @return     true if success, false if failed
*
******************************************************************************/
mss_timer_state_t mss_timer_get_state(mss_timer_t hdl);

/**************************************************************************//**
*
* mss_timer_check_expired
*
* @brief      check whether the timer has expired (timer state is either
*             MSS_TIMER_STATE_EXPIRED_ONE_SHOT in one shot mode or 
*             MSS_TIMER_STATE_EXPIRED_PERIODC in periodic mode)
*
* @param[in]  hdl     timer handle
*
* @return     true if timer has elapsed, false if not
*
******************************************************************************/
bool mss_timer_check_expired(mss_timer_t hdl);

/**************************************************************************//**
*
* MSS_TIMER_MS_TO_TICKS
*
* @brief      macro function to convert time in milliseconds to mss timer ticks
*
* @param[in]  ms     time in milliseconds
*
* @return     number of corresponding timer ticks
*
******************************************************************************/
#define MSS_TIMER_MS_TO_TICKS(ms)    (ms/MSS_TIMER_TICK_MS)

/**************************************************************************//**
*
* MSS_TIMER_WAIT_EXPIRES
*
* @brief      macro (blocking) function to wait until a timer expires.
*             the mss task will be suspended during waiting the timer
*             expiration
*
* @param[in]  hdl       timer handle
* @param[in]  context   mss task context
*
* @return     number of corresponding timer ticks
*
******************************************************************************/
#define MSS_TIMER_WAIT_EXPIRES(hdl, context)           \
        do{while(mss_timer_check_expired(hdl) != true) \
           MSS_RETURN(context);}while(0)

/**************************************************************************//**
*
* MSS_DELAY_MS
*
* @brief      macro function to simulate delay in miliseconds. the mss task
*             will be suspended during waiting the timer expiration
*
* @param[in]  hdl       timer handle
* @param[in]  ms        delay time in milliseconds
* @param[in]  context   mss task context
*
* @return     number of corresponding timer ticks
*
******************************************************************************/
#define MSS_TIMER_DELAY_MS(hdl, ms, context)                 \
        do{mss_timer_start(hdl, MSS_TIMER_MS_TO_TICKS(ms));  \
           while(mss_timer_check_expired(hdl) != true)       \
           MSS_RETURN(context);}while(0)


/** @} MSS Timer API Functions */

/** @} MSS_Timer_API */

#endif /* _MSS_TIMER_H_*/
