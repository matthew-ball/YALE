#include <stdlib.h>
#include <string.h>

#include "expression.h"
#include "evaluate.h"
#include "environment.h"

expression *t;
expression *nil;

expression *fn_quote(expression *args, expression *env) {
  return LIST_HEAD(args);
}

expression *fn_head(expression *args, expression *env) {
  expression *ptr = LIST_HEAD(args);

  return LIST_HEAD(ptr);
}

expression *fn_tail(expression *args, expression *env) {
  expression *ptr = LIST_HEAD(args);

  return LIST_TAIL(ptr);
}

expression *fn_list(expression *args, expression *env) {
  expression *ptr = list(LIST_HEAD(args), NULL);

  args = LIST_HEAD(LIST_TAIL(args));
  while (args != NULL && IS_LIST(args)) {
	append(ptr, LIST_HEAD(args));
	args = LIST_TAIL(args);
  }

  return ptr;
}

expression *fn_equal(expression *args, expression *env) {
  expression *exp1 = LIST_HEAD(args);
  expression *exp2 = LIST_HEAD(LIST_TAIL(args));

  if (strcmp(SYMBOL_NAME(exp1), SYMBOL_NAME(exp2)) == 0) {
	return t;
  } else {
	return nil;
  }
}

expression *fn_symbol(expression *args, expression *env) {
  expression *ptr = LIST_HEAD(args);

  if (IS_SYMBOL(ptr)) {
	return t;
  } else {
	return nil;
  }
}

expression *fn_cond(expression *args, expression *env) {
  while (args != NULL && IS_LIST(args)) {
	expression *cond_and_body = LIST_HEAD(args);
	expression *cond_result   = nil;
	expression *cond          = LIST_HEAD(cond_and_body);
	expression *body          = LIST_HEAD(LIST_TAIL(cond_and_body));

	if (cond != nil) {
	  cond_result = evaluate_expression(cond, env);
	}

	if (cond_result != nil) {
	  return evaluate_expression(body, env);
	}

	args = LIST_TAIL(args);
  }

  return NULL;
}

expression *create_parameter_pairs(expression *params, expression *args) {
  expression *param = LIST_HEAD(params);
  expression *arg   = LIST_HEAD(args);
  expression *pair  = list(param, list(arg, NULL));
  expression *pairs = list(pair, NULL);

  params = LIST_TAIL(params);
  args   = LIST_TAIL(args);

  while (params != NULL && IS_LIST(params)) {
	param = LIST_HEAD(params);
	arg   = LIST_HEAD(args);

	pair = list(param, list(arg, NULL));
	append(pairs, pair);

	params = LIST_TAIL(params);
	args   = LIST_TAIL(args);
  }

  return pairs;
}

expression *replace_symbol(expression *exp, expression *pairs) {
  if (IS_LIST(exp)) {
	expression *ptr = list(replace_symbol(LIST_HEAD(exp), pairs), NULL);

	exp = LIST_TAIL(exp);

	while (exp != NULL && IS_LIST(exp)) {
	  append(ptr, replace_symbol(LIST_HEAD(exp), pairs));
	  exp = LIST_TAIL(exp);
	}

	return ptr;
  } else {
	expression *tmp = pairs;

	while (tmp != NULL && IS_LIST(tmp)) {
	  expression *pair = LIST_HEAD(tmp);

	  if (strcmp(SYMBOL_NAME(LIST_HEAD(pair)), SYMBOL_NAME(exp)) == 0) {
		return LIST_HEAD(LIST_TAIL(pair));
	  }

	  tmp = LIST_TAIL(tmp);
	}

	return exp;
  }
}

expression *fn_lambda(expression *args, expression *env) {
  lambda_expression *lambda = (lambda_expression *)LIST_HEAD(args);
  expression *pair          = create_parameter_pairs(lambda->args, LIST_TAIL(args));
  expression *body          = replace_symbol(lambda->body, pair);

  return evaluate_expression(body, env);
}

expression *fn_label(expression *args, expression *env) {
  expression *arg   = LIST_HEAD(args);
  expression *name  = symbol(SYMBOL_NAME(arg));
  expression *value = LIST_HEAD(LIST_TAIL(args));

  append(env, list(name, list(value, NULL)));

  return t;
}

expression *init_environment() {
  expression *env;

#define FUNCTION_SYMBOL(name, func_ptr) (list(symbol((name)), list(function((func_ptr)), NULL)))

  env = list(FUNCTION_SYMBOL("QUOTE", &fn_quote), NULL);

  //append(env, FUNCTION_SYMBOL("PRINT", &fn_print));
  append(env, FUNCTION_SYMBOL("HEAD", &fn_head));
  append(env, FUNCTION_SYMBOL("TAIL", &fn_tail));
  append(env, FUNCTION_SYMBOL("LIST", &fn_list));
  append(env, FUNCTION_SYMBOL("EQUAL", &fn_equal));
  append(env, FUNCTION_SYMBOL("SYMBOL", &fn_symbol));
  append(env, FUNCTION_SYMBOL("COND", &fn_cond));
  append(env, FUNCTION_SYMBOL("LAMBDA", &fn_lambda));
  append(env, FUNCTION_SYMBOL("LABEL", &fn_label));

#undef FUNCTION_SYMBOL

  t = symbol("#T");
  nil = list(NULL, NULL);

  return env;
}
