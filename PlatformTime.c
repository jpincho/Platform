#include "Platform.h"
#include "PlatformTime.h"
#if defined ( PLATFORM_COMPILER_MSVC )
#include <windows.h>
#include <sysinfoapi.h>
#else
#include <stdlib.h>
#endif

double Platform_GetPerformanceCurrentTime(void)
{
#if defined ( PLATFORM_COMPILER_MSVC )
	LARGE_INTEGER now, frequency = { 0, 0 };
	if (frequency.QuadPart == 0)
	{
		QueryPerformanceFrequency(&frequency);
	}
	QueryPerformanceCounter(&now);
	return (double)now.QuadPart / (double)frequency.QuadPart;
#else
	struct timespec spec;
	static double nano_divisor = 1.0 / 1.0e9;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	double result = spec.tv_nsec;
	result *= NanoDivisor;
	result += spec.tv_sec;
	return result;
#endif
}

local_time_struct Platform_GetLocalTime(void)
{
	local_time_struct result;
#if defined (PLATFORM_COMPILER_MSVC)
	SYSTEMTIME local_time;
	GetLocalTime(&local_time);
	result.tm_sec = local_time.wSecond;
	result.tm_min = local_time.wMinute;
	result.tm_hour = local_time.wHour;
	result.tm_mday = local_time.wDay;
	result.tm_mon = local_time.wMonth;
	result.tm_year = local_time.wYear;
	result.tm_wday = local_time.wDay;
	result.tm_usec = 0;
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	// Convert to local time
	struct tm* local_time = gmtime(&now);

	result.tm_sec = local_time->tm_sec;
	result.tm_min = local_time->tm_min;
	result.tm_hour = local_time->tm_hour;
	result.tm_mday = local_time->tm_mday;
	result.tm_mon = local_time->tm_mon;
	result.tm_year = local_time->tm_year+1900;
	result.tm_wday = local_time->tm_wday;
	result.tm_usec = now.tv_usec;
#endif

	return result;
}