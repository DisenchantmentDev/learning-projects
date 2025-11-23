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
#include "lex.h"
#include "parser.h"
#include "code_gen.h"

#include "main.h"

size_t line = 1;
char *raw, *token;
int depth, type;

struct symtab {
    int depth;
    int type;
    char *name;
    struct symtab *next;
};

struct symtab *head;

/*
* File reading and prepping for lexing
*/

void
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

// static void
// next(void)
// {
// 
//     type = lex();
//     ++raw;
// 
// }
// 
// static void
// expect(int match)
// {
// 
//     if (match != type)
//         error("syntax error");
// 
//     next();
// }
// 
// static void
// parse(void)
// {
// 
//     next();
//     block();
//     expect(TOK_DOT);
// 
//     if (type != 0)
//         error("extra tokens at end of file");
// 
// }

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
    //parse();


    free(startp);

    return 0;
}






















