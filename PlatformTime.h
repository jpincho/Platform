#pragma once
#include <time.h>
#include "defines.h"

BEGIN_C_DECLARATIONS
typedef struct
    {
    int tm_sec;   // seconds after the minute - [0, 60] including leap second
    int tm_min;   // minutes after the hour - [0, 59]
    int tm_hour;  // hours since midnight - [0, 23]
    int tm_mday;  // day of the month - [1, 31]
    int tm_mon;   // months since January - [0, 11]
    int tm_year;  // years since 1900
    int tm_wday;  // days since Sunday - [0, 6]
    int tm_isdst; // daylight savings time flag
    int tm_usec;  // micro seconds
    } local_time_struct;

double Platform_GetPerformanceCurrentTime ( void );
local_time_struct Platform_GetLocalTime ( void );
END_C_DECLARATIONS
