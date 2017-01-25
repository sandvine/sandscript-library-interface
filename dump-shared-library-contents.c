#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>


#include "sandscript-library-interface/sharedLibManifest.h"
#include "sandscript-library-interface/sharedLibEvents.h"

psl_GetEventManifest GetEventManifest;
psl_GetManifest GetManifest;
const char *types[] = {
    "boolean",
    "string",
    "integer",
    "float",
    "ipaddress"
};

static void
dumpFunction(const psl_FunctionDescription * const f)
{
    int i;

    printf("%s %s(", types[f->returnType], f->functionName);
    for (i=0; i < f->numArgs; i++)
    {
        printf("%s%s", types[f->argTypes[i]], (i+1< f->numArgs) ? ",":"");
    }
    printf(")\n");
}

static void
dumpEvents(const psl_EventManifest * const e)
{
    int i,j;
    printf(
    "------------------------------------------------------------------------------\n"
    "|%-28s|%-8s| Notes                                |\n"
    "|----------------------------|--------|--------------------------------------|\n",
      "Event-name","Type");

    for (i = 0; i < e->numDescriptions; i++)
    {
        for (j = 0; j < e->numFields; j++)
        {
            if (e->eventDescriptions[i]->contextNumber == e->eventFields[j]->contextNumber)
                printf("|%-28s|%-8s|%38s|\n",
  e->eventFields[j]->fieldName,
  types[e->eventFields[j]->type],"");
        }
    }

    printf(
    "------------------------------------------------------------------------------\n");
}

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
        printf("m->version: %u, m->numFunctionDescriptions: %u, m->functionDesscriptions: %p\n", m->version, m->numFunctionDescriptions, m->functionDescriptions);
        for (i = 0; i < m->numFunctionDescriptions; i++)
        {
            dumpFunction(m->functionDescriptions[i]);
//            printf(" Fn: %s\n", m->functionDescriptions[i]->functionName);
        }
    }

    e = ep();
    if (e)
    {
        dumpEvents(e);
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
    exit(0);
}
