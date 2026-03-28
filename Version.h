#pragma once
#include <stdbool.h>
#include "defines.h"

BEGIN_C_DECLARATIONS
typedef struct
	{
	unsigned Major, Minor;
	} VersionNumber;

void VersionNumber_Set ( VersionNumber *Output, const unsigned Major, const unsigned Minor );
bool VersionNumber_ParseString ( VersionNumber *Output, const char *string );
bool VersionNumber_LesserThan ( const VersionNumber Version, const unsigned Major, const unsigned Minor );
bool VersionNumber_GreaterThan ( const VersionNumber Version, const unsigned Major, const unsigned Minor );
bool VersionNumber_Equal ( const VersionNumber Version, const unsigned Major, const unsigned Minor );
bool VersionNumber_LesserThanOrEqual ( const VersionNumber Version, const unsigned Major, const unsigned Minor );
bool VersionNumber_GreaterThanOrEqual ( const VersionNumber Version, const unsigned Major, const unsigned Minor );
END_C_DECLARATIONS
