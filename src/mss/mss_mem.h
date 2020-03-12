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
* @file     mss_mem.h
* 
* @brief    mcu simple scheduler memory module header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   this will be included in compilation only if MSS_TASK_USE_MEM
*           defined as TRUE
* 
******************************************************************************/

#ifndef _MSS_MEM_H_
#define _MSS_MEM_H_

/**
 * @ingroup   MSS_API
 * @defgroup  MSS_Mem_API  MSS Memory API
 * @brief     MSS Memory module API definitions, data types, and functions
 *            (enabled only if (MSS_TASK_USE_MEM == TRUE))
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
 * @name MSS Memory Handles
 * @{
 */

/** mss_mem_t
 *  mss memory handle data type
 */
typedef struct mss_mem_tbl_t*  mss_mem_t;

/** MSS_MEM_INVALID_HDL
 *  invalid mque invalid handle
 */
#define MSS_MEM_INVALID_HDL      ((mss_mem_t)NULL)

/** @} MSS Memory Handles */

//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name MSS Memory API Functions
 * @{
 */

/**************************************************************************//**
*
* mss_mem_create
*
* @brief      create memory block list, once created can not be deleted
*
* @param[in]  block_size      size of a single memory block in bytes
* @param[in]  num_of_blocks   number of memory blocks to be created
*
* @return     MSS_MEM_INVALID_HDL - failed to create memory blocks
*             other - valid mss memory blocks handle
*
* @remark     when creating memory block list to be used together with
*             message queue (mque), the block size shall be the sum of
*             message queue's message size plus the size of a pointer variable
*
******************************************************************************/
mss_mem_t mss_mem_create(uint8_t block_size, uint8_t num_of_blocks);

/**************************************************************************//**
*
* mss_mem_alloc
*
* @brief      allocate a memory block
*
* @param[in]  hdl      memory blocks handle
*
* @return     NULL if failed, otherwise pointer to the allocated memory block
*
******************************************************************************/
void* mss_mem_alloc(mss_mem_t hdl);

/**************************************************************************//**
*
* mss_mem_free
*
* @brief      free a memory block (return to free list)
*
* @param[in]  hdl      memory blocks handle
* @param[in]  data     pointer of the data to be freed
*
* @return     -
*
******************************************************************************/
void mss_mem_free(mss_mem_t hdl, void* data);

/** @} MSS Memory API Functions */

/** @} MSS_Mem_API */

#endif /* _MSS_MEM_H_*/
