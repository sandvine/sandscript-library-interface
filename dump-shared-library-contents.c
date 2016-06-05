#include <stdio.h>
#include <dlfcn.h>

#include "sandscript-library-interface/sharedLibManifest.h"
#include "sandscript-library-interface/sharedLibEvents.h"

psl_GetEventManifest GetEventManifest;
psl_GetManifest GetManifest;

static void
dumpLib(char *lib)
{
    int i,j;
    psl_GetManifest *mp;
    psl_GetEventManifest *ep;
    const psl_Manifest *m;
    const psl_EventManifest * e;
    void *lh;

    lh = dlopen(lib, RTLD_NOW | RTLD_GLOBAL);
    if (!lh)
    {
        fprintf(stderr,"Error opening '%s': %s\n", lib, dlerror());
        return;
    }

    mp = dlsym(lh, "GetManifest");
    ep = dlsym(lh, "GetEventManifest");

    m = mp();
    
    if (m) 
    {
        printf("m->version: %u, m->numFunctionDescriptions: %u, m->functionDesscriptions: %p\n", m->version,
                                                                                                 m->numFunctionDescriptions,
                                                                                                 m->functionDescriptions);
        for (i = 0; i < m->numFunctionDescriptions; i++) 
        {
            printf(" Fn: %s\n", m->functionDescriptions[i]->functionName);
        }
    }

    e = ep();
    if (e) 
    {
        for (i = 0; i < e->numDescriptions; i++) 
        {
            printf(" Desc: %s [ctxt: %u]\n", e->eventDescriptions[i]->eventName, e->eventDescriptions[i]->contextNumber);
            for (j = 0; j < e->numFields; j++) 
            {
                if (e->eventDescriptions[i]->contextNumber == e->eventFields[j]->contextNumber)
                    printf("  Field: %s [ctxt: %u]\n", e->eventFields[j]->fieldName, e->eventFields[j]->contextNumber);
            }
        }
    }
}

int
main(int argc, char **argv)
{
    int libnum;

    for (libnum = 1; libnum < argc; libnum++)
    {
        dumpLib(argv[libnum]);
    }
}
