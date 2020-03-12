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
* @file     dev_types.h
* 
* @brief    device specific data types and hardware definitions header file
* 
* @version  0.2.1
*
* @author   Leo Hendrawan
*
* @remark   target device: MSP430G2452 & MSP430G2553 on MSP-EXP430G2 Launchpad
* 
******************************************************************************/

#ifndef _DEV_TYPES_H_
#define _DEV_TYPES_H_

//*****************************************************************************
// Include section
//*****************************************************************************

// include stdint.h and stbool.h if available
#include <msp430.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

//*****************************************************************************
// Global variable declarations 
//*****************************************************************************


//*****************************************************************************
// Macros (defines) and data types 
//*****************************************************************************

/** NULL
 *  null pointer */
#ifndef NULL
#define NULL                               ((void*)0)
#endif

/** TRUE
 *  general logical true
 */
#ifndef TRUE
#define TRUE                               (true)
#endif

/** FALSE
 *  general logical false
 */
#ifndef FALSE
#define FALSE                              (false)
#endif

/** SUCCESS
 *  general success operation
 */
#ifndef SUCCESS
#define SUCCESS                            (true)
#endif

/** ERROR
 *  general error operation
 */
#ifndef ERROR
#define ERROR                              (false)
#endif

/*-------------- data type check - DO NOT CHANGE -------------------------- */

/* test part for int8_t size */
typedef char dev_types_chk_int8[((sizeof(int8_t)==1)? 1 : -1)];

/* test part for uint8_t size */
typedef char dev_types_chk_uint8[((sizeof(uint8_t)==1)? 1 : -1)];

/* test part for int16_t size */
typedef char dev_types_chk_int16[((sizeof(int16_t)==2)? 2 : -1)];

/* test part for uint16_t size */
typedef char dev_types_chk_uint16[((sizeof(uint16_t)==2)? 2 : -1)];

/* test part for int32_t size */
typedef char dev_types_chk_int32[((sizeof(int32_t)==4)? 4 : -1)];

/* test part for uint32_t size */
typedef char dev_types_chk_uint32[((sizeof(uint32_t)==4)? 4 : -1)];


//*****************************************************************************
// External function declarations
//*****************************************************************************

#endif /* _DEV_TYPES_H_ */
