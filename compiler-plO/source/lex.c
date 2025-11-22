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
#include "strtonum.h"
#include "parser.h"

#include "lex.h"
/*
* Lexer
* All functions that relate to taking in raw input from file and
* translating that to tokens which can be translated into output code.
* - Comment(): function for dealing with commented out code
* - number(): function which handles numerical values. Uses strtonum.c
* - lex(): the actual lexer. Skips over white space, checks for ident, number, and comment
*       outputs the token into *raw which is read by the parser.
*/

static void
comment (void) //method for lexing past comments
{
    /*
     * Only currently has functionality for { ... } comments. pl0 also should support
     * comments with the {* ... *} format. TODO later. 
     * Comments can be nested, and you can mix and match opening and closing comment characters.
     */
    int ch;

    while ((ch = *raw++) != '}') { //loops to check if the next character is an ending comment character
        if (ch == '\0') //if we hit the end of the file, a comment was not properly closed
            error("unterminated comment");

        if (ch == '\n') //move to next line if we hit a new line
            ++line;
    }
}

static int
ident(void)
{
    char *p;
    size_t i, len;

    p = raw;
    /* iterate over the source code until we run into a character that is not a
     * letter, number, or underscore, generating a word that way. */
    while (isalnum(*raw) || *raw == '_')
        ++raw;

    len = raw - p;

    --raw; //revert back a character so we don't skip a token

    free(token); //free token from memory just in case it's been used before

    if ((token = malloc(len + 1)) == NULL)
        error("malloc failed in token ident");

    for (i = 0; i < len; i++)
        token[i] = *p++;
    token[i] = '\0';

    /* Table that checks if our token is a reserved word. If not, then it's an identifier*/
    if(!strcmp(token, "const"))
        return TOK_CONST;
    else if (!strcmp(token, "var"))
            return TOK_VAR;
    else if (!strcmp(token, "procedure"))
            return TOK_PROCEDURE;
    else if (!strcmp(token, "call"))
            return TOK_CALL;
    else if (!strcmp(token, "begin"))
            return TOK_BEGIN;
    else if (!strcmp(token, "end"))
            return TOK_END;
    else if (!strcmp(token, "if"))
            return TOK_IF;
    else if (!strcmp(token, "then"))
            return TOK_THEN;
    else if (!strcmp(token, "while"))
            return TOK_WHILE;
    else if (!strcmp(token, "do"))
            return TOK_DO;
    else if (!strcmp(token, "odd"))
            return TOK_ODD;

    return TOK_IDENT;
}

static int
number(void)
{
    const char *errstr;
    char *p;
    size_t i, j = 0, len;

    p = raw;
    /* Allowing the user to use an underscore as a decimal seperator. */
    while (isdigit(*raw) || *raw == '_')
        ++raw;

    len = raw - p;

    --raw;

    free(token);

    if((token = malloc(len + 1)) == NULL)
        error("malloc failed in number toden ident");

    for (i = 0; i < len; i++) {
        if (isdigit(*p))
            token[j++] = *p;
        ++p;
    }
    token[j] = '\0';

    /* strtonum is a function found in OpenBSD but is included in this project
     * as well. We cast the result to void because we don't actually care about
     * the result, we just want to verify the token. */
    (void) strtonum(token, 0, LONG_MAX, &errstr);
    if (errstr != NULL) 
        error("invalid number: %s", token);

    return TOK_NUMBER;
}

int
lex(void) //lexer function. Just a big switch/case checking the token of each character
{

again:
    /* Skipping over whitespace */
    while (*raw == ' ' || *raw == '\t' || *raw == '\n') {
        if(*raw++ == '\n')
            ++line;
    }

    /* Checks if a alphanumeric token is a reserved word or identifier.*/
    if(isalpha(*raw) || *raw == '_') 
        return ident();

    /* Simply lexes numbers. We also allow for underscores to seperate numbers. */
    if (isdigit(*raw)) 
        return number();

     switch (*raw){
        case '{':
            comment();
            goto again;
        case '.':
        case '=':
        case ',':
        case ';':
        case '#':
        case '<':
        case '>':
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
            return (*raw);
        case ':':
            if (*++raw != '=')
                error("unknown token: ':%c'", *raw);
            return TOK_ASSIGN;
        case '\0':
            return 0;
        default:
            error("unknown token: '%c'", *raw);
    }
    return 0;
}
