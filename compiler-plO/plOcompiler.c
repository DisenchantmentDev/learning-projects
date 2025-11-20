// Complier for PL/O, a simplified version of Pascal, designed by the same person: Niklaus Wirth. 
#include <stdio.h>
#include <stdlib.h>

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

char *raw;

int main(int argc, char *argv[])
{
    char *startp;

    if (argc !=2) {
        (void) fputs("usage: plOc file.plO\n", stderr);
        exit(1);
    }

    readin(argv[1]);
    startp = raw;

    parse();


    free(startp);

    return 0;
}

/*
* File reading and prepping for lexing
*/

static size_t line = 1;
static char *raw, *token;
static int type;

static void
error(const char *fmt, ...)
{
    va_list ap; //va_list is a struct for error function with variable # of inputs

    fprintf(stderr, "plOc error: %lu: ", line); //print the line w/ error

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap); //functions that handle the values in va_list
    va_end(ap);

    fputc('\n', stderr);

    exit(1);
}

static void
readin(char *file)
{
    int fd; //file descripter of plO file
    struct stat st; //stat struct describes a struct which stores information about an open file. 
    //Specifics found here: https://pubs.opengroup.org/onlinepubs/009695299/basedefs/sys/stat.h.html
    
    if(strrchr(file, '.') == NULL) //strrchr() loactes the last occurance of a specific character in a string. 
        //In this case, checking if there is a single occurance of '.' in the file name
        error("file must end in .plO");

    if(!!strcmp(strrchr(file, '.'), ".plO"))) //strcmp compares two strings. Checking to see if file properly ends in .plO
        error("file must end in .plO");

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
*/

static void
comment (void) //method for lexing past comments
{
    return;
}

static int
ident(void)
{
    return 0;
}

static int
number(void)
{
    return 0;
}

static int
lex(void) //lexer function. Just a big switch/case checking the token of each character
{
    return 0;
}

/*
* parser
*/

static void
parse(void)
{
    return;
}
























