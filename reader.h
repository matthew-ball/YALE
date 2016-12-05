#ifndef READER_H
#define READER_H

#define BUFFER_SIZE 1024

expression *next_token(FILE *input);
expression *read_tail(FILE *input);
expression *read_expression(FILE *input);

#endif
