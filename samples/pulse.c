// Implement pulse event in SandScript
// The event is raised every second, specifically
// whenever time(0) returns a new value.
// When the event is raised, the 'pulse' field will have the time.

#include "sharedLibEvents.h"
#include "sharedLibManifest.h"
#include <time.h>

#define arraysize(A) (sizeof(A)/sizeof(&A[0]))

// required interface
psl_GetEventManifest GetEventManifest __attribute__ ((visibility ("default")));
psl_GetManifest GetManifest __attribute__ ((visibility ("default")));

static psl_PollEventsFn pulse_poll;
static psl_FreeEventFn pulse_free;
static psl_FieldGetFn  pulse_get;

const psl_EventDescription event = {
    .eventName = "PulseTick",
    .contextNumber = 1,
    .pollEvents = &pulse_poll,
    .freeEvent = &pulse_free
};

const psl_EventDescription* const events = {
    &event
};

static const psl_EventField pulse_field =
{
    //! The event (per psl_EventDescription::contextNumber that this field may be used with.
    .contextNumber = 1,
    //! The name of the field to be exposed to SandScript, such as MyEvent.foo
    .fieldName = "pulse",
    //! Type of the field;  is it string, float, etc.?
    .type = psl_integer,    
    //! flags, TBD. For now, use 0.
    .flags = 0,
    //! call to obtain the field from the event pointed by handle.
    .getFunction = &pulse_get
};

const psl_EventField* fields[] = {
    &pulse_field
};

static psl_EventManifest eventManifest =
{
    //! Library API version identifier. Use 1.
    .version = 1,
    //! Number of entries of the eventDescriptions array
    .numDescriptions = arraysize(events),
    //! Number of entries of the eventFields array.
    .numFields = arraysize(fields),
    //! Array of pointers to each of the events that can be raised by the library.
    .eventDescriptions = &events,
    //! Array of pointers to each of the fields exposed by the library.
    .eventFields = fields
};

const psl_EventManifest* GetEventManifest()
{
    return &eventManifest;
}

const psl_Manifest manifest = {
    .version = 1,
    .numFunctionDescriptions = 0
};

const psl_Manifest* GetManifest()
{
    return &manifest;
}

static time_t last_time = 0;
static int
pulse_poll(unsigned context, psl_EventInstanceHandle* eventHandle)
{
    time_t t = time(0);
    if (t != last_time)
    {
	*eventHandle = (void*)t;
	last_time = t;
	return 0;
    }
    return -1;
}

static void
pulse_free(psl_EventInstanceHandle eventHandle)
{
}

//! @param ResultLocation  Place the result here, of type type indicated in psl_EventField
//! @param eventHandle     Points to the memory of the event that fired. Must not be used if the field is a constant.
//! @return false if null, true otherwise (and result is provided)
static bool
pulse_get(psl_Value* ResultLocation, psl_EventInstanceHandle eventHandle)
{
    ResultLocation->i = (time_t)eventHandle;
    return true;
}

