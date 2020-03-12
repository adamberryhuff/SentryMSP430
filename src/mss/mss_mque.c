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
* @file     mss_mque.c
* 
* @brief    mcu simple scheduler message queue (mque) module
*
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_MQUE
*           defined as TRUE
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss.h"
#include "mss_int.h"

#if (MSS_TASK_USE_MQUE == TRUE)

//*****************************************************************************
// Global variables 
//*****************************************************************************

//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_mque_tbl_t
 *  mss mque block data type
 */
struct mss_mque_tbl_t {
  llist_t msg_list;
  uint8_t owner_task;
};

// number of created message queues
static uint8_t num_of_mque = 0;

// message queue blocks
static struct mss_mque_tbl_t mque_tbl[MSS_MAX_NUM_OF_MQUE];

//*****************************************************************************
// Internal function declarations
//*****************************************************************************


//*****************************************************************************
// External functions
//*****************************************************************************

/**************************************************************************//**
*
* mss_mque_create
*
* @brief      create a new message queue
*
* @param      owner_task_id    task id of mque's owner task
*
* @return     handle to created mque, MSS_MQUE_INVALID_HDL if failure
*
******************************************************************************/
mss_mque_t mss_mque_create(uint8_t owner_task_id)
{
  mss_mque_t ret_hdl = MSS_MQUE_INVALID_HDL;

  // check if there is a free mque block
  if(num_of_mque < MSS_MAX_NUM_OF_MQUE)
  {
    // initializes message queue list
	  mque_tbl[num_of_mque].msg_list = llist_create();
	  MSS_DEBUG_CHECK(mque_tbl[num_of_mque].msg_list != LLIST_INVALID_HDL);

    // set owner task
    mque_tbl[num_of_mque].owner_task = owner_task_id;

    // return valid handle and increment number of mque block
    ret_hdl = &mque_tbl[num_of_mque++];
  }

  return ret_hdl;
}

/**************************************************************************//**
*
* mss_mque_send
*
* @brief      send a message to a message queue (mque). The receiving task
*             will be automatically activated.
*
* @param[in]  hdl   handle of the mque
* @param[in]  msg   pointer to the mque message to be sent
*
* @return     -
*
******************************************************************************/
void mss_mque_send(mss_mque_t hdl, mss_mque_msg_t* msg)
{
  mss_int_flag_t int_flag;

  // check task id
  MSS_DEBUG_CHECK(hdl != MSS_MQUE_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // insert data to mque linked read list
  llist_add_last(hdl->msg_list, msg);

  // wake up owner task
  mss_activate_task_int(hdl->owner_task);

  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}

/**************************************************************************//**
*
* mss_mque_read
*
* @brief      receive message from the message queue (mque)
*
* @param[in]  hdl     handle of the mque
*
* @return     pointer to first message in message queue (NULL if no message
*             available)
*
******************************************************************************/
mss_mque_msg_t* mss_mque_read(mss_mque_t hdl)
{
  // check task id
  MSS_DEBUG_CHECK(hdl != MSS_MQUE_INVALID_HDL);

  return((mss_mque_msg_t*)llist_get_first(hdl->msg_list));
}

//*****************************************************************************
// Internal functions
//*****************************************************************************

#endif /* (MSS_TASK_USE_MQUE == TRUE) */
