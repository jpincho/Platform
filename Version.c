#include "Version.h"
#include "Platform.h"
#include <string.h>
#if defined (PLATFORM_COMPILER_MSVC)
#include <stdio.h>
#elif defined (PLATFORM_COMPILER_GCC)
#include <stdio.h>
#endif

void VersionNumber_Set ( VersionNumber *Output, const unsigned Major, const unsigned Minor )
	{
	Output->Major = Major;
	Output->Minor = Minor;
	}

bool VersionNumber_ParseString ( VersionNumber *Output, const char *string )
	{
	// Safe values
	Output->Major = 0;
	Output->Minor = 0;

	const char *DotPosition = strchr ( string, '.' );
	if ( DotPosition == NULL )
		return false;

	const char *VersionStart;
	for ( VersionStart = DotPosition; ( VersionStart > string ) && ( *VersionStart != ' ' ); --VersionStart );
	if ( *VersionStart != ' ' ) // If there was nothing before the version numbers, assume the whole string
		VersionStart = string;

#if defined ( PLATFORM_COMPILER_MSVC  )
	sscanf_s ( VersionStart, "%u.%u", &Output->Major, &Output->Minor );
#else
	sscanf ( VersionStart, "%u.%u", &Output->Major, &Output->Minor );
#endif
	return true;
	}

bool VersionNumber_LesserThan ( const VersionNumber Version, const unsigned Major, const unsigned Minor )
	{
	return ( ( Version.Major < Major ) ||
	         ( ( Version.Major == Major ) && ( Version.Minor < Minor ) ) );
	}

bool VersionNumber_GreaterThan ( const VersionNumber Version, const unsigned Major, const unsigned Minor )
	{
	return ( ( Version.Major > Major ) ||
	         ( ( Version.Major == Major ) && ( Version.Minor > Minor ) ) );
	}

bool VersionNumber_Equal ( const VersionNumber Version, const unsigned Major, const unsigned Minor )
	{
	return ( ( Version.Major == Major ) && ( Version.Minor == Minor ) );
	}

bool VersionNumber_LesserThanOrEqual ( const VersionNumber Version, const unsigned Major, const unsigned Minor )
	{
	return ( ( Version.Major < Major ) ||
	         ( ( Version.Major == Major ) && ( Version.Minor <= Minor ) ) );
	}

bool VersionNumber_GreaterThanOrEqual ( const VersionNumber Version, const unsigned Major, const unsigned Minor )
	{
	return ( ( Version.Major > Major ) ||
	         ( ( Version.Major == Major ) && ( Version.Minor >= Minor ) ) );
	}
