sandscript-library-interface
============================

This provides the header files for interfacing Sandvine's 
SandScript Policy Engine to arbitrary external shared
libraries.

To 'test' your library, you can compile dump-shared-library-contents.c
as: 
        gcc -o dump-shared-library-contents dump-shared-library-contents.c -ldl

and then run it as:

        ./dump-shared-library-contents <mylib>

and it will dump out the data structures as the policy engine
will see them.
