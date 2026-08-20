#include "Misc.h"
#include <stdlib.h>
#include <Platform/Platform.h>

int rand_range ( const int min, const int max )
	{
	int random = rand () % ( max - min );
	return random + min;
	}

float frand ( void )
	{
	int random = rand ();
	return ( float ) random;
	}

float frand_decimal ( void )
	{
	float max = RAND_MAX;
	return ( ( float ) rand () / max );
	}

float frand_range ( const float min, const float max )
	{
	return frand_decimal () * ( max - min ) + min;
	}

float fclamp ( const float value, const float min, const float max )
	{
	if ( value < min )
		return min;
	else if ( value > max )
		return max;
	else
		return value;
	}

double dclamp ( const double value, const double min, const double max )
	{
	if ( value < min )
		return min;
	else if ( value > max )
		return max;
	else
		return value;
	}

uint64_t round_pot_64 ( uint64_t input )
	{
	if ( input <= 1 )
		return input;

#if defined (PLATFORM_COMPILER_GNU)
	uint64_t output = 1 << ( 64 - __builtin_clzl ( input - 1 ) );
	return output;
#else
	uint64_t output = 1;
	uint64_t shifted = input;
	while ( shifted != 1 )
		{
		shifted >>= 1;
		output <<= 1;
		}
	if ( output < input )
		output <<= 1;
	return output;
#endif
	}

uint32_t round_pot_32 ( const uint32_t input )
	{
	if ( input <= 1 )
		return input;

#if defined (PLATFORM_COMPILER_GNU)
	uint32_t output = 1 << ( 32 - __builtin_clz ( input - 1 ) );
	return output;
#else
	uint32_t output = 1;
	uint32_t shifted = input;
	while ( shifted != 1 )
		{
		shifted >>= 1;
		output <<= 1;
		}
	if ( output < input )
		output <<= 1;
	return output;
#endif
	}