#pragma once
#include "Platform.h"

#if !defined (PLATFORM_FNMATCH_EXISTS)
BEGIN_C_DECLARATIONS
#define FNM_NOMATCH		-1
#define FNM_NOESCAPE	-2
#define FNM_PATHNAME	-3
#define FNM_PERIOD		-4
#define FNM_FILE_NAME	FNM_PATHNAME
#define FNM_LEADING_DIR	-5
#define FNM_CASEFOLD	-6
#define FNM_IGNORECASE	FNM_CASEFOLD
#define FNM_EXTMATCH	-7

int fnmatch ( const char *pattern, const char *string, int flags );
END_C_DECLARATIONS
#endif
