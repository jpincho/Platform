#pragma
#include <stdbool.h>
#include "defines.h"

BEGIN_C_DECLARATIONS
struct version
    {
    unsigned major, minor;
    };
void version_set ( struct version *output, const unsigned major, const unsigned minor );
bool version_parse_string ( struct version *output, const char *string );
bool version_lesser_than ( const struct version first, const unsigned major, const unsigned minor );
bool version_greater_than ( const struct version first, const unsigned major, const unsigned minor );
bool version_equal ( const struct version first, const unsigned major, const unsigned minor );
bool version_lesser_or_equal_than ( const struct version first, const unsigned major, const unsigned minor );
bool version_greater_or_equal_than ( const struct version first, const unsigned major, const unsigned minor );
END_C_DECLARATIONS
