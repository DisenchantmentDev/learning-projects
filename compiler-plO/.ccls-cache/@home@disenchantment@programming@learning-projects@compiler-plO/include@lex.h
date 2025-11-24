#pragma once

#include <stdio.h>

extern size_t line;
extern char *raw, *token;
extern int type;

int lex();
