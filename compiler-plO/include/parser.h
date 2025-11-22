#pragma once

extern size_t line;
extern char *raw, *token;
extern int depth, type;

static void expression(void);
void parse(void);
