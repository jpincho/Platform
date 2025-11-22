#include "Version.h"
#include <string.h>
#if defined ( _MSC_VER  )
#include <stdio.h>
#endif

void Version_Set(struct version *output, const unsigned major, const unsigned minor)
	{
	output->major = major;
	output->minor = minor;
	}

bool Version_ParseString(struct version *output, const char *string)
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

bool Version_LesserThan (const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major < major) ||
	        ((first.major == major) && (minor < minor)));
	}

bool Version_GreaterThan(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major > major) ||
	        ((first.major == major) && (minor > minor)));
	}

bool Version_Equal(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major == major) && (first.minor == minor));
	}

bool version_LesserThanOrEqual(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((Version_LesserThan(first, major, minor)) || (Version_Equal(first, major, minor)));
	}

bool version_GreaterThanOrEqual(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((Version_GreaterThan(first, major, minor)) || (Version_Equal(first, major, minor)));
	}
