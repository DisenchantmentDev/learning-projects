#pragma once

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

long long
strtonum(const char *numstr, long long minval, long long maxval,
    const char **errstrp);

