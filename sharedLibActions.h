#ifndef POLICY_SHARED_LIB_ACTIONS_H
#define POLICY_SHARED_LIB_ACTIONS_H

//! Describes one argument of the action.
typedef struct psl_ArgDescription
{
    const char*   argument_name;
    psl_DataType  type;
    //! Argument flags: see psl_ActionArgFlag_*
    uint32_t      flags;
} psl_ArgDescription;

//! Used in psl_ArgDescription::flags, indicates the argument is optional.
#define psl_ActionArgFlag_optional 1

typedef struct psl_ArgValue
{
    //! A pointer to the same description provided by library when registering the action.
    const psl_ArgDescription* argDefinition;
    //! If non-zero, the value is valid. If zero, the value is null.
    uint8_t                   notNull;
    //! Only valid if notNull; interpretation depends on argDefinition.type
    psl_Value                 argValue;
} psl_ArgValue;

//! @param Result  Location the function places the function return value. Never null pointer.
//! @param eventHandle  The event the action is called in. The type will match
//!                     the event type it was registered with. If registered as any (-1),
//!                     the function must not use the handle.
//! @param args         Array of arguments passed to the action. All required (non optional)
//!                     arguments will be present.
//! @param numArgs      Size of array args.
//! @return bool  True if Result is valid; if false Result will not be used.
typedef bool psl_ActionFn(psl_Value* Result,
                          psl_EventInstanceHandle eventHandle,
                          const psl_ArgValue* args,
                          unsigned numArgs);

#endif

