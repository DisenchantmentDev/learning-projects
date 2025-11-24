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

/* Function for initializing the table that tracks symbols we're writing.
 * This is done via a struct "symtab" (defined in main) that stores 
 * information such as function depth, token type, symbol name (think
 * of it like the scope of the symbol aka what function is it in), and the next
 * tab (singly linked list for simplicity). */
void
initsymtab(void)
{
    struct symtab *new;

    if ((new = malloc(sizeof(struct symtab)) == NULL))
        error("malloc of symtab creation failed");

    new -> depth = 0;
    new->type = TOK_PROCEDURE;
    new->name = "main";
    new->next = NULL;

    head = new; 
}

void
addsymbol(int type)
{
    struct symtab *curr, *new;

    curr = head;
    while (1) {
        if (!strcmp(curr->name, token)) {
            if (curr->depth == (depth - 1))
                error("duplicate symbol: %s", token);
        }

        if (curr->next == NULL)
            break;

        curr = curr->next;
    }

    if ((new = malloc(sizeof(struct symtab))) == NULL)
        error("malloc of symtab failed");

    new->depth = depth - 1;
    new ->type = type;
    if((new->name = strdup(token)) == NULL)
        error("malloc failed");
    new->next = NULL;

    curr->next = new;
}

void
destroysymbols(void)
{
    struct symtab *curr, *prev;

again:
    curr = head;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    if (curr->type != TOK_PROCEDURE) {
        free(curr->name);
        free(curr);
        prev->next = NULL;
        goto again;
    }
}

void
cg_const(void)
{
    aout("const long %s=", token);
}

void
cg_semicolon(void) {
    aout(";\n");
}

void
cg_cymbol(void)
{
    switch (type) {
    case TOK_IDENT:
        aout("%s", token);
        break;
    case TOK_NUMBER:
        aout("{\n");
        break;
    case TOK_END:
        aout(";\n}\n");
        break;
    case TOK_THEN:
    case TOK_DO:
        aout(")");
        break;
    case TOK_ODD:
        aout("(");
        break;
    case TOK_WHILE:
        aout("while(");
        break;
    case TOK_EQUAL:
        aout("==");
        break;
    case TOK_COMMA:
        aout(",");
        break;
    case TOK_ASSIGN:
        aout("=");
        break;
    case TOK_HASH:
        aout("!=");
        break;
    case TOK_LESSTHAN:
        aout("<");
        break;
    case TOK_GREATERTHAN:
        aout(">");
        break;
    case TOK_PLUS:
        aout("+");
        break;
    case TOK_MINUS:
        aout("-");
        break;
    case TOK_MULTIPLY:
        aout("*");
        break;
    case TOK_DIVIDE:
        aout("/");
        break;
    case TOK_LPAREN:
        aout("(");
        break;
    case TOK_RPAREN:
        aout(")");
        break;
    }
}

void
cg_end(void)
{
    aout("/* PL/0 compiler %s */\n", PL0C_VERSION);
}
