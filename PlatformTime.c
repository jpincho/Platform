#include "Platform.h"
#include "PlatformTime.h"
#if defined ( PLATFORM_COMPILER_MSVC )
#include <windows.h>
#include <sysinfoapi.h>
#elif defined (PLATFORM_COMPILER_GCC)
#include <sys/time.h>
#endif

double Platform_GetPerformanceCurrentTime ( void )
	{
#if defined ( PLATFORM_COMPILER_MSVC )
	LARGE_INTEGER now, frequency = { 0, 0 };
	if ( frequency.QuadPart == 0 )
		{
		QueryPerformanceFrequency ( &frequency );
		}
	QueryPerformanceCounter ( &now );
	return ( double ) now.QuadPart / ( double ) frequency.QuadPart;
#elif defined ( PLATFORM_COMPILER_GCC)
	struct timespec spec;
        static double NanoDivisor = 1.0 / 1.0e9;
	clock_gettime ( CLOCK_MONOTONIC, &spec );
	double result = spec.tv_nsec;
	result *= NanoDivisor;
	result += spec.tv_sec;
	return result;
#else
#error "Undefined platform"
#endif
	}

local_time_struct Platform_GetLocalTime ( void )
	{
	local_time_struct result;
#if defined (PLATFORM_COMPILER_MSVC)
	SYSTEMTIME local_time;
	GetLocalTime ( &local_time );
	result.tm_sec = local_time.wSecond;
	result.tm_min = local_time.wMinute;
	result.tm_hour = local_time.wHour;
	result.tm_mday = local_time.wDay;
	result.tm_mon = local_time.wMonth;
	result.tm_year = local_time.wYear;
	result.tm_wday = local_time.wDay;
	result.tm_usec = 0;
#elif defined ( PLATFORM_COMPILER_GCC)
        struct timespec spec;
        clock_gettime ( CLOCK_REALTIME, &spec );
        struct tm *new_time=gmtime(&spec.tv_sec);


        result.tm_sec  = new_time->tm_sec;
        result.tm_min  = new_time->tm_min;
        result.tm_hour = new_time->tm_hour;
        result.tm_mday = new_time->tm_mday;
        result.tm_mon  = new_time->tm_mon;
        result.tm_year = new_time->tm_year + 1900;
        result.tm_wday = new_time->tm_wday;
        result.tm_usec = spec.tv_nsec / 1000;
#else
#error "Undefined platform"
#endif

	return result;
	}
