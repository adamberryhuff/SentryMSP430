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
* @file     mss_event.h
* 
* @brief    mcu simple scheduler event module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
*
* @remark   this will be included in compilation only if MSS_TASK_USE_EVENT
*           defined as TRUE
* 
******************************************************************************/

#ifndef _MSS_EVENT_H_
#define _MSS_EVENT_H_

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_Event_API  MSS Event API
 * @brief     MSS Event module API definitions, data types, and functions
 *            (enabled only if (MSS_TASK_USE_EVENT == TRUE))
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


//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name MSS Event API Functions
 * @{
 */

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
void mss_event_set(uint8_t task_id, mss_event_t event);

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
mss_event_t mss_event_get(void);

/**************************************************************************//**
*
* MSS_EVENT_WAIT
*
* @brief      macro (blocking) function which waits until a task get at least
*             one event bit get set
*
* @param[out] event     buffer for storing event bits
* @param[in]  context   mss task context
*
* @return     -
*
******************************************************************************/
#define MSS_EVENT_WAIT(event, context)           \
        do{while((event = mss_event_get()) == 0) \
        	MSS_RETURN(context);}while(0)

/** @} MSS Event API Functions */

/** @} MSS_Event_API */

#endif /* _MSS_EVENT_H_*/
