#pragma once

extern size_t line;
extern char *raw, *token;
extern int depth, type;

extern struct symtab;
extern struct symtab *head;

static void aout(const char *fmt, ...);
void cg_const(void);
void cg_semicolon(void);
void cg_crlf(void);
void cg_var(void);
void cg_procedure(void);
void cg_epilogue(void);
void cg_symbol(void);
void cg_odd(void);
void cg_end(void);

void symcheck(int);
void destroysymbols(void);
void addsymbol(int);
