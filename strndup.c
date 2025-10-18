#include "strndup.h"
#include <memory.h>
#if defined (PLATFORM_STRNLEN_EXISTS)
#include <string.h>
#else
#include "strnlen.h"
#endif

#if !defined (PLATFORM_STRNDUP_EXISTS)
char *strndup(const char *string, const size_t max)
    {
    size_t len = strnlen(string, max);
    char *p = malloc(len + 1);
    if (p)
        {
        memcpy(p, string, len);
        p[len] = '\0';
        }
    return p;
    }
#endif