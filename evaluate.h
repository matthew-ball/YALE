#ifndef EVALUATE_H
#define EVALUATE_H

expression *lookup(const char *symbol, expression *env);
expression *evaluate_function(expression *exp, expression *env);
expression *evaluate_expression(expression *exp, expression *env);

#endif
