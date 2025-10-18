#include <Shlwapi.h>
#include "fnmatch.h"
#if !defined (PLATFORM_FNMATCH_EXISTS)

int fnmatch(const char *pattern, const char *string, int flags)
    {
    if (PathMatchSpecA(string, pattern) == TRUE)
        return 0;
    return FNM_NOMATCH;
    }
#endif