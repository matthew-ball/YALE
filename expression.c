#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

expression *symbol(const char *name) {
  symbol_expression *ptr = malloc(sizeof(symbol_expression));
  MALLOC_CHECK(ptr);

  char *tmp = malloc(strlen(name) + 1);
  MALLOC_CHECK(tmp);

  ptr->type = SYMBOL;
  strcpy(tmp, name);
  ptr->name = tmp;

  return (expression *)ptr;
}

expression *list(expression *head, expression *tail) {
  list_expression *ptr = malloc(sizeof(list_expression));
  MALLOC_CHECK(ptr);

  ptr->type = LIST;
  ptr->head = head;
  ptr->tail = tail;

  return (expression *)ptr;
}

expression *function(expression *(*fn)(expression *, expression *)) {
  function_expression *ptr = malloc(sizeof(function_expression));
  MALLOC_CHECK(ptr);

  ptr->type = FUNCTION;
  ptr->function = fn;

  return (expression *)ptr;
}

expression *lambda(expression *args, expression *body) {
  lambda_expression *ptr = malloc(sizeof(lambda_expression));
  MALLOC_CHECK(ptr);

  ptr->type = LAMBDA;
  ptr->args = args;
  ptr->body = body;

  return (expression *)ptr;
}

void append(expression *exp, expression *object) {
  expression *ptr;

  for (ptr = exp; LIST_TAIL(ptr) != NULL; ptr = LIST_TAIL(ptr))
	;

  LIST_TAIL(ptr) = list(object, NULL);
}

void print_expression(const expression *exp) {
  if (exp == NULL) {
	return;
  } else if (IS_SYMBOL(exp)) {
	printf("%s", SYMBOL_NAME(exp));
  } else if (IS_LIST(exp)) {
	printf("(");
	print_expression(LIST_HEAD(exp));
	exp = LIST_TAIL(exp);
	while (exp != NULL && IS_LIST(exp)) {
	  printf(" ");
	  print_expression(LIST_HEAD(exp));
	  exp = LIST_TAIL(exp);
	}
	printf(")");
  } else if (IS_LAMBDA(exp)) {
	printf("@");
	print_expression(LAMBDA_ARGS(exp));
	print_expression(LAMBDA_BODY(exp));
  }
}
