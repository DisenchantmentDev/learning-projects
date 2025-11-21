// Complier for PL/O, a simplified version of Pascal, designed by the same person: Niklaus Wirth. 
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "strtonum.h"

#define TOK_IDENT 'I'
#define TOK_NUMBER 'N'
#define TOK_CONST 'C'
#define TOK_VAR 'V'
#define TOK_PROCEDURE 'P'
#define TOK_CALL 'c'
#define TOK_BEGIN 'B'
#define TOK_END 'E'
#define TOK_IF 'i'
#define TOK_THEN 'T'
#define TOK_WHILE 'W'
#define TOK_DO 'D'
#define TOK_ODD 'O'
#define TOK_DOT '.'
#define TOK_EQUAL '='
#define TOK_COMMA ','
#define TOK_SEMICOLON ';'
#define TOK_ASSIGN ':'
#define TOK_HASH '#'
#define TOK_LESSTHAN '<'
#define TOK_GREATERTHAN '>'
#define TOK_PLUS '+'
#define TOK_MINUS '-'
#define TOK_MULTIPLY '*'
#define TOK_DIVIDE '/'
#define TOK_LPAREN '('
#define TOK_RPAREN ')'

static size_t line = 1;
static char *raw, *token;
static int type;


/*
* File reading and prepping for lexing
*/

static void
error(const char *fmt, ...)
{
    va_list ap; //va_list is a struct for error function with variable # of inputs

    fprintf(stderr, "pl0c error: %lu: ", line); //print the line w/ error

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap); //functions that handle the values in va_list
    va_end(ap);

    fputc('\n', stderr);

    exit(1);
}

static void
readin(char *file)
{
    int fd; //file descripter of pl0 file
    struct stat st; //stat struct describes a struct which stores information about an open file. 
    //Specifics found here: https://pubs.opengroup.org/onlinepubs/009695299/basedefs/sys/stat.h.html
    
    if(strrchr(file, '.') == NULL) //strrchr() loactes the last occurance of a specific character in a string. 
        //In this case, checking if there is a single occurance of '.' in the file name
        error("file must end in .pl0");

    if(!!strcmp(strrchr(file, '.'), ".pl0")) //strcmp compares two strings. Checking to see if file properly ends in .pl0
        error("file must end in .pl0");

    if ((fd = open(file, O_RDONLY)) == -1) //checks to see if opening the given file descripter fails. If it does then error, otherwise stores the result in fd
        error("couldn't open %s", file);

    if (fstat(fd, &st) == -1) //get the stat of file. 
        error("couldn't get file size of given file");

    if((raw = malloc(st.st_size + 1)) == NULL) //attempt to malloc required memory based on the size of file stored in st
        error("mallco failed");

    if(read(fd, raw, st.st_size) != st.st_size)
        error("couldn't read %s", file);
    raw[st.st_size] = '\0'; //add end character to the raw text.

    close(fd);
}

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

static int
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

/*
* Parser. Takes the lexer's output and turns it into a string of tokens
* For now, just spits things out to stdout as a testing tool
*/

static void
parse_print(void)
{
    /* Loop that continues for as long as lex does not return 0 */
    while((type = lex()) != 0) {
        ++raw;
        /* Prints to stdout the line number and the type of the character */
        (void) fprintf(stdout, "%lu|%d\t", line, type);
        /* Lookup table so we know how to print out to stdout */
        switch (type) {
            case TOK_IDENT:
            case TOK_NUMBER:
            case TOK_CONST:
            case TOK_VAR:
            case TOK_PROCEDURE:
            case TOK_CALL:
            case TOK_BEGIN:
            case TOK_END:
            case TOK_IF:
            case TOK_THEN:
            case TOK_WHILE:
            case TOK_DO:
            case TOK_ODD:
                (void) fprintf(stdout, "%s", token); //standard formatted output to stream
                break;
            case TOK_DOT:
            case TOK_EQUAL:
            case TOK_COMMA:
            case TOK_SEMICOLON:
            case TOK_HASH:
            case TOK_LESSTHAN:
            case TOK_GREATERTHAN:
            case TOK_PLUS:
            case TOK_MINUS:
            case TOK_MULTIPLY:
            case TOK_DIVIDE:
            case TOK_LPAREN:
            case TOK_RPAREN:
                (void) fputc(type, stdout); //fputc for putting a single character into the stream
                break;
            case TOK_ASSIGN:
                (void) fputs(":=", stdout); //fputs for putting a null-terminated string into the stream
        }
        (void) fputc('\n', stdout);
    }
}

/*
 * Parser implementation and it's relevant helper functions.
 * - next(): controls the lexer by telling it when to move to next token
 * - expect(int): expects proper syntax depending on what is returned from lexer
 * - 
 */

static void
next(void)
{

    type = lex();
    ++raw;

}

static void
expect(int match)
{

    if (match != type)
        error("syntax error");

    next();
}

static void
parse(void)
{

    next();
    block();
    expect(TOK_DOT);

    if (type != 0)
        error("extra tokens at end of file");

}

/*
 * main
*/
int main(int argc, char *argv[])
{
    char *startp;

    if (argc !=2) {
        (void) fputs("usage: pl0c file.pl0\n", stderr);
        exit(1);
    }

    readin(argv[1]);
    startp = raw;

    parse_print();


    free(startp);

    return 0;
}






















