#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

extern expression *t;
extern expression *nil;

expression *fn_quote(expression *args, expression *env);
expression *fn_head(expression *args, expression *env);
expression *fn_tail(expression *args, expression *env);
expression *fn_list(expression *args, expression *env);
expression *fn_equal(expression *args, expression *env);
expression *fn_symbol(expression *args, expression *env);
expression *fn_cond(expression *args, expression *env);
expression *create_parameter_pairs(expression *params, expression *args);
expression *replace_symbol(expression *exp, expression *pairs);
expression *fn_lambda(expression *args, expression *env);
expression *fn_label(expression *args, expression *env);
expression *init_environment();

#endif
