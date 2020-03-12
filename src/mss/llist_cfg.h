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
* @file     llist_cfg.h
* 
* @brief    linked list library configuration header file
* 
* @version  0.2.1
* 
* @remark   
* 
******************************************************************************/

#ifndef _LLIST_CFG_H_
#define _LLIST_CFG_H_

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss_cfg.h"

//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/** MAX_NUM_OF_LLIST
 *  maximum number of linked list. MSS needs at least:
 *   - one linked list if MSS timer module is activated (MSS_TASK_USE_TIMER==TRUE)
 *   - one for every message queues 
 *   - one for every memory blocks
 *  The application might increase the number of linked list if it wish to
 *  use the linked list (llist) module.
 */
#if (MSS_TASK_USE_TIMER == TRUE)
#define MAX_NUM_OF_LLIST         (1 + MSS_MAX_NUM_OF_MQUE + MSS_MAX_NUM_OF_MEM)
#else
#define MAX_NUM_OF_LLIST         (MSS_MAX_NUM_OF_MQUE + MSS_MAX_NUM_OF_MEM)
#endif

/** LLIST_DEBUG_MODE
 *  activate the debug mode of linked list (llist) module if TRUE. Can be
 *  turned off by setting it to FALSE in order to reduce memory usage.
 */
#define LLIST_DEBUG_MODE         (FALSE)

//*****************************************************************************
// External function declarations
//*****************************************************************************


#endif /* _LLIST_CFG_H_*/
