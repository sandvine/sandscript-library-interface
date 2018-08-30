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

//! @file sharedLibManifest.h
//! This is the interface that a shared library will implement to have code
//! loaded into the policy engine.

#ifndef POLICY_SHARED_LIB_MANIFEST_H
#define POLICY_SHARED_LIB_MANIFEST_H

#include "sharedLibTypes.h"

#include <unistd.h>
#include <stdbool.h>  // not needed

#ifdef __cplusplus
extern "C" {
#endif

//! Describes one function in the shared library.
typedef struct psl_FunctionDescription
{
    const char*   functionName;   //!< null-terminated string for the name of the function in policy.
    uint32_t      flags;          //!< Flags. See psl_Flag_Pure. Set unused bits to to 0.
    psl_Function* functionPointer;//!< Function to be called at each invocation.
    psl_DataType  returnType;     //!< Data type of the function return value.
    uint16_t      numArgs;        //!< Number of arguments the function requires.
    const psl_DataType* argTypes; //!< Data types of each function argument.
} psl_FunctionDescription;

//! include this flag in the flags field if the function depends solely on its inputs.
//! If this flag is set, some optimizations may occur such that the function is not
//! called each case it is mentioned, such as if its output is not used.
#define psl_Flag_Pure  (0x00000001)

//! The information about functions found in this shared library.
typedef struct psl_Manifest
{
    //! Library API version identifier. Use 1.
    unsigned                       version;
    //! Number of entries in the functionDescriptions array.
    unsigned                       numFunctionDescriptions;
    //! Array, with one entry for each function in the shared library.
    const psl_FunctionDescription* const* functionDescriptions;
} psl_Manifest;

//! Function signature required for the function named "GetManifest"
typedef const psl_Manifest* psl_GetManifest(void);

//! The function name to be retrieved via dlsym() from the shared library.
//! This is the version 1 name. Different versions should implement different names.
//! A shared library could implement multiple versions.
#define psl_GetManifestFunctionNameV1 "GetManifest"

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif
