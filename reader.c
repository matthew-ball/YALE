#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression.h"
#include "reader.h"

expression *next_token(FILE *input) {
  char buffer[BUFFER_SIZE];
  int ch, index = 0;

  ch = getc(input);

  while (isspace(ch)) {
	ch = getc(input);
  }

  if (ch == '\n') {
	ch = getc(input);
  }

  if (ch == EOF) {
	exit(EXIT_FAILURE);
  }

  if (ch == ')') {
	return symbol(")");
  }

  if (ch == '(') {
	return symbol("(");
  }

  while (!isspace(ch) && ch != ')') {
	buffer[index++] = ch;
	ch = getc(input);
  }

  buffer[index++] = '\0';

  if (ch == ')') {
	ungetc(ch, input);
  }

  return symbol(buffer);
}

expression *read_tail(FILE *input) {
  expression *token = next_token(input);

  if (strcmp(SYMBOL_NAME(token), ")") == 0) {
	return NULL;
  } else if (strcmp(SYMBOL_NAME(token), "(") == 0) {
	expression *head = read_tail(input);
	expression *tail  = read_tail(input);

	return list(head, tail);
  } else {
	expression *head = token;
	expression *tail  = read_tail(input);

	return list(head, tail);
  }
}

expression *read_expression(FILE *input) {
  expression *token = next_token(input);

  if (strcmp(SYMBOL_NAME(token), "(") == 0) {
	return read_tail(input);
  }

  return token;
}
