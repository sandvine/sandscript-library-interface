#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>


#include "sharedLibManifest.h"
#include "sharedLibEvents.h"
#include "sharedLibActions.h"

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
    char args[180];

    printf("| %-9s |%s(", types[f->returnType], f->functionName);
    for (i=0; i < f->numArgs; i++)
    {
        printf("%s%s", types[f->argTypes[i]], (i+1< f->numArgs) ? ", ":"");
    }
    printf(")\n");
}

static void
dumpEvents(const psl_EventManifest * const e)
{
    int i,j;

    if (e->version != 1)
    {
        fprintf(stderr, "Unsupported event manifest version %u\n", e->version);
        exit(1);
    }
    if (e->numFields == 0)
    {
        return;
    }

    printf(
    " EVENTS and FIELDS\n"
    "------------------------------------------------------------------------------\n"
    "|%-16s|%-30s|%-8s| Notes             |\n"
    "|----------------|------------------------------|--------|-------------------|\n",
      "Event Name","Field Name","Type");

    for (i = 0; i < e->numDescriptions; i++)
    {
        for (j = 0; j < e->numFields; j++)
        {
            if (e->eventDescriptions[i]->contextNumber == e->eventFields[j]->contextNumber)
                printf("|%-16s|%-30s|%-8s|%19s|\n",
                       e->eventDescriptions[i]->eventName,
                       e->eventFields[j]->fieldName,
                       types[e->eventFields[j]->type],"");
        }
    }

    printf(
    "------------------------------------------------------------------------------\n");
}

static const char* 
find_action_name(const psl_EventManifest* events, int contextNumber)
{
    if(events)
    {
        if (events->version != 1)
        {
            fprintf(stderr, "Unsupported event manifest version %u\n", events->version);
            exit(1);
        }
        for (unsigned i = 0; i < events->numDescriptions; i++)
        {
            if (events->eventDescriptions[i]->contextNumber == contextNumber)
            {
                return events->eventDescriptions[i]->eventName;
            }
        }
    }
    fprintf(stderr, "Event references a non-existent event using number %d\n", contextNumber);
    exit(1);
    return NULL;
}

static void
dumpActions(const psl_ActionManifest* actions, const psl_EventManifest* events)
{
    if (actions->version != 1)
    {
        fprintf(stderr, "Unsupported action manifest version %u\n", actions->version);
        exit(1);
    }
    if (actions->numActions == 0)
    {
        return;
    }
    printf(
    " ACTIONS\n"
    "-------------------------------------------------------------------------------\n"
    "|%-30s|%-8s| Context                             |\n"
    "|                              |        |    Param Type | Parameter name      |\n"
    "|------------------------------|--------|--------------------------------------\n",
      "Action Name","Type");
    for(unsigned i=0; i < actions->numActions; ++i)
    {
        struct psl_ActionDescription* action = &actions->actionDescriptions[i];
        const char* context = (action->contextNumber>=0)? find_action_name(events, action->contextNumber): "---";
        printf("|%-30s|%-8s|%-37s|\n", action->name, types[action->returnType], context);
        for(unsigned j=0; j < action->numArgDescriptions; ++j)
        {
            const psl_ArgDescription* arg = &action->argDescriptions[j];
            printf("|                              |        |   (%c)%9s|%-21s|\n",
                   (arg->flags & psl_ActionArgFlag_optional)? 'O': 'R',
                   types[arg->type],
                   arg->arg_name);
        }
        printf("|------------------------------|--------|--------------------------------------\n");
    }


}

static void
dumpLib(char *lib)
{
    void *lh = dlopen(lib, RTLD_NOW | RTLD_GLOBAL);
    if (!lh)
    {
        fprintf(stderr,"Error opening '%s': %s\n", lib, dlerror());
        return;
    }

    psl_GetManifest *mp = dlsym(lh, "GetManifest");
    psl_GetEventManifest *ep = dlsym(lh, "GetEventManifest");
    psl_GetActionManifest *am = dlsym(lh, "GetActionManifest");

    const psl_Manifest *m;
    if (mp && (m=mp()) )
    {
        if (m->version != 1)
        {
            fprintf(stderr, "Unsupported function manifest version %u\n", m->version);
            exit(1);
        }

        if(m->numFunctionDescriptions)
        printf(" FUNCTIONS\n");
        printf("------------------------------------------------------------------------------\n");
        printf("|Return type| Function                                                       |\n");
        printf("------------------------------------------------------------------------------\n");
        for (unsigned i = 0; i < m->numFunctionDescriptions; i++)
        {
            dumpFunction(m->functionDescriptions[i]);
        }
        printf("------------------------------------------------------------------------------\n");
    }

    const psl_EventManifest * e;
    if (ep && (e=ep()) )
    {
        dumpEvents(e);
    }

    const psl_ActionManifest* a;
    if (am && (a=am()) )
    {
        dumpActions(a, e);
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
