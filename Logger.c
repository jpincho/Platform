#include "Logger.h"
#include "PlatformTime.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

static bool print_time = false;

void Logger_Log(const char *filename, const char *long_function_name, const char *short_function_name, const unsigned line, int8_t text_color, const char *message, ...)
	{
	static char complete_log_message[1024];
	static char time_string[25]="";

	if (print_time)
		{
		local_time_struct time = Platform_GetLocalTime();
		snprintf(time_string, sizeof(time_string), "%02u-%02u-%04u %02u:%02u:%02u:%03u ", time.tm_mday, time.tm_mon + 1, time.tm_year + 1900, time.tm_hour, time.tm_min, time.tm_sec, (unsigned)time.tm_usec / 1000);
		}
	else
		time_string[0] = 0;


	va_list args;
	va_start(args, message);
	vsnprintf(complete_log_message, sizeof(complete_log_message), message, args);
	va_end(args);
	size_t length = strlen(complete_log_message);
	if (complete_log_message[length - 1] == '\n')
		complete_log_message[length - 1] = 0;

	const char *textcolor_string;
	if (text_color != -1)
		textcolor_string = TextColorF(text_color);
	else
		textcolor_string = NoTextColor();
	printf("%s%s%s%s (%u) - %s%s\n", TextColorF(YELLOW), time_string, TextColorF(GREEN), short_function_name, line, textcolor_string, complete_log_message);
	}
