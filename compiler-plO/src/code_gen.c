#include <sys/stat.h>

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
 *   aout(const char, ...): function which prints to std out. Only function
 * which handles this cg_end(): called when parsing is complete and successful.
 * Simply notifies if compilation succeeded
 */

/* Function which is convenient way of typing (void) fprintf(stdout, "%s"...);
 */
static void
aout (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    (void)vfprintf (stdout, fmt, ap);
    va_end (ap);
}

/*
 * Symantics checking function.
 * This is needed due to the fact that, in a statement, we can either be
 * initializing, updating, or doing a variety of other things within the code
 * in such a way that is indistinguishable from each other. This checks
 * specific sides of the code depending on what is passed in
 */

static void
symcheck (int check)
{
    struct symtab *curr, *ret = NULL;

    curr = head;
    while (curr != NULL)
        {
            if (!strcmp (token, curr->name))
                ret = curr;
            curr = curr->next;
        }

    if (ret == NULL)
        error ("undefined symbol: %s", token);

    switch (check)
        {
        case CHECK_LHS:
            if (ret->type != TOK_VAR)
                error ("must be a variable: %s", token);
            break;
        case CHECK_RHS:
            if (ret->type == TOK_PROCEDURE)
                error ("must not be a procedure: %s", token);
            break;
        case CHECK_CALL:
            if (ret->type != TOK_PROCEDURE)
                error ("must be a procedure: %s", token);
        }
}

/* Function for initializing the table that tracks symbols we're writing.
 * This is done via a struct "symtab" (defined in main) that stores
 * information such as function depth, token type, symbol name (think
 * of it like the scope of the symbol aka what function is it in), and the next
 * tab (singly linked list for simplicity). */

void
cg_const (void)
{
    aout ("const long %s=", token);
}

void
cg_var (void)
{
    aout ("long %s;\n", token);
}

void
cg_semicolon (void)
{
    aout (";\n");
}

void
cg_procedure (void)
{
    if (proc == 0)
        {
            aout ("int\n");
            aout ("main(int argc, char *argv[])\n");
        }
    else
        {
            aout ("void\n");
            aout ("%s(void)\n", token);
        }
    aout ("\n");
}

void
cg_epilogue (void)
{
    aout (";");

    if (proc == 0)
        aout ("return 0;");

    aout ("\n}\n\n");
}

void
cg_cymbol (void)
{
    switch (type)
        {
        case TOK_IDENT:
            aout ("%s", token);
            break;
        case TOK_NUMBER:
            aout ("{\n");
            break;
        case TOK_END:
            aout (";\n}\n");
            break;
        case TOK_THEN:
        case TOK_DO:
            aout (")");
            break;
        case TOK_ODD:
            aout ("(");
            break;
        case TOK_WHILE:
            aout ("while(");
            break;
        case TOK_EQUAL:
            aout ("==");
            break;
        case TOK_COMMA:
            aout (",");
            break;
        case TOK_ASSIGN:
            aout ("=");
            break;
        case TOK_HASH:
            aout ("!=");
            break;
        case TOK_LESSTHAN:
            aout ("<");
            break;
        case TOK_GREATERTHAN:
            aout (">");
            break;
        case TOK_PLUS:
            aout ("+");
            break;
        case TOK_MINUS:
            aout ("-");
            break;
        case TOK_MULTIPLY:
            aout ("*");
            break;
        case TOK_DIVIDE:
            aout ("/");
            break;
        case TOK_LPAREN:
            aout ("(");
            break;
        case TOK_RPAREN:
            aout (")");
            break;
        }
}

void
cg_crlf (void)
{
    aout ("\n");
}

void
cg_end (void)
{
    aout ("/* PL/0 compiler %s */\n", PL0C_VERSION);
}
