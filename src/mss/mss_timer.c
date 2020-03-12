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
* @file     mss_timer.c
* 
* @brief    mcu simple scheduler timer module
*
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_TIMER
*           defined as TRUE
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss.h"
#include "mss_int.h"

#if (MSS_TASK_USE_TIMER == TRUE)

//*****************************************************************************
// Global variables 
//*****************************************************************************

// mss hardware timer tick counter
mss_timer_tick_t mss_timer_tick_cnt = 0;

//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_timer_tbl_t
 *  mss timer table data type
 */
struct mss_timer_tbl_t {
  void *next;
  mss_timer_tick_t expired_tick;
  mss_timer_tick_t reload_tick;
  uint8_t task_id;
  mss_timer_state_t state;
};

// linked list of active timer
static llist_t active_timer_llist;

// mss timer blocks
static struct mss_timer_tbl_t timer_tbl[MSS_MAX_NUM_OF_TIMER];

// number of used mss timer blocks
static uint8_t num_of_timer = 0;

// MSB mask for knowing in which half the timer tick resides
#define MSB_TMR_MASK  (mss_timer_tick_t) (1UL << ( (sizeof(mss_timer_tick_t)*8) - 1 ))

// state bit mask
#define TIMER_ALL_EXPIRED_MASK   (MSS_TIMER_STATE_EXPIRED_ONE_SHOT | \
		                          MSS_TIMER_STATE_EXPIRED_PERIODIC | \
		                          MSS_TIMER_STATE_OVERFLOW)

#define TIMER_ALL_RUNNING_MASK   (MSS_TIMER_STATE_RUNNING_ONE_SHOT | \
		                          MSS_TIMER_STATE_RUNNING_PERIODIC)

//*****************************************************************************
// Internal function declarations
//*****************************************************************************

static bool timer_start(mss_timer_t hdl, mss_timer_tick_t tick,
		                mss_timer_tick_t reload);
static int8_t timer_cmp(void *a, void *b);

//*****************************************************************************
// External functions
//*****************************************************************************

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
void mss_timer_init(void)
{
  uint8_t i;
  
  // initialize timer blocks
  for(i=0 ; i<MSS_MAX_NUM_OF_TIMER ; i++)
  {
	timer_tbl[i].task_id = MSS_INVALID_TASK_ID;
	timer_tbl[i].state = MSS_TIMER_STATE_IDLE;
	timer_tbl[i].expired_tick = 0;
	timer_tbl[i].reload_tick = 0;
  }
  
  // initialize linked list timer
  active_timer_llist = llist_create();
  MSS_DEBUG_CHECK(active_timer_llist != LLIST_INVALID_HDL);
}

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
mss_timer_t mss_timer_create(uint8_t owner_task_id)
{
  mss_timer_t ret_hdl = MSS_TIMER_INVALID_HDL;

  if(num_of_timer < MSS_MAX_NUM_OF_TIMER)
  {
    // set owner task ID
    timer_tbl[num_of_timer].task_id = owner_task_id;
     
    // return valid handle
    ret_hdl = &timer_tbl[num_of_timer++];
  }

  return ret_hdl;
}

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
mss_timer_tick_t mss_timer_get_tick_cnt(void)
{
  mss_int_flag_t int_flag;
  mss_timer_tick_t cur_tick;

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // this should be an atomic process since timer counter can be wider
  // than CPU register
  cur_tick = mss_timer_tick_cnt;

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return cur_tick;
}

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
bool mss_timer_start(mss_timer_t hdl, mss_timer_tick_t tick)
{
  return (timer_start(hdl, tick, 0));
}

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
		                      mss_timer_tick_t reload)
{
  return (timer_start(hdl, tick, reload));
}

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
void mss_timer_stop(mss_timer_t hdl)
{
  mss_int_flag_t int_flag;

  // check timer handler
  MSS_DEBUG_CHECK(hdl != MSS_TIMER_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  if(hdl->state & TIMER_ALL_RUNNING_MASK)
  {
    // search for the timer and remove it
    llist_remove(active_timer_llist, hdl);

    // set timer state as idle
    hdl->state = MSS_TIMER_STATE_IDLE;
  }

  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

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
mss_timer_state_t mss_timer_get_state(mss_timer_t hdl)
{
  mss_int_flag_t int_flag;
  mss_timer_state_t state;
    
  // check timer handler
  MSS_DEBUG_CHECK(hdl != MSS_TIMER_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);
  
  // return timer state
  state = hdl->state;

  if(state & TIMER_ALL_EXPIRED_MASK)
  {
    if(hdl->reload_tick == 0)
    {
      // set timer state as idle
      hdl->state = MSS_TIMER_STATE_IDLE;
    }
    else
    {
      // set timer state as idle
      hdl->state = MSS_TIMER_STATE_RUNNING_PERIODIC;
    }
  }

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return (state);
}

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
bool mss_timer_check_expired(mss_timer_t hdl)
{
  bool ret = false;

  // check timer state
  if(mss_timer_get_state(hdl) & TIMER_ALL_EXPIRED_MASK)
  {
    ret = true;
  }

  return ret;
}

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
bool mss_timer_tick(void)
{
  // local timer tick
  static mss_timer_tick_t timer_tick_cnt = 0;
  // flag indicating whether the timer tick function is already running
  static bool timer_tick_running = false;
  struct mss_timer_tbl_t *youngest_tmr;
  bool ret = false, loop;
  mss_int_flag_t int_flag;

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  if(timer_tick_running == true)
  {
	// return false since this function is already running and now called
	// in different ISR context
    MSS_LEAVE_CRITICAL_SECTION(int_flag);
    return false;
  }

  // set flag to indicate timer tick is already running
  timer_tick_running = true;

  // copy hardware timer count tick to local timer tick
  timer_tick_cnt = mss_timer_tick_cnt;

  // loop in case hardware timer tick interrupt occurs between
  // long active timer list processing
  do
  {
	// if local timer tick is different than hardware timer tick,
	// it means that the hardware timer has fired while the timer tick
	// function is still processing the active timer list. Therefore
	// the timer tick function shall update the local timer and continue
	// processing the active timer list
	if(timer_tick_cnt != mss_timer_tick_cnt)
	{
      // increment timer tick
      timer_tick_cnt++;
	}

	do
	{
      // reset loop flag
      loop = false;

      // check for expired timer
      youngest_tmr = llist_touch_first(active_timer_llist);
	  if(youngest_tmr != NULL)
	  {
		if(youngest_tmr->expired_tick == timer_tick_cnt)
		{
		  // wake up task
		  mss_activate_task_int(youngest_tmr->task_id);

          // change timer state by shifting left one bit the state variable
          // which will change from running to expired in both one-shot and
          // periodic mode or from expired periodic to overflow
          youngest_tmr->state <<= 1;

          // remove timer object from active timer list
          llist_get_first(active_timer_llist);

          // if a periodic timer, returns to the active timer list
          if(youngest_tmr->reload_tick > 0)
          {
            // update timer tick first
            youngest_tmr->expired_tick = timer_tick_cnt +
                                         youngest_tmr->reload_tick;

            // put the timer into the active timer linked list
            llist_add_first(active_timer_llist, youngest_tmr);

            // sort the active timer list
            llist_sort(active_timer_llist, timer_cmp);
          }

          // return true
          ret = true;

          // do another loop for checking simultaneous timer ticks
          loop = true;

          // enable interrupt in between the process
          MSS_LEAVE_CRITICAL_SECTION(int_flag);
          MSS_ENTER_CRITICAL_SECTION(int_flag);
        }
      }
    }while(loop == true);
  }while(timer_tick_cnt != mss_timer_tick_cnt);


  // reset flag
  timer_tick_running = false;

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return ret;
}

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
mss_timer_tick_t mss_timer_get_next_tick(void)
{
  struct mss_timer_tbl_t *youngest_tmr;
  mss_timer_tick_t ret = MSS_SLEEP_NO_TIMEOUT;
  mss_int_flag_t int_flag;

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // check for expired timer
  youngest_tmr = llist_touch_first(active_timer_llist);
  if(youngest_tmr != NULL)
  {
    ret = youngest_tmr->expired_tick - mss_timer_tick_cnt;
  }

  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return ret;
}

//*****************************************************************************
// Internal functions
//*****************************************************************************

/**************************************************************************//**
*
* timer_start
*
* @brief      start a mss timer
*
* @param[in]  hdl     timer handle
* @param[in]  tick    number of timer ticks to run
*                     (maximum value is ((sizeof(mss_timer_tick_t)/2)-1) )
* @param[in]  reload  number of ticks of reloading/periodic timer. 0 means
*                     that it is a one shot timer (maximum value is 
*                     ((sizeof(mss_timer_tick_t)/2)-1) )
*
* @return     true if success, false if failed
*
******************************************************************************/
static bool timer_start(mss_timer_t hdl, mss_timer_tick_t tick,
		                mss_timer_tick_t reload)
{
  bool ret = false;
  mss_int_flag_t int_flag;

  // check timer handler
  MSS_DEBUG_CHECK(hdl != MSS_TIMER_INVALID_HDL);

  if((!(tick & MSB_TMR_MASK)) && (tick > 0))
  {
    // disable timer interrupt to enable re-setting the timer
    MSS_ENTER_CRITICAL_SECTION(int_flag);

    // set timer to active state and set timer counter
    hdl->expired_tick = mss_timer_tick_cnt + tick;
    hdl->reload_tick = reload;

    if(!(hdl->state & TIMER_ALL_RUNNING_MASK))
    {
      // put the timer into the active timer linked list
      llist_add_first(active_timer_llist, hdl);
    }

    // set new state
    hdl->state = (reload > 0) ? MSS_TIMER_STATE_RUNNING_PERIODIC :
  		                      MSS_TIMER_STATE_RUNNING_ONE_SHOT;

    // sort the active timer list
    llist_sort(active_timer_llist, timer_cmp);

    // return true
    ret = true;

    MSS_LEAVE_CRITICAL_SECTION(int_flag);
  }

  return ret;
}

/**************************************************************************//**
* 
* timer_cmp
* 
* @brief      compare timer function for sorting the active timer list
*
* @param[in]  a   pointer to first active timer
* @param[in]  b   pointer to second active timer
* 
* @return     -1 if (a < b), 0 if (a == b), 1 if (a > b)
* 
******************************************************************************/
static int8_t timer_cmp(void *a, void *b)
{
  struct mss_timer_tbl_t *tmr1, *tmr2;
  int8_t ret = 0;

  // cast pointers
  tmr1 = (struct mss_timer_tbl_t*) a;
  tmr2 = (struct mss_timer_tbl_t*) b;

  if(((tmr1->expired_tick & MSB_TMR_MASK) && (tmr2->expired_tick & MSB_TMR_MASK)) ||
     ((!(tmr1->expired_tick & MSB_TMR_MASK)) && (!(tmr2->expired_tick & MSB_TMR_MASK))))
  {
    // both ticks are in the same half side just do normal compare
    if(tmr1->expired_tick > tmr2->expired_tick)
    {
      ret = 1;
    }
    else
    {
      ret = -1;
    }
  }
  else
  {
	  // see if first timer is in the same half side with the timer tick
	if(((tmr1->expired_tick & MSB_TMR_MASK) && (mss_timer_tick_cnt & MSB_TMR_MASK)) ||
	   ((!(tmr1->expired_tick & MSB_TMR_MASK)) && (!(mss_timer_tick_cnt & MSB_TMR_MASK))))
	{
	  // first timer is less than second timer because it resides in the
	  // same half as the timer tick (the second timer must reside in
      // the other half and therefore bigger than first timer)
	  ret = -1;
	}
	else
	{
	  ret = 1;
	}
  }

  return ret;
}

#endif /* (MSS_TASK_USE_TIMER == TRUE) */
