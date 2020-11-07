#define SIZE 2048
#include "functions.h"
#include "list.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

number *infixEval(char *exp);
int readline(char *line, int len);
void getArgs(int argc, char **argv);