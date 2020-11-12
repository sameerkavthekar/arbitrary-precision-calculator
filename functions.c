#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOTE: You might see the frequent use of destroyNumber() and free()
// This is to ensure that there is absolutely no garbage memory caused
// in the program.
// I am freeing all numbers input to the function unless explicitly stated

// Equalise the length of both numbers:
// eg: 23 and 3 will become 23 and 03
void equaliseNums(number **n1, number **n2) {
  int i = (*n1)->size;
  int j = (*n2)->size;
  if (i > j) {
    while (i > j) {
      pushToNumber(*n2, 0);
      j++;
    }
  } else {
    while (j > i) {
      pushToNumber(*n1, 0);
      i++;
    }
  }
  return;
}

// Function to check if the given number is zero
// eg: 000000 will return true
int isNumberZero(number *n1) {
  node *p = n1->head;
  while (p) {
    if (p->data != 0)
      return 0;
    p = p->next;
  }
  return 1;
}

// Function to remove trailing zeros from number
// eg: 0000045 will become 45
void removeTrailingZeros(number *n) {
  if (n->head == NULL) {
    return;
  }
  if (isNumberZero(n)) {
    destroyNumber(n);
    addToNumber(n, 0);
    return;
  }
  node *p = n->head;
  node *q = NULL;
  while (p->data == 0) {
    q = p->next;
    free(p);
    p = q;
    n->size--;
  }
  n->head = p;
  n->head->prev = NULL;
  return;
}

// Function to compare two numbers based on size and value
// Returns 1 if n1 > n2
// Returns -1 if n2 > n2
// Returns 0 if n1 == n2
int compare(number *n1, number *n2) {
  node *p = n1->head;
  node *q = n2->head;
  if (n1->size > n2->size) {
    return 1;
  } else if (n2->size > n1->size) {
    return -1;
  } else {
    while (p && q) {
      if (p->data > q->data)
        return 1;
      else if (q->data > p->data) {
        return -1;
      }
      p = p->next;
      q = q->next;
    }
  }
  return 0;
}

// END OF UTILITY FUNCTIONS
/*----------------------------------------------------------------------------------*/

// NOTE: In all functions n1 is second number and n2 is first number
// eg: 12 + 3;
// n1 is 3 and n2 is 12

// Function two add two numbers
number *addNums(number *n1, number *n2) {

  removeTrailingZeros(n1);
  removeTrailingZeros(n2);

  if (isNumberZero(n1)) {
    destroyNumber(n1);
    free(n1);
    return n2;
  } else if (isNumberZero(n2)) {
    destroyNumber(n2);
    free(n2);
    return n1;
  }

  equaliseNums(&n1, &n2);
  node *p = n1->tail;
  node *q = n2->tail;
  int carry = 0;
  number *sumNum;
  sumNum = (number *)malloc(sizeof(number));
  initNumber(sumNum);

  // If both numbers are negative then final number also negative
  if (n1->sign == MINUS && n2->sign == MINUS)
    sumNum->sign = MINUS;

  // If the sign of both numbers differ then subtract them
  // eg: -12 + 3 will give output -9
  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    sumNum = subNums(n1, n2, 1);
    return sumNum;
  }

  // Iterate both numbers from end and add digits and carry
  while (p && q) {
    int sum = p->data + q->data + carry;
    carry = sum / 10;
    pushToNumber(sumNum, sum % 10);
    if (p)
      p = p->prev;
    if (q)
      q = q->prev;
  }

  // Push the carry if it exists
  if (carry != 0) {
    pushToNumber(sumNum, carry);
  }

  return sumNum;
}

// Function to subtract two numbers
number *subNums(number *n1, number *n2, int freeNums) {
  removeTrailingZeros(n1);
  removeTrailingZeros(n2);

  // If either number is zero return
  if (isNumberZero(n1)) {
    destroyNumber(n1);
    free(n1);
    return n2;
  } else if (isNumberZero(n2)) {
    n1->sign = MINUS;
    destroyNumber(n2);
    free(n2);
    return n1;
  }

  number *diffNum = (number *)malloc(sizeof(number));
  initNumber(diffNum);
  node *p, *q;

  // If the signs arent equal then add the numbers and set the sign of result to
  // negative
  // eg: -12 - 13 will give -25
  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    diffNum = addNums(n1, n2);
    return diffNum;
  }

  // Compare and appropriatly set p & q pointers
  if (compare(n2, n1) == 1) {
    p = n1->tail;
    q = n2->tail;
  } else if (compare(n1, n2) == 1) {
    q = n1->tail;
    p = n2->tail;
    diffNum->sign = MINUS;
  } else if (compare(n1, n2) == 0) {
    // Special case.
    // If both numbers are equal then result is zero
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    addToNumber(diffNum, 0);
    return diffNum;
  }

  equaliseNums(&n1, &n2);

  if (n1->sign == MINUS && n2->sign == MINUS) {
    if (compare(n1, n2) == 1)
      diffNum->sign = PLUS;
    else
      diffNum->sign = MINUS;
  }

  int borrow = 0;

  // Iterate over p and q and subtract each digit
  // and keep note of borrow
  while (p && q) {
    int sub = q->data - p->data - borrow;
    if (sub < 0) {
      sub = sub + 10;
      borrow = 1;
    } else
      borrow = 0;
    pushToNumber(diffNum, sub);
    p = p->prev;
    q = q->prev;
  }

  // If the freeNums flag is set as true then only free both numbers else dont.
  if (freeNums) {
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
  } else {
    removeTrailingZeros(n1);
    removeTrailingZeros(n2);
  }

  removeTrailingZeros(diffNum);
  return diffNum;
}

// Program to multiply two numbers.
// This algorithm is based on the multiplication technique we do on paper.
number *mulNums(number *n1, number *n2, int freeNums) {

  number *mulNum = (number *)malloc(sizeof(number));
  initNumber(mulNum);
  int m_sign = PLUS;

  // Sign checking
  if (n1->sign != n2->sign) {
    m_sign = MINUS;
  }

  // If any of the numbers is zero, return zero
  if (isNumberZero(n1) || isNumberZero(n2)) {
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    addToNumber(mulNum, 0);
    return mulNum;
  }

  pushToNumber(mulNum, 0);
  node *p = n2->tail;
  int i = 0;

  while (p) {
    node *q = n1->tail;
    number *temp = (number *)malloc(sizeof(number));
    initNumber(temp);
    int carry = 0;

    while (q) {
      int mul = 0;
      mul = q->data * p->data + carry;
      int num = mul % 10;
      carry = mul / 10;
      pushToNumber(temp, num);
      q = q->prev;
    }

    if (carry != 0) {
      pushToNumber(temp, carry);
    }

    for (int j = 0; j < i; j++) {
      addToNumber(temp, 0);
    }

    // Here we need not call free on temp as addNums() natively frees and
    // destroys the two numbers
    mulNum = addNums(mulNum, temp);
    p = p->prev;
    i++;
  }

  if (freeNums) {
    destroyNumber(n2);
    free(n2);
  }

  destroyNumber(n1);
  free(n1);

  mulNum->sign = m_sign;
  return mulNum;
}

// Function to calculate division and modulus of two numbers
// Long Division Algorithm
// n1 is the divisor and n2 is the number. n2/n1 or n2%n1
number *divNums(number *n1, number *n2, int returnRemainderOrQuotient) {

  number *temp = (number *)malloc(sizeof(number));
  number *temp2 = NULL;
  number *Q = (number *)malloc(sizeof(number));
  initNumber(temp);
  initNumber(Q);
  int i = 0;
  node *q = n2->head;

  // If the divisor is zero then exit program with error
  if (isNumberZero(n1)) {
    printf("ERROR: Cannot divide with zero\n");
    exit(0);
  }

  int Q_sign = PLUS;
  int temp_sign = PLUS;

  // Sign checking
  switch (n1->sign) {
  case PLUS:
    switch (n2->sign) {
    case PLUS:
      temp_sign = PLUS;
      Q_sign = PLUS;
      break;
    case MINUS:
      Q_sign = MINUS;
      temp_sign = PLUS;
      break;
    }
    break;
  case MINUS:
    switch (n2->sign) {
    case PLUS:
      Q_sign = MINUS;
      temp_sign = MINUS;
      break;
    case MINUS:
      Q_sign = PLUS;
      temp_sign = MINUS;
      break;
    }
    break;
  }

  n1->sign = PLUS;
  n2->sign = PLUS;

  if (compare(n1, n2) == 0) {
    // If both numbers are equal then return 1 as quotient
    // or 0 as remainder
    if (returnRemainderOrQuotient == 1) {
      pushToNumber(Q, 0);
      free(temp);
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      Q->sign = Q_sign;
      return Q;
    }
    pushToNumber(Q, 1);
    free(temp);
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    Q->sign = Q_sign;
    return Q;
  } else if (compare(n1, n2) == 1) {
    // Since we dont have floats, if the divisor is greater
    // than the dividend then we return 0 as quotient and the
    // dividend as the remainder
    // eg: 123 / 456 = 0 and 123 % 456 = 123
    if (returnRemainderOrQuotient == 1) {
      node *p = n2->head;
      while (p) {
        addToNumber(Q, p->data);
        p = p->next;
      }
      destroyNumber(n1);
      destroyNumber(n2);
      free(n1);
      free(n2);
      free(temp);
      Q->sign = Q_sign;
      return Q;
    }
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    pushToNumber(Q, 0);
    free(temp);
    Q->sign = Q_sign;
    return Q;
  }

  // Algorithm starts
  pushToNumber(temp, q->data);

  // If n1(divisor) is more than temp then append the next digit of n2 to temp
  while (compare(temp, n1) == -1) {
    i++;
    q = q->next;
    addToNumber(temp, q->data);
  }

  // Keep iterating till i is less than size of dividend
  while (n2->size > i) {
    int j = 0;
    if (!isNumberZero(temp)) {
      int k = compare(temp, n1);
      while (k != -1) {
        temp2 = temp;
        temp = subNums(n1, temp, 0);
        destroyNumber(temp2);
        free(temp2);
        j++;
        k = compare(temp, n1);
        if (k == 0) {
          j++;
          break;
        }
      }
    }
    ++i;
    j = j % 10;
    addToNumber(Q, j);
    q = q->next;
    if (q)
      addToNumber(temp, q->data);
  }

  // If quotient is zero then push 0 to Q
  if (Q->size == 0)
    pushToNumber(Q, 0);
  // We destroy the divisor as we no longer need it
  destroyNumber(n2);
  free(n2);
  if (returnRemainderOrQuotient == 0) {
    // Return quotient
    destroyNumber(temp);
    destroyNumber(n1);
    free(temp);
    free(n1);
    Q->sign = Q_sign;
    return Q;
  } else {
    // Return remainder
    if (compare(temp, n1) == 0) {
      destroyNumber(n1);
      free(n1);
      destroyNumber(temp);
      pushToNumber(temp, 0);
      return temp;
    }
    destroyNumber(Q);
    destroyNumber(n1);
    free(Q);
    temp->sign = temp_sign;
    return temp;
  }
}

// Function for power
// This is implemented using repeated multiplication
number *power(number *n1, number *n2) {

  number *pow = (number *)malloc(sizeof(number));
  number *temp = (number *)malloc(sizeof(number));
  initNumber(pow);
  int pow_sign = PLUS;

  // Sign check
  if (n2->sign == MINUS)
    pow_sign = MINUS;

  // Zero check
  if (isNumberZero(n1)) {
    addToNumber(pow, 1);
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    return pow;
  } else if (isNumberZero(n2)) {
    addToNumber(pow, 0);
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    return pow;
  }

  // If sign of exponent is zero then output is 1
  // eg: 134 ^ 0 is 1
  if (n1->sign == MINUS) {
    pushToNumber(pow, 0);
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    return pow;
  }

  number *UnityNumber = (number *)malloc(sizeof(number));
  initNumber(UnityNumber);
  pushToNumber(UnityNumber, 1);

  if (compare(n1, UnityNumber) == 0) {
    destroyNumber(n1);
    destroyNumber(UnityNumber);
    free(UnityNumber);
    free(temp);
    free(n1);
    return n2;
  }

  if (compare(n2, UnityNumber) == 0) {
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    free(temp);
    return UnityNumber;
  }

  addToNumber(pow, 1);

  while (!isNumberZero(n1)) {
    pow = mulNums(pow, n2, 0);
    n1 = subNums(UnityNumber, n1, 0);
  }

  destroyNumber(n1);
  destroyNumber(n2);
  destroyNumber(temp);
  free(n1);
  free(n2);
  free(temp);

  pow->sign = pow_sign;
  return pow;
}