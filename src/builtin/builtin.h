#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdio.h>

#include "expansion/expansion.h"

int cd(char **argv);
int echo(char **argv, FILE *sout);
int my_true(void);
int my_false(void);
int my_export(char **argv);
int unset(char **argv);

#endif /* !BUILTIN_H */
