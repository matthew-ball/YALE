#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"
#include "reader.h"

int main(int argc, char *argv[]) {
  FILE *input = (argc > 1) ? fopen(argv[1], "r") : stdin;
  expression *environment = init_environment();

  do {
	printf("YALE> ");
	print_expression(evaluate_expression(read_expression(input), environment));
	printf("\n");
  } while (1);

  fclose(input);

  return EXIT_SUCCESS;
}
