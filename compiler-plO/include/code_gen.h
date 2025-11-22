#pragma once

extern size_t line;
extern char *raw, *token;
extern int depth, type;

static void aout(const char *fmt, ...);
void cg_end(void);
