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
* @file     mss_sema.h
* 
* @brief    mcu simple scheduler semaphore module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_SEMA
*           defined as TRUE
* 
******************************************************************************/

#ifndef _MSS_SEMA_H_
#define _MSS_SEMA_H_

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_Sema_API  MSS Semaphore API
 * @brief     MSS Semaphore module API definitions, data types, and functions
 *            (enabled only if (MSS_TASK_USE_SEMA == TRUE))
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
 * @name MSS Semaphore handle
 * @{
 */

/** mss_sema_t
 *  mss semaphore handle data type
 */
typedef struct mss_sema_tbl_t*   mss_sema_t;

/** MSS_SEMA_INVALID_HDL
 *  invalid mque invalid handle
 */
#define MSS_SEMA_INVALID_HDL      ((mss_sema_t)NULL)

/** @} MSS Semaphore handle */

/**
 * @name MSS Semaphore API Functions
 * @{
 */

/** MSS_SEMA_WAIT
 *  macro function to wait for a semaphore, and blocks if the semaphore is
 *  not available
 */
#define MSS_SEMA_WAIT(hdl, context)           \
        do{while(mss_sema_wait(hdl) == FALSE) \
           MSS_RETURN(context);}while(0)


//*****************************************************************************
// External function declarations
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
mss_sema_t mss_sema_create(uint16_t init_val);

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
bool mss_sema_wait(mss_sema_t hdl);

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
void mss_sema_post(mss_sema_t hdl);

/** @} MSS Semaphore API Functions */

/** @} MSS_Sema_API */

#endif /* _MSS_SEMA_H_*/
