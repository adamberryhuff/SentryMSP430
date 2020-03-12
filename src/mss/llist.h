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
* @file     llist.h
* 
* @brief    single linked list library header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
* 
* @remark   
* 
******************************************************************************/

#ifndef _LLIST_H_
#define _LLIST_H_

//*****************************************************************************
// Include section
//*****************************************************************************

#include "llist_cfg.h"

/**
 * @defgroup  LLIST_API  LLIST API
 * @brief     LLIST (single linked list module) API definitions, data types,
 *            and functions
 * @{
 */


//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/**
 * @name Llist Handles
 * @{
 */

/** llist_t
 *  single linked list handle data type
 */
typedef struct llist_tbl_t* llist_t;

/** LLIST_INVALID_HDL
 *  invalid linked list handle
 */
#define LLIST_INVALID_HDL            (NULL)

/** @} Llist Handles */

/** LLIST_ASSERT
 *  llist module assert function
 */
#if (LLIST_DEBUG_MODE == TRUE)
#define LLIST_ASSERT(cond)                    {if(!(cond)) while(1);}
#else
#define LLIST_ASSERT(cond)
#endif /* (LLIST_DEBUG_MODE == TRUE) */

//*****************************************************************************
// External function declarations
//*****************************************************************************

/**
 * @name LLIST API Functions
 * @{
 */

/**************************************************************************//**
*
* llist_create
*
* @brief      create a (single) linked list
*
* @param      -
*
* @return     LLIST_INVALID_HDL - failed to create linked list
*             other - handle of the created single linked list
*
******************************************************************************/
llist_t llist_create(void);

/**************************************************************************//**
*
* llist_add_first
*
* @brief      add an object into the first position of a linked list
*
* @param[in]  hdl      handle of the linked list
*
* @param[in]  object   pointer object to be added into the linked list
*
* @return     -
*
* @remark     the object parameter shall be pointer to a structure in which
*             the first structure list member is a void pointer type variable
*             which will be used by the llist module to link the objects in
*             the linked list
*
******************************************************************************/
void llist_add_first(llist_t hdl, void* object);

/**************************************************************************//**
*
* llist_add_last
*
* @brief      add an object into the last position of a linked list
*
* @param[in]  hdl      handle of the linked list
*
* @param[in]  object   pointer object to be added into the linked list
*
* @return     -
*
* @remark     the object parameter shall be pointer to a structure in which
*             the first structure list member is a void pointer type variable
*             which will be used by the llist module to link the objects in
*             the linked list
*
******************************************************************************/
void llist_add_last(llist_t hdl, void* object);

/**************************************************************************//**
*
* llist_touch_first
*
* @brief      get pointer to the first object in the linked list without
*             removing it out of the linked list
*
* @param[in]  hdl      handle of the linked list
*
* @return     pointer to the first object in the linked list, NULL if no
*             object is found in the list
*
******************************************************************************/
void* llist_touch_first(llist_t hdl);

/**************************************************************************//**
*
* llist_get_first
*
* @brief      remove and return the pointer of the first object of a linked
*             list
*
* @param[in]  hdl      handle of the linked list
*
* @return     pointer to the first object in the linked list, NULL if no
*             object is found in the list
*
******************************************************************************/
void* llist_get_first(llist_t hdl);

/**************************************************************************//**
*
* llist_remove
*
* @brief      remove an object from a linked list
*
* @param[in]  hdl      handle of the linked list
*
* @param[in]  object   pointer of object to be removed from the linked list
*
* @return     -
*
******************************************************************************/
void llist_remove(llist_t hdl, void* object);

/**************************************************************************//**
*
* llist_sort
*
* @brief      sort the objects in the linked list by the help of a compare
*             callback function
*
* @param[in]  hdl        handle of the linked list
*
* @param[in]  comp_func  pointer to callback function for comparing two objects
*                        in the linked list. the return value of the callback
*                        function determines whether the first object given is
*                        less (minus return value), equal(zero return value),
*                        or greater (plus return value) than the second object
*
* @return     -
*
******************************************************************************/
void llist_sort(llist_t hdl, int8_t (*comp_func)(void*, void*));

#if (LLIST_DEBUG_MODE == TRUE)
/**************************************************************************//**
*
* llist_check
*
* @brief      check the linked list by going through all its objects
*
* @param[in]  hdl        handle of the linked list
* @param[in]  callback   pointer to callback function to check the linked list
*                        objects
*
* @return     -
*
******************************************************************************/
void llist_check(llist_t hdl, void (*callback)(void*));
#endif /* (LLIST_DEBUG_MODE == TRUE) */

/** @} LLIST API Functions */

/** @} LLIST_API */

#endif /* _LLIST_H_*/
