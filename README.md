sandscript-library-interface
============================

This provides the header files for interfacing Sandvine's SandScript Policy
Engine to arbitrary external shared libraries.

# The Public Interfaces

The SandScript Policy Engine will `dlopen()` the library and seek these
symbols:
 - `GetManifest`
 - `GetEventManifest`

These are both functions. You may implement one or both of these.
 
GetManifest must be a symbol of type `psl_GetManifest`, specifically a function
that returns the manifest of all functions in the library. You will need to
implement this if you want to extend SandScript with **functions** a SandScript
author can call.

GetEventManifest must be a symbol of type `psl_GetEventManifest`, a function
that returns the manifest of all of the events in the library.  You will need
to implement this if you want to extend SandScript to **raise events** that can be
handled in SandScript.

The library developer's primary role is to fill in the manifests with all of
the information to describe the functions and events provided by the shared
library. Please refer to the header files for more information:
 - `sharedLibTypes.h` -- common data types
 - `sharedLibManifest.h` -- for declaring functions
 - `sharedLibEvents.h` -- for declaring events

# Compiling

You are building a shared library, so (assuming you are using gcc) it must be
linked with `-shared` and `-fPIC`. We also recommend turning on all warnings

    gcc -fPIC -Wall -shared -o my_shared_lib.so source1.c source2.c ...

Deploy the shared library in `/usr/local/sandvine/loadable/` to be picked up
the next time the policy engine is started.

# Sanity-Checking

To 'test' your library, there is a simple tool that will open your shared
library and read the manifests.

You can compile `dump-shared-library-contents.c` as:

    gcc -o dump-shared-library-contents dump-shared-library-contents.c -ldl

and then run it as:

    ./dump-shared-library-contents <mylib>

and it will dump out the data structures as the policy engine will see them.

This tools walks the manifests but does not actually call the functions you
have defined.

Note: once you have compiled the tool, you can test it on different versions of
your library without re-building it.

