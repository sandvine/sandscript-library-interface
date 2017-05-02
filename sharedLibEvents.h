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

//! @file sharedLibEvents.h
//! This is the events interface that a shared library will implement to have
//! code triggering events into the policy engine.

#ifndef POLICY_SHARED_LIB_EVENTS_H
#define POLICY_SHARED_LIB_EVENTS_H

#include "sharedLibTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//! Handle for an instance of an event allocated and freed by shared lib.
typedef void* psl_EventInstanceHandle;

//! @brief Type of function for raising events to SandScript.
//
//! Events are raised to SandScript by the Policy Engine polling for them.
//! For each instance of an event, the PollEventsFn allocates a handle,
//! which it returns as an opaque pointer (psl_EventInstanceHandle is a void*).
//! This handle will be subsequently passed to psl_FieldGetFn to retrieve field
//! values.
//! When all policy in the SandScript event handler has been run, the handle
//! is released by calling the psl_FreeEventFn.
//! It is important the function only return events for the specified context;
//! otherwise the Policy Engine may call the wrong fields with the handle.
//! @param context      The context number of the event being polled (in
//!                     case the same function is used for different contexts).
//! @param eventHandle  Returns a handle to the private memory holding an event.
//! @return -1 if no events; otherwise the number of events waiting after this one.
typedef int psl_PollEventsFn(unsigned context, psl_EventInstanceHandle* eventHandle);

//! The function prototype for freeing events obtained from the corresponding psl_PollEventsFn
//! @param eventHandle  A handle previously obtained from psl_PollEventsFn
typedef void psl_FreeEventFn(psl_EventInstanceHandle eventHandle);

//! Description of an event raised by the shared library.
typedef struct psl_EventDescription
{
    //! what the event should be called in error logs, PDB, etc., such as "MyEvent"
    const char*  eventName;    
    //! A number the library uses to uniquely identify the event type. Must be unique within a library.
    unsigned     contextNumber;
    //! the function to call to see what events need to be run
    psl_PollEventsFn * pollEvents;
    //! function to call to free an event handle obtained by pollEvents
    psl_FreeEventFn*   freeEvent;
} psl_EventDescription;

//! @param ResultLocation  Place the result here, of type type indicated in psl_EventField
//! @param eventHandle     Points to the memory of the event that fired. Must not be used if the field is a constant.
//! @return false if null, true otherwise (and result is provided)
typedef bool psl_FieldGetFn(psl_Value* ResultLocation, psl_EventInstanceHandle eventHandle);

//! Description of a field of an event raised by the shared library
typedef struct psl_EventField
{
    //! The event (per psl_EventDescription::contextNumber) that this field may be used with.
    unsigned contextNumber;
    //! The name of the field to be exposed to SandScript, such as MyEvent.foo
    const char* fieldName;
    //! Type of the field;  is it string, float, etc.?
    psl_DataType  type;    
    //! flags, TBD. For now, use 0.
    uint32_t      flags;
    //! Function that SandScript will call to obtain the field identified by the handle.
    psl_FieldGetFn*  getFunction;
} psl_EventField;

//! Manifest detailing the contents of the shared library regarding events raised by it.
typedef struct psl_EventManifest
{
    //! Library API version identifier. Use 1.
    unsigned version;
    //! Number of entries of the eventDescriptions array
    unsigned numDescriptions;
    //! Number of entries of the eventFields array.
    unsigned numFields;
    //! Array of pointers to each of the events that can be raised by the library.
    const psl_EventDescription* const* eventDescriptions;
    //! Array of pointers to each of the fields exposed by the library.
    const psl_EventField* const* eventFields;
} psl_EventManifest;

//! The type of the function that is called GetEventManifest
typedef const psl_EventManifest* psl_GetEventManifest();

//! The function name to be retrieved via dlsym() from the shared library.
#define psl_GetEventManifestFunctionNameV1 "GetEventManifest"

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif
