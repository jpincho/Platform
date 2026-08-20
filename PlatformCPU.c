#include "PlatformCPU.h"
#include "Platform.h"

#if defined ( PLATFORM_COMPILER_MSVC )
#include <windows.h>
#elif defined (PLATFORM_COMPILER_GNU)
#include <unistd.h>
#endif


unsigned Platform_GetCoreCount ( void )
	{
#if defined ( PLATFORM_COMPILER_MSVC )
	SYSTEM_INFO Info;
	GetSystemInfo ( &Info );
	if ( Info.dwNumberOfProcessors == 0 )
		return 1;
	return ( unsigned ) Info.dwNumberOfProcessors;
#else
	const long Count = sysconf ( _SC_NPROCESSORS_ONLN );
	if ( Count < 1 )
		return 1;
	return ( unsigned ) Count;
#endif
	}