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
* @file     mss_mque.h
* 
* @brief    mcu simple scheduler message queue (mque) module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_MQUE
*           defined as TRUE
* 
******************************************************************************/

#ifndef _MSS_MQUE_H_
#define _MSS_MQUE_H_

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_Mque_API  MSS Message Queue API
 * @brief     MSS Message queue (mque) module API definitions, data types,
 *            and functions (enabled only if (MSS_TASK_USE_MQUE == TRUE))
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
 * @name MSS Message Queue handle
 * @{
 */

/** mss_mque_tbl_t
 *  mss mque table item data type
 */
typedef struct mss_mque_tbl_t*      mss_mque_t;

/** MSS_MQUE_INVALID_HDL
 *  invalid mque invalid handle
 */
#define MSS_MQUE_INVALID_HDL    ((mss_mque_t)NULL)

/** @} MSS Message Queue handle */

/**
 * @name MSS Message Queue data type
 * @{
 */

/** mss_mque_msg_t
 *  mss mque message data type
 */
typedef struct {
  /** pointer variable for putting message into linked list */
  void *next;
  /** message data array */
  uint8_t data [1];
}mss_mque_msg_t;

/** @} MSS Message Queue data type */


//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name MSS Message Queue API Functions
 * @{
 */

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
mss_mque_t mss_mque_create(uint8_t owner_task_id);

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
void mss_mque_send(mss_mque_t hdl, mss_mque_msg_t* msg);

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
mss_mque_msg_t* mss_mque_read(mss_mque_t hdl);

/**************************************************************************//**
*
* MSS_MQUE_WAIT_MSG
*
* @brief      macro (blocking) function to wait until a message is received
*             in the message queue of an active mss task
*
* @param[in]  hdl     handle of the mque block
* @param[out] msg     pointer to the received message in mque
* @param[in]  context   mss task context
*
* @return     -
*
******************************************************************************/
#define MSS_MQUE_WAIT_MSG(hdl, msg, context)         \
        do{while((msg = mss_mque_read(hdl)) == NULL) \
           MSS_RETURN(context);}while(0)

/** @} MSS Message Queue API Functions */

/** @} MSS_Mque_API */

#endif /* _MSS_MQUE_H_*/
