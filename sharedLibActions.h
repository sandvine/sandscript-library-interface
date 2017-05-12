#ifndef POLICY_SHARED_LIB_ACTIONS_H
#define POLICY_SHARED_LIB_ACTIONS_H
#include <sharedLibTypes.h>
#include <sharedLibEvents.h>

//! Describes one argument of the action.
typedef struct psl_ArgDescription
{
    const char*   arg_name;
    uint16_t      arg_handle;
    psl_DataType  type;
    //! Argument flags: see psl_ActionArgFlag_*
    uint32_t      flags;
} psl_ArgDescription;

//! Used in psl_ArgDescription::flags, indicates the argument is optional.
#define psl_ActionArgFlag_optional 1

typedef struct psl_ArgValue
{
    //! Handle to uniquely identify the parameter, from psl_ArgDescription
    uint16_t      argHandle;
    //! If non-zero, the value is valid. If zero, the value is null.
    uint8_t       notNull;
    //! Only valid if notNull; interpretation depends on argDefinition.type
    psl_Value     argValue;
} psl_ArgValue;

//! @param Result  Location the function places the function return value. Never null pointer.
//! @param eventHandle  The event the action is called in. The type will match
//!                     the event type it was registered with. If registered as any (contextNumber=-1),
//!                     the function must not use the handle.
//! @param args         Array of arguments passed to the action. All required (non optional)
//!                     arguments will be present. The arguments must not be identified by
//!                     order in the list, rather by the argHandle, to match with the
//!                     handle used when specifying the arguments in psl_ArgDescription.
//!                     This array will only include arguments specified when invoked from SandScript.
//! @param numArgs      Size of array args.
//! @return bool  True if Result is valid; if false Result will not be used.
typedef bool psl_ActionFn(psl_Value* Result,
                          psl_EventInstanceHandle eventHandle,
                          const psl_ArgValue* args,
                          unsigned numArgs);

typedef struct psl_ActionDescription
{
    //! Name the function is called in SandScript
    const char*   name;
    //! The method to be called to invoke the action. @see psl_ActionFn
    psl_ActionFn* function;
    //! Return type of the action.
    psl_DataType  returnType;
    //! The context that the action must be called in, or -1 if the function may be called in any context.
    int           contextNumber;
    //! No flags defined yet. Set to 0.
    uint32_t      flags;
    //! Array of defined function arguments.
    const psl_ArgDescription* argDescriptions;
    //! Number of defined function arguments. (Size of argDescriptions array)
    unsigned                  numArgDescriptions;
} psl_ActionDescription;

typedef struct psl_ActionManifest
{
    //! API version. Use .version=1
    unsigned      version;
    //! Pointer to array of actions
    psl_ActionDescription*  actionDescriptions;
    //! Number of items in the array of actions
    unsigned                numActions;
} psl_ActionManifest;

//! The function signature that the library function "GetActionManifest" must conform to.
typedef const psl_ActionManifest* psl_GetActionManifest(void);

//! This is the name of the function in the library to be sought with dlsym
#define psl_GetActionManifestFunctionNameV1 "GetActionManifest"

#endif

