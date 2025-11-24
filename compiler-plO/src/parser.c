#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "lex.h"
#include "main.h"
// #include "code_gen.h"

#include "parser.h"

/*
 * Parser implementation and it's relevant helper functions.
 * - next(): controls the lexer by telling it when to move to next token
 * - expect(int): expects proper syntax depending on what is returned from lexer
 * - factor(): checks if left or right side of mathematical expression is a
 * value we can operate on
 * - term(): checks validity of both left and right terms
 * - expression(): checks validity of mathematical expression
 * - condition(): checks validity of conditional expression
 * - statement(): identifies full statements and checks validity
 * - block(): function that processes the given block of code
 * - parse(): function that is called by main to begin parsing
 */

static void next(void) {

  type = lex();
  ++raw;
}

/* Checks the passed token against what is currently in buffer */
static void expect(int match) {

  if (match != type)
    error("syntax error, expected token '%c'\n\tActually found '%c'",
          (char)match, (char)type);

  next();
}

static void factor(void) {

  switch (type) {
  case TOK_IDENT:
  case TOK_NUMBER:
    next();
    break;
  case TOK_LPAREN:
    expect(TOK_LPAREN);
    expression();
    expect(TOK_RPAREN);
  }
}

static void term(void) {
  factor();

  while (type == TOK_MULTIPLY || type == TOK_DIVIDE) {
    next();
    factor();
  }
}

static void expression(void) {

  if (type == TOK_PLUS || type == TOK_MINUS)
    next();

  term();

  while (type == TOK_PLUS || type == TOK_MINUS) {
    next();
    term();
  }
}

static void condition(void) {

  if (type == TOK_ODD) {
    expect(TOK_ODD);
    expression();
  } else {
    expression();

    switch (type) {
    case TOK_EQUAL:
    case TOK_HASH:
    case TOK_LESSTHAN:
    case TOK_GREATERTHAN:
      next();
      break;
    default:
      error("invalid condition");
    }

    expression();
  }
}

static void statement(void) {

  switch (type) {
  case TOK_IDENT:
    expect(TOK_IDENT);
    expect(TOK_ASSIGN);
    expression();
    break;
  case TOK_CALL:
    expect(TOK_CALL);
    expect(TOK_IDENT);
    break;
  case TOK_BEGIN:
    expect(TOK_BEGIN);
    statement();
    while (type == TOK_SEMICOLON) {
      expect(TOK_SEMICOLON);
      statement();
    }
    expect(TOK_END);
    break;
  case TOK_IF:
    expect(TOK_IF);
    condition();
    expect(TOK_THEN);
    statement();
    break;
  case TOK_WHILE:
    expect(TOK_WHILE);
    condition();
    expect(TOK_DO);
    statement();
  }
}

static void block(void) {

  if (depth++ > 3)
    error("nesting depth exceeded");

  if (type == TOK_CONST) {
    expect(TOK_CONST);
    //    if (type == TOK_IDENT)
    //      addsymbol(TOK_CONST);
    //    cg_const();
    expect(TOK_IDENT);
    expect(TOK_EQUAL);
    //    if (type == TOK_NUMBER) {
    //      cg_symbol();
    //      cg_semicolon();
    //    }
    expect(TOK_NUMBER);
    while (type == TOK_COMMA) {
      expect(TOK_COMMA);
      //      if (type == TOK_COMMA)
      //        addsymbol(TOK_CONST);
      //      cg_const();
      expect(TOK_IDENT);
      expect(TOK_EQUAL);
      //      if (type == TOK_NUMBER) {
      //        cg_symbol();
      //        cg_semicolon();
      //      }
      expect(TOK_NUMBER);
    }
    expect(TOK_SEMICOLON);
  }

  if (type == TOK_VAR) {
    expect(TOK_VAR);
    expect(TOK_IDENT);
    while (type == TOK_COMMA) {
      expect(TOK_COMMA);
      expect(TOK_IDENT);
    }
    expect(TOK_SEMICOLON);
  }

  while (type == TOK_PROCEDURE) {
    expect(TOK_PROCEDURE);
    expect(TOK_IDENT);
    expect(TOK_SEMICOLON);

    block();

    expect(TOK_SEMICOLON);
  }

  statement();

  if (--depth < 0)
    error("nesting depth less than 0");
}

void parse(void) {

  next();
  block();
  expect(TOK_DOT);

  if (type != 0)
    error("extra tokens at end of file");

  //  cg_end();
}
