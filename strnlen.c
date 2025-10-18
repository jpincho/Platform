#include "strnlen.h"
#include <memory.h>

#if !defined (PLATFORM_STRNLEN_EXISTS)
size_t strnlen(const char *string, const size_t max)
    {
    size_t len = 0;
    while (len < max && string[len])
        ++len;
    return len;
    }
#endif