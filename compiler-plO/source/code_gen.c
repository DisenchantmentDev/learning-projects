#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "main.h"
#include "parser.h"

#include "code_gen.h"

/* 
* Code Generator
* Generates code into C. Consider rewriting into asm
* Functions:
*   aout(const char, ...): function which prints to std out. Only function which handles this
*   cg_end(): called when parsing is complete and successful. Simply notifies if compilation succeeded
*/

/* Function which is convenient way of typing (void) fprintf(stdout, "%s"...); */
static void
aout(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    (void) vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void
cg_end(void)
{
    aout("/* PL/0 compiler %s */\n", PL0C_VERSION);
}
