/*
   Copyright 2016 Sandvine Incorporated ULC

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

//! @file sharedLibTypes.h
//! This is the interface that a shared library will implement to have code
//! loaded into the policy engine.

#ifndef POLICY_SHARED_LIB_TYPES_H
#define POLICY_SHARED_LIB_TYPES_H

#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! The possible argument and return types
typedef enum psl_DataType
{
    psl_boolean = 0,
    psl_string = 1,
    psl_integer = 2,
    psl_float = 3,
    psl_ipaddress = 4
} psl_DataType;

//! String arguments and return values are references, not passed by value.
//! Strings are pointer/length (not null-terminated).
typedef struct psl_stringRef
{
    const char*    begin;
    unsigned       length;
} psl_stringRef;

//! The argument and return values, per psl_DataType
typedef union psl_Value
{
    bool          b;     //!< boolean
    struct psl_stringRef s;     //!< string reference
    int64_t       i;     //!< integer
    double        f;     //!< float
    uint8_t       a[16]; //!< address IPv6 or IPv4-mapped-IPv6
} psl_Value;

//! The function signature required for loadable functions.
//! ResultLocation points to where the result will be stored.
//! Arguments points to an array of pointers to the arguments.
//! Null value arguments are represented by null pointers.
//! E.g., if arg0 was defined as float type, first check if it is null by
//!       checking Arguments[0]==0. If it is not null, access the arg by Arguments[0]->f
//! Return value of true indicates the result is valid; false results in a null result.
typedef bool psl_Function(psl_Value* ResultLocation, const psl_Value* const* Arguments);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif
