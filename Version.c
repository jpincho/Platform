#include "Version.h"
#include <string.h>
#if defined ( _MSC_VER  )
#include <stdio.h>
#endif

void VersionNumber_Set(VersionNumber *output, const unsigned major, const unsigned minor)
	{
	output->major = major;
	output->minor = minor;
	}

bool VersionNumber_ParseString(VersionNumber *output, const char *string)
	{
	// Safe values
	output->major = 0;
	output->minor = 0;

	const char *DotPosition = strchr(string, '.');
	if (DotPosition == NULL)
		return false;

	const char *VersionStart;
	for (VersionStart = DotPosition; (VersionStart > string) && (*VersionStart != ' '); --VersionStart);
	if (*VersionStart != ' ') // If there was nothing before the version numbers, assume the whole string
		VersionStart = string;

#if defined ( _MSC_VER  )
	sscanf_s(VersionStart, "%u.%u", &output->major, &output->minor);
#else
	sscanf(VersionStart, "%u.%u", &output->major, &output->minor);
#endif
	return true;
	}

bool VersionNumber_LesserThan (const VersionNumber first, const unsigned major, const unsigned minor)
	{
	return ((first.major < major) ||
	        ((first.major == major) && (minor < minor)));
	}

bool VersionNumber_GreaterThan(const VersionNumber first, const unsigned major, const unsigned minor)
	{
	return ((first.major > major) ||
	        ((first.major == major) && (minor > minor)));
	}

bool VersionNumber_Equal(const VersionNumber first, const unsigned major, const unsigned minor)
	{
	return ((first.major == major) && (first.minor == minor));
	}

bool VersionNumber_LesserThanOrEqual(const VersionNumber first, const unsigned major, const unsigned minor)
	{
	return ((VersionNumber_LesserThan(first, major, minor)) || (VersionNumber_Equal(first, major, minor)));
	}

bool VersionNumber_GreaterThanOrEqual(const VersionNumber first, const unsigned major, const unsigned minor)
	{
	return ((VersionNumber_GreaterThan(first, major, minor)) || (VersionNumber_Equal(first, major, minor)));
	}
