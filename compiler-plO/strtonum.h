#ifndef STREAM_H_DEFINED
#define STREAM_H_DEFINED

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

long long
strtonum(const char *numstr, long long minval, long long maxval,
    const char **errstrp);

#endif
