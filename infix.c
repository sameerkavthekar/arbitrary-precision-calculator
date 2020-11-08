#include "infix.h"
#include <stdio.h>
#include <string.h>

// Function for calculating the precedence of given operator
int precedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/' || op == '%')
    return 2;
  if (op == '^')
    return 3;
  return 0;
}

// Function for applyting operation and returning number
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

// Different states of the infix evaluator. Will be used for error handling.
typedef enum states { OPERATOR, NUMBER, START, END } states;

// The infix evaluator
number *infixEval(char *exp) {

  // This is block of code initialises all
  // variables, stacks and numbers
  states s1 = START;
  int isRightAssociative = 0;
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

  // If the user types quit then exit the program
  if (strcmp(exp, "quit") == 0)
    exit(0);

  // Iterating through all possible cases of characters
  for (i = 0; i < len; i++) {
    if (exp[i] == ' ') {
      // If character is a space
      continue;
    } else if (exp[i] == '(') {
      // If character is opening bracket (
      cpush(&c, exp[i]);
    } else if (isdigit(exp[i])) {
      // If character is number

      // This case will come true if there occurs a number
      // after number. eg: 3 13
      if (s1 == NUMBER) {
        printf("Syntax error\n");
        return NULL;
      }

      // Reading the entire number inside this while loop and pushing
      // it to the number ADT
      while (i < len && isdigit(exp[i])) {
        addToNumber(n1, (exp[i] - '0'));
        i++;
      }

      // If there was a - sign before the number then this flag will be true
      // Hence making the sign of the number MINUS
      if (signFlag) {
        n1->sign = MINUS;
        signFlag = 0;
      }

      // Pushing the number to the number stack
      npush(&n, n1);
      n1 = (number *)malloc(sizeof(number));
      initNumber(n1);
      i--;
      s1 = NUMBER;

    } else if (exp[i] == ')') {
      // If character is closing bracket )

      // Keep popping the number stack twice and operator stack once and
      // evaluating till the opening brace ( is encountered
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
      // Pop the opening brace (
      if (!cisempty(c))
        cpop(&c);

    } else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' ||
               exp[i] == '/' || exp[i] == '%' || exp[i] == '^') {
      // If character is an operator

      // If a - character occurs after the evaluation has started or after
      // a operator r=then set the signFlag to 1
      // eg: This is used to evaluate expressions like
      // -2 + 3 or 2 + - 3
      if (exp[i] == '-' && (s1 == OPERATOR || s1 == START)) {
        signFlag = 1;
        continue;
      }

      // If an operator occurs immidiately after an operator the throw error
      // eg: 2 + + 3 or 13 * / 2
      if (s1 == OPERATOR) {
        printf("Syntax Error at %d(%c)\n", i, exp[i]);
        return NULL;
      }

      // If an exponent character is encountered then set the expression to
      // follow right associativity
      if (exp[i] == '^') {
        isRightAssociative = 1;
      } else {
        isRightAssociative = 0;
      }

      // If the expression is right associative then we dont evaluate it here
      if (!isRightAssociative) {
        // If the operator is is of less precedence than the the top operator in
        // the operator stack then evaluate the top operator
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
      }
      // Push operator into operator stack and set state to operator
      cpush(&c, exp[i]);
      s1 = OPERATOR;
    } else {
      // If any other character than the ones specified are encountered
      // Then throw an error. eg: 27 & 3 will throw error
      printf("Invalid symbol: %c\n", exp[i]);
      return NULL;
    }
  }

  // Expression should always end with a number else throw an error
  // Used to handle cases like 2 + 3 - or 3 * 2 %
  if (s1 != NUMBER) {
    printf("Syntax error at: %d(%c)\n", i, exp[i]);
    return NULL;
  }

  // Till the operator stack is not empty:
  // 1. Pop number stack twice
  // 2. Pop character stack once
  // 3. Evaluate and push result back onto number stack
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

  // These two functions are used everywhere in my program and are there
  // to make sure that every malloc() is accompanied by a free()
  destroyNumber(n1);
  destroyNumber(n2);
  free(n1);
  free(n2);

  s1 = END;

  // Return the number in the number stack which is our answer to the expression
  return npop(&n);
}

// This function is basically used to read a line from the standard input
// and break when EOF is occured
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

// Some info about the program
void printInfo() {
  printf("bc 1.07.1\n");
  printf("Made as a learning experience by Sameer Kavthekar 111903153\n");
  printf("This is free and under no warranty\n");
}

// This function basically handles the three flags that I have
// implemented in the program
void getArgs(int argc, char **argv) {
  // Only execute if number of arguments is more than 1
  if (argc > 1) {
    // Iterate through the argument vector
    for (int i = 1; i < argc; i++) {
      // -h is for usage
      // -q is for quiet i.e info will not be printed
      // -v is to print only info and exit
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
        // File handling

        // If a user provides any arguments other than flags we will treat them
        // as text file names. We try to open the file and evaluate all lines
        // inside the text file
        char exp[SIZE];
        FILE *fp;
        fp = fopen(argv[i], "r");

        // Opening the file fails
        if (!fp) {
          printf("File does not exist or does not have right permissions\n");
          exit(0);
        }

        // Read all lines of the file and execute each line one by one
        while (fscanf(fp, "%[^\n]%*c", exp) != EOF) {
          printf("-> ");
          number *n3;
          n3 = (number *)malloc(sizeof(number));
          initNumber(n3);
          n3 = infixEval(exp);
          if (n3)
            printNum(*n3);
          free(n3);
        }

        fclose(fp);
      }
    }
  }
  printInfo();
  return;
}