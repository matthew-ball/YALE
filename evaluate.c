#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"

expression *lookup(const char *symbol, expression *env) {
  expression *tmp = env;

  while (tmp != NULL && IS_LIST(tmp)) {
	expression *pair = LIST_HEAD(tmp);

	if (strcmp(SYMBOL_NAME(LIST_HEAD(pair)), symbol) == 0) {
	  return LIST_HEAD(LIST_TAIL(pair));
	}

	tmp = LIST_TAIL(tmp);
  }

  return nil;
}

expression *evaluate_function(expression *exp, expression *env) {
  if (IS_LAMBDA(LIST_HEAD(exp))) {
	return fn_lambda(exp, env);
  } else if (IS_FUNCTION(LIST_HEAD(exp))) {
	return (FUNCTION_NAME(LIST_HEAD(exp))->function)(LIST_TAIL(exp), env);
  } else {
	return exp;
  }
}

expression *evaluate_expression(expression *exp, expression *env) {
  if (exp == NULL) {
	return nil;
  } else if (IS_LIST(exp)) {
	if (IS_SYMBOL(LIST_HEAD(exp)) && strcmp(SYMBOL_NAME(LIST_HEAD(exp)), "LAMBDA") == 0) {
	  expression *lambda_args = LIST_HEAD(LIST_TAIL(exp));
	  expression *lambda_body = LIST_HEAD(LIST_TAIL(LIST_TAIL(exp)));

	  return lambda(lambda_args, lambda_body);
	} else {
	  expression *accum = list(evaluate_expression(LIST_HEAD(exp), env), NULL);

	  exp = LIST_TAIL(exp);
	  while (exp != NULL && IS_LIST(exp)) {
		append(accum, evaluate_expression(LIST_HEAD(exp), env));
		exp = LIST_TAIL(exp);
	  }

	  return evaluate_function(accum, env);
	}
  } else {
	expression *value = lookup(SYMBOL_NAME(exp), env);

	if (value == nil) {
	  return exp;
	} else {
	  return value;
	}
  }

  return nil;
}
