#pragma once
#include "Platform.h"
#include "defines.h"
#include "ConsoleTextColor.h"

BEGIN_C_DECLARATIONS
void logger_log( const char *filename, const char *long_function_name, const char *short_function_name, const unsigned line, int8_t text_color, const char *message, ... );

#define LOG_MSG(...) do {logger_log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
#define LOG_WARNING(...) do{logger_log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, YELLOW, __VA_ARGS__ );} while(0)
#define LOG_DEBUG(...) do{logger_log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
#define LOG_ERROR(...) do{logger_log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, RED, __VA_ARGS__ );} while(0)
#define LOG_RAW(...) do{logger_log ( __FILE__, __PRETTY_FUNCTION__, __FUNCTION__, __LINE__, -1, __VA_ARGS__ );} while(0)
END_C_DECLARATIONS
