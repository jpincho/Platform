#include "Version.h"
#include <string.h>
#if defined ( _MSC_VER  )
#include <stdio.h>
#endif

void version_set(struct version *output, const unsigned major, const unsigned minor)
	{
	output->major = major;
	output->minor = minor;
	}

bool version_parse_string(struct version *output, const char *string)
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

bool version_lesser_than (const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major < major) ||
		((first.major == major) && (minor < minor)));
	}

bool version_greater_than(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major > major) ||
		((first.major == major) && (minor > minor)));
	}

bool version_equal(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((first.major == major) && (first.minor == minor));
	}

bool version_lesser_or_equal_than(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((version_lesser_than(first, major, minor)) || (version_equal(first, major, minor)));
	}

bool version_greater_or_equal_than(const struct version first, const unsigned major, const unsigned minor)
	{
	return ((version_greater_than(first, major, minor)) || (version_equal(first, major, minor)));
	}
