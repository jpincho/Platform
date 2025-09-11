#pragma once
#include "Platform.h"
#include <stdlib.h>

#if !defined (PLATFORM_STRNLEN_EXISTS)
size_t strnlen ( const char *string, const size_t max );
#endif
#if !defined (PLATFORM_STRNDUP_EXISTS)
char *strndup ( const char *string, const size_t max );
#endif
