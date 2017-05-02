/*
   Copyright 2016-2017 Sandvine Incorporated ULC

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

//! @brief Represents a string by pointer/length.
//!
//! String arguments and return values are references, not passed by value.
//! Strings are pointer/length (not null-terminated).
typedef struct psl_stringRef
{
    const char*    begin;  //!< Start of the binary string
    unsigned       length; //!< Number of octets in the binary string
} psl_stringRef;

//! @brief The union to pass function arguments and return values.
//!
//! The member used depends on psl_DataType specified at the time the function was registered.
typedef union psl_Value
{
    bool          b;     //!< boolean
    struct psl_stringRef s;     //!< string reference
    int64_t       i;     //!< integer
    double        f;     //!< float
    uint8_t       a[16]; //!< address IPv6 or IPv4-mapped-IPv6
} psl_Value;

//! The function signature required for loadable functions.
//! @param ResultLocation  When the function is called, this is where the
//!                        function result must be stored if the function
//!                        returns true. If the function returns false,
//!                        the result is not used.
//!                        The ResultLocation is a union. The function
//!                        must fill the union according to the return
//!                        type registered in psl_FunctionDescription.
//!                        When returning a string, it is done by reference
//!                        to static memory in the library; this memory
//!                        will be copied before any other access is done
//!                        to the library.
//! @param Arguments  The arguments are passed to the function as an array
//!                   of pointers to psl_Value unions. The array size is
//!                   identical to the registered parameter list.
//!                   Some parameters may be null, represented by a NULL pointer;
//!                   it is up to the function to handle null in its own way.
//!                   Each Arguments[i] parameter will have the appropriate type
//!                   populated in the union according to the type specified
//!                   in argTypes of the psl_FunctionDescription.
//!                   E.g., if argTypes[2]==psl_float, first check if it is
//!                   null by testing if Arguments[2]==NULL. If it is not NULL,
//!                   then the argument will be available in Arguments[2]->f
//! @return  value of true indicates the result is in ResultLocation; false indicates a null result.
typedef bool psl_Function(psl_Value* ResultLocation, const psl_Value* const* Arguments);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif
