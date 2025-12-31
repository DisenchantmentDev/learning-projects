#pragma once

#include <stdio.h>

extern size_t line;
extern char *raw, *token;
extern int depth, type, proc;
extern struct symtab *head;

void initsymtab (void);
static void expression (void);
void parse (void);
