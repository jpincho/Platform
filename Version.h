#pragma once
#include <stdbool.h>
#include "defines.h"

BEGIN_C_DECLARATIONS
typedef struct 
	{
	unsigned major, minor;
	}VersionNumber;

void VersionNumber_Set ( VersionNumber *output, const unsigned major, const unsigned minor );
bool VersionNumber_ParseString ( VersionNumber *output, const char *string );
bool VersionNumber_LesserThan ( const VersionNumber first, const unsigned major, const unsigned minor );
bool VersionNumber_GreaterThan ( const VersionNumber first, const unsigned major, const unsigned minor );
bool VersionNumber_Equal ( const VersionNumber first, const unsigned major, const unsigned minor );
bool VersionNumber_LesserThanOrEqual ( const VersionNumber first, const unsigned major, const unsigned minor );
bool VersionNumber_GreaterThanOrEqual ( const VersionNumber first, const unsigned major, const unsigned minor );
END_C_DECLARATIONS
