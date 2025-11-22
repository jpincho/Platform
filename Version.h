#pragma
#include <stdbool.h>
#include "defines.h"

BEGIN_C_DECLARATIONS
struct version
	{
	unsigned major, minor;
	};
void Version_Set ( struct version *output, const unsigned major, const unsigned minor );
bool Version_ParseString ( struct version *output, const char *string );
bool Version_LesserThan ( const struct version first, const unsigned major, const unsigned minor );
bool Version_GreaterThan ( const struct version first, const unsigned major, const unsigned minor );
bool Version_Equal ( const struct version first, const unsigned major, const unsigned minor );
bool version_LesserThanOrEqual ( const struct version first, const unsigned major, const unsigned minor );
bool version_GreaterThanOrEqual ( const struct version first, const unsigned major, const unsigned minor );
END_C_DECLARATIONS
