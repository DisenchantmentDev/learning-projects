#pragma once

#include <stdio.h>

extern size_t line;
extern char *raw, *token;
extern int depth, type, proc;
extern struct symtab *head;

static void expression (void);
void parse (void);
