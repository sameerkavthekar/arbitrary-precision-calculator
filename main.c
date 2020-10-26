#include "functions.h"
#include "list.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 2048

int precedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  return 0;
}

number *applyOp(number *a, number *b, char op) {
  switch (op) {
  case '+':
    return addNums(a, b);
  case '-':
    return subNums(a, b);
    // case '*':
    //   return a * b;
    // case '/':
    //   return a / b;
  }
}

number *infixEval(char *exp) {
  number *n1, *n2;
  n1 = (number *)malloc(sizeof(number));
  n2 = (number *)malloc(sizeof(number));
  initNumber(n1);
  initNumber(n2);
  c_stack c;
  n_stack n;
  cinitStack(&c);
  ninitStack(&n);
  int len = strlen(exp);
  int i = 0;
  for (i = 0; i < len; i++) {
    if (exp[i] == ' ')
      continue;
    else if (exp[i] == '(') {
      cpush(&c, exp[i]);
    } else if (isdigit(exp[i])) {
      while (i < len && isdigit(exp[i])) {
        addToNumber(n1, (exp[i] - '0'));
        i++;
      }
      npush(&n, n1);
      n1 = (number *)malloc(sizeof(number));
      initNumber(n1);
      i--;
    } else if (exp[i] == ')') {
      while (!cisempty(c) && cpeek(c) != '(') {
        n1 = npop(&n);
        n2 = npop(&n);
        char op = cpop(&c);
        npush(&n, applyOp(n1, n2, op));
        n1 = (number *)malloc(sizeof(number));
        n2 = (number *)malloc(sizeof(number));
        initNumber(n1);
        initNumber(n2);
      }
      if (!cisempty(c))
        cpop(&c);
    } else {
      while (!cisempty(c) && precedence(cpeek(c)) >= precedence(exp[i])) {
        n1 = npop(&n);
        n2 = npop(&n);
        char op = cpop(&c);
        npush(&n, applyOp(n1, n2, op));
        n1 = (number *)malloc(sizeof(number));
        n2 = (number *)malloc(sizeof(number));
        initNumber(n1);
        initNumber(n2);
      }
      cpush(&c, exp[i]);
    }
  }

  while (!cisempty(c)) {
    n1 = npop(&n);
    n2 = npop(&n);
    char op = cpop(&c);
    npush(&n, applyOp(n1, n2, op));
    n1 = (number *)malloc(sizeof(number));
    n2 = (number *)malloc(sizeof(number));
    initNumber(n1);
    initNumber(n2);
  }
  free(n1);
  free(n2);

  return npop(&n);
}

int main() {
  char exp[SIZE];
  scanf("%[^\n]%*c", exp);
  number *n3;
  n3 = (number *)malloc(sizeof(number));
  initNumber(n3);
  n3 = infixEval(exp);
  printNum(*n3);
  free(n3);
}