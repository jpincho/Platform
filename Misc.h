#pragma once
#include <Platform/defines.h>
#include <stdint.h>

BEGIN_C_DECLARATIONS
int rand_range ( const int min, const int max );
float frand ( void );
float frand_decimal ( void );
float frand_range ( const float min, const float max );
float fclamp ( const float value, const float min, const float max );
double dclamp ( const double value, const double min, const double max );

uint64_t round_pot_64 ( const uint64_t input );// Returns the power of two greater or equal to input
uint32_t round_pot_32 ( const uint32_t input );// Returns the power of two greater or equal to input


#define SWAP_VALUES( TYPE, A, B ) \
    do { \
        TYPE tmp = A; \
        A = B; \
        B = tmp; \
    } while ( 0 )
END_C_DECLARATIONS
