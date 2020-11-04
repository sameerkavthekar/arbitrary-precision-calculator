#define SIZE 2048
#include "functions.h"
#include "list.h"
#include "stack.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int precedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/' || op == '%')
    return 2;
  if (op == '^')
    return 3;
  return 0;
}

number *applyOp(number *a, number *b, char op) {
  switch (op) {
  case '+':
    return addNums(a, b);
  case '-':
    return subNums(a, b, 1);
  case '*':
    return mulNums(a, b, 1);
  case '/':
    return divNums(a, b, 0);
  case '%':
    return divNums(a, b, 1);
  case '^':
    return power(a, b);
  }
  return NULL;
}

typedef enum states { OPERATOR, NUMBER, START } states;

number *infixEval(char *exp) {
  states s1 = START;
  number *n1, *n2;
  n1 = (number *)malloc(sizeof(number));
  n2 = (number *)malloc(sizeof(number));
  initNumber(n1);
  initNumber(n2);
  c_stack c;
  n_stack n;
  cinitStack(&c);
  ninitStack(&n);
  int signFlag = 0;
  int startFlag = 1;
  int len = strlen(exp);
  int i = 0;
  for (i = 0; i < len; i++) {
    if (i != 0) {
      startFlag = 0;
    }
    if (exp[i] == ' ') {
      continue;
    } else if (exp[i] == '(') {
      cpush(&c, exp[i]);
    } else if (isdigit(exp[i])) {
      if (s1 == NUMBER) {
        printf("Syntax error\n");
        return NULL;
      }
      while (i < len && isdigit(exp[i])) {
        addToNumber(n1, (exp[i] - '0'));
        i++;
      }
      if (signFlag) {
        n1->sign = MINUS;
        signFlag = 0;
      }
      npush(&n, n1);
      n1 = (number *)malloc(sizeof(number));
      initNumber(n1);
      i--;
      s1 = NUMBER;
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
    } else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' ||
               exp[i] == '/' || exp[i] == '%' || exp[i] == '^') {
      if (s1 == OPERATOR) {
        printf("Syntax Error\n");
        return NULL;
      }
      if (startFlag == 1 && exp[i] == '-') {
        if (isdigit(exp[i + 1])) {
          signFlag = 1;
          continue;
        }
      }
      while (cisempty(c) != 1 && precedence(cpeek(c)) >= precedence(exp[i])) {
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
      s1 = OPERATOR;
    } else {
      printf("Invalid symbol: %c\n", exp[i]);
      return NULL;
    }
  }

  if (s1 != NUMBER) {
    printf("Syntax error\n");
    return NULL;
  }

  while (cisempty(c) != 1) {
    n1 = npop(&n);
    n2 = npop(&n);
    char op = cpop(&c);
    npush(&n, applyOp(n1, n2, op));
    n1 = (number *)malloc(sizeof(number));
    n2 = (number *)malloc(sizeof(number));
    initNumber(n1);
    initNumber(n2);
  }
  destroyNumber(n1);
  destroyNumber(n2);

  return npop(&n);
}
