#pragma once
#include "Platform.h"
#include "defines.h"
#include <stdlib.h>

BEGIN_C_DECLARATIONS
#if !defined (PLATFORM_STRNLEN_EXISTS)
size_t strnlen ( const char *string, const size_t max );
#endif
END_C_DECLARATIONS
