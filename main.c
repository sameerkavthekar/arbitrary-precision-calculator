#include "main.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char exp[SIZE];
  scanf("%[^\n]%*c", exp);
  number *n3;
  n3 = (number *)malloc(sizeof(number));
  initNumber(n3);
  n3 = infixEval(exp);
  if (n3)
    printNum(*n3);
  free(n3);
}