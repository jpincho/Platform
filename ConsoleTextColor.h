#pragma once
#include "defines.h"

#include <stdio.h>
#include <stdint.h>

#define RESET		0
#define BOLD		1
#define DIM			2
#define UNDERLINE 	4
#define BLINK		5
#define REVERSE		7
//#define HIDDEN		8

#define BLACK 		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

BEGIN_C_DECLARATIONS
const char *TextColorF ( int8_t foreground );
const char *TextColor ( int8_t foreground, int8_t background );
const char *NoTextColor ( void );
END_C_DECLARATIONS
