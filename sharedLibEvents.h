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

//! @file sharedLibEventsManifest.h
//! This is the events interface that a shared library will implement to have
//! code triggering events into the policy engine.

#ifndef POLICY_SHARED_LIB_EVENTS_H
#define POLICY_SHARED_LIB_EVENTS_H

#include "sharedLibTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// handle for an instance of an event allocated and freed by shared lib.
typedef void* psl_EventInstanceHandle;

//! @param context      Provides the context number of the event that is returned.
//! @param eventHandle  Returns a handle to the memory holding an event.
//! @return -1 if no events; otherwise the number of events waiting after this one.
typedef int psl_PollEventsFn(unsigned context, psl_EventInstanceHandle* eventHandle);

typedef void psl_FreeEventFn(psl_EventInstanceHandle eventHandle);

typedef struct psl_EventDescription
{
	const char*  eventName;     // what the event should be called in error logs, PDB, etc.
	unsigned     contextNumber;  // start numbering at 1.
	// the function to call to see what events need to be run
	psl_PollEventsFn* pollEvents;
	// function to call to free an event handle
	psl_FreeEventFn*   freeEvent;
} psl_EventDescription;

//! @param ResultLocation  Place the result here, of type type indicated in psl_EventField
//! @param eventHandle     Points to the memory of the event that fired. Must not be used if the field is a constant.
//! @return false if null, true otherwise (and result is provided)
typedef bool psl_FieldGetFn(psl_Value* ResultLocation, psl_EventInstanceHandle eventHandle);

typedef struct psl_EventField
{
	unsigned contextNumber; // what context the field applies to
	const char* fieldName;  // full name of field, such as MyEvent.foo
	psl_DataType  type;     // is it string, float, etc.?
	uint32_t      flags;    // isConstant, canOptimize, etc. TBD.
	psl_FieldGetFn*  getFunction;  // call to obtain the field from the event pointed by handle.
} psl_EventField;

typedef struct psl_EventManifest
{
	unsigned version;
	unsigned numDescriptions;
	unsigned numFields;
	const psl_EventDescription* const* eventDescriptions;
	const psl_EventField* const* eventFields;
} psl_EventManifest;

typedef const psl_EventManifest* psl_GetEventManifest();

//! The function name to be retrieved via dlsym() from the shared library.
#define psl_GetEventManifestFunctionNameV1 "GetEventManifest"

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif
