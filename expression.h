#ifndef EXPRESSION_H
#define EXPRESSION_H

#define MALLOC_CHECK(ptr) ({ if ((ptr) == NULL) { fprintf(stderr, "[%s] malloc failed\n", __func__); exit(EXIT_FAILURE); } })

typedef enum { SYMBOL, LIST, LAMBDA, FUNCTION } expression_type;

typedef struct {
  expression_type type;
} expression;

#define IS_SYMBOL(expression) ((expression)->type == SYMBOL)
#define IS_LIST(expression) ((expression)->type == LIST)
#define IS_LAMBDA(expression) ((expression)->type == LAMBDA)
#define IS_FUNCTION(expression) ((expression)->type == FUNCTION)

typedef struct {
  expression_type type;
  char *name;
} symbol_expression;

#define SYMBOL_NAME(symbol) (((symbol_expression *)(symbol))->name)

typedef struct {
  expression_type type;
  expression *head;
  expression *tail;
} list_expression;

#define LIST_HEAD(list) (((list_expression *)(list))->head)
#define LIST_TAIL(list) (((list_expression *)(list))->tail)

typedef struct {
  expression_type type;
  expression *args;
  expression *body;
} lambda_expression;

#define LAMBDA_ARGS(lambda) (((lambda_expression *)(lambda))->args)
#define LAMBDA_BODY(lambda) (((lambda_expression *)(lambda))->body)

typedef struct {
  expression_type type;
  expression *(*function)(expression *, expression *);
} function_expression;

#define FUNCTION_NAME(function) (((function_expression *)(function)))

expression *symbol(const char *name);
expression *list(expression *head, expression *tail);
expression *function(expression *(*fn)(expression *, expression *));
expression *lambda(expression *args, expression *body);
void append(expression *exp, expression *object);
void print_expression(const expression *exp);

#endif
