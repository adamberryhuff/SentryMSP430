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
* @file     mss_mem.c
* 
* @brief    mcu simple scheduler event module
*
* @version  0.2.1
*
* @author   Leo Hendrawan
*
* @remark   this will be included in compilation only if MSS_TASK_USE_MEM
*           defined as TRUE
* 
******************************************************************************/

//*****************************************************************************
// Include section
//*****************************************************************************

#include "mss.h"
#include "mss_int.h"

#if (MSS_TASK_USE_MEM == TRUE)

//*****************************************************************************
// Global variables 
//*****************************************************************************


//*****************************************************************************
// Macros (defines), data types, static variables
//*****************************************************************************

/** mss_mem_tbl_t
 *  mss memory table data type
 */
struct mss_mem_tbl_t {
  llist_t free_blks;
};

// number of created memory block
static uint8_t num_of_mem = 0;

// memory table
static struct mss_mem_tbl_t mem_tbl[MSS_MAX_NUM_OF_MEM];

#define LLIST_HDR_LEN   sizeof(void*)

//*****************************************************************************
// Internal function declarations
//*****************************************************************************


//*****************************************************************************
// External functions
//*****************************************************************************

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
mss_mem_t mss_mem_create(uint8_t block_size, uint8_t num_of_blocks)
{
  mss_mem_t ret_hdl = MSS_MEM_INVALID_HDL;
  uint8_t *data, i;
  uint16_t real_blk_size = block_size + LLIST_HDR_LEN;

  // check if there is a free semaphore block
  if(num_of_mem < MSS_MAX_NUM_OF_MEM)
  {
    // initialize linked list
    mem_tbl[num_of_mem].free_blks = llist_create();
    MSS_DEBUG_CHECK(mem_tbl[num_of_mem].free_blks != LLIST_INVALID_HDL);

    // try to allocate memory dynamically
    data = MSS_MALLOC(real_blk_size * num_of_blocks);
    MSS_DEBUG_CHECK(data != NULL)

    // put the blocks into free block list in the beginning
    for(i=0 ; i<num_of_blocks ; i++)
    {
      // insert block into the list
      llist_add_first(mem_tbl[num_of_mem].free_blks, data);

      // update pointer
      data += real_blk_size;
    }

    // return handle and increment number of mque block
    ret_hdl = &mem_tbl[num_of_mem++];
  }

  return ret_hdl;
}

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
void* mss_mem_alloc(mss_mem_t hdl)
{
  void* ret;
  mss_int_flag_t int_flag;

  // check handle
  MSS_DEBUG_CHECK(hdl != MSS_MEM_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);
  
  // get a memory block from free list
  ret = (((uint8_t*) llist_get_first(hdl->free_blks)) + LLIST_HDR_LEN);
  
  MSS_LEAVE_CRITICAL_SECTION(int_flag);

  return ret;
}

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
void mss_mem_free(mss_mem_t hdl, void* data)
{
  uint8_t* mem_blk;
  mss_int_flag_t int_flag;

  // check handle
  MSS_DEBUG_CHECK(hdl != MSS_MEM_INVALID_HDL);

  MSS_ENTER_CRITICAL_SECTION(int_flag);

  // return a memory block to free list
  mem_blk = ((uint8_t*)data) - LLIST_HDR_LEN;
  llist_add_last(hdl->free_blks, mem_blk);

  MSS_LEAVE_CRITICAL_SECTION(int_flag);
}


//*****************************************************************************
// Internal functions
//*****************************************************************************


#endif /* (MSS_TASK_USE_MEM == TRUE) */
