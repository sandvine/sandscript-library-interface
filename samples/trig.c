//
// Compile with: gcc -fPIC -Wall -I.. -shared -o trig.so trig.c
//
#include "sharedLibManifest.h"
#include "sharedLibTypes.h"

// this is just needed for sin() function
#include <math.h>

// forward declare sin_fn() as of the required type for the API.
psl_Function sin_fn;
// sin() takes a single argument, type float.
const psl_DataType function1Args [] = {psl_float};
const psl_FunctionDescription function1 = {
    "sin",          // PAL name
    psl_Flag_Pure,  // flags. Currently only Pure is available. sin depends only on input args and has no side-effects.
    sin_fn,         // function called when "sin" is invoked in SandScript
    psl_float,      // return type
    1,              // length of arguments list
    function1Args   // the list of argument types above
};

// the list of all functions provided by this .so
const psl_FunctionDescription* functions [] = {
    &function1
};

struct psl_Manifest s_manifest = {
    /*version*/ 1,
    sizeof(functions)/sizeof(functions[0]),  // length of 'functions' list
    functions
};

// every .so must implement GetManifest to the following signature.
psl_GetManifest GetManifest __attribute__ ((visibility ("default"))); // verify it is correct type
const psl_Manifest* GetManifest()
{
    return &s_manifest;
}

// This is the function that is called when invoked by PAL
bool sin_fn(psl_Value* ResultLocation, const psl_Value * const* Arguments)
{
    // null arguments are represented by null pointers. You have to check.
    if(!Arguments[0]) return false; // null
    // Calculate the function of the args and store the result.
    ResultLocation->f = sin(Arguments[0]->f);
    // return true to say the result is not null. Return false for a null result.
    return true;
}
