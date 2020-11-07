#include "infix.h"
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

typedef enum states { OPERATOR, NUMBER, START, END } states;

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
  int len = strlen(exp);
  int i = 0;

  if (strcmp(exp, "exit") == 0)
    exit(0);

  for (i = 0; i < len; i++) {
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

      if (exp[i] == '-') {
        if (isdigit(exp[i + 1])) {
          signFlag = 1;
          continue;
        }
      }

      if (s1 == OPERATOR) {
        printf("Syntax Error at %d(%c)\n", i, exp[i]);
        return NULL;
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
    printf("Syntax error at: %d(%c)\n", i, exp[i]);
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
  free(n1);
  free(n2);

  s1 = END;

  return npop(&n);
}

int readline(char *line, int len) {
  int i;
  char ch;
  i = 0;
  while (i < len - 1) {
    ch = getchar();
    if (ch == EOF)
      return 0;
    if (ch == '\n') {
      line[i++] = '\0';
      return i - 1;
    } else
      line[i++] = ch;
  }
  line[len - 1] = '\0';
  return len - 1;
}

void printInfo() {
  printf("bc 1.07.1\n");
  printf("Made as a learning experience by Sameer Kavthekar 111903153\n");
  printf("This is free and under no warranty\n");
}

void getArgs(int argc, char **argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-h") == 0) {
        printf("usage: ./bc [options]\n");
        printf("-h\t\tprint usage and exit\n");
        printf("-q\t\tdon't print initial banner\n");
        printf("-v\t\tprint version information and exit\n");
        exit(0);
      } else if (strcmp(argv[i], "-q") == 0) {
        return;
      } else if (strcmp(argv[i], "-v") == 0) {
        printInfo();
        exit(0);
      } else {
        printf("Invalid Flag\n");
        exit(0);
      }
    }
  }
  printInfo();
  return;
}