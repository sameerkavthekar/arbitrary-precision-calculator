#include "functions.h"
#include <stdio.h>
#include <string.h>

void infixEval(char *exp) {
  int i = 0;
  while (exp[i] != '\0') {
  }
}

int main() {
  number n;
  number n1;
  number n2;
  initNumber(&n);
  initNumber(&n1);
  initNumber(&n2);
  addToNumber(&n1, 9);
  addToNumber(&n1, 9);
  addToNumber(&n1, 9);
  printNum(n1);
  addToNumber(&n2, 9);
  addToNumber(&n2, 9);
  addToNumber(&n2, 9);
  printNum(n2);
  n = addNums(n1, n2);
  printNum(n);
}
