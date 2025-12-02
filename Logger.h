#pragma once
#include "Platform.h"
#include "defines.h"
#include "ConsoleTextColor.h"

BEGIN_C_DECLARATIONS
void Logger_Log ( const char *Filename, const char *LongFunctionName, const char *ShortFunctionName, const unsigned LineNumber, int8_t TextColor, const char *Text, ... );

#define LOG_MSG(...) do {Logger_Log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
#define LOG_WARNING(...) do{Logger_Log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, YELLOW, __VA_ARGS__ );} while(0)
#define LOG_DEBUG(...) do{Logger_Log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
#define LOG_ERROR(...) do{Logger_Log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, RED, __VA_ARGS__ );} while(0)
#define LOG_RAW(...) do{Logger_Log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
END_C_DECLARATIONS
