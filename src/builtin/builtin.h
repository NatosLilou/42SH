#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>

#include "expansion/expansion.h"

int echo(char **argv, FILE *sout);
int my_true(void);
int my_false(void);
int unset(char **argv);

#endif /* !BUILTIN_H */
