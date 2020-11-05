#include "functions.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}

int isNumberZero(number *n1) {
  node *p = n1->head;
  while (p) {
    if (p->data != 0)
      return 0;
    p = p->next;
  }
  return 1;
}

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

  if (n1->sign == MINUS && n2->sign == MINUS)
    sumNum->sign = MINUS;

  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    sumNum = subNums(n2, n1, 1);
    return sumNum;
  }

  while (p && q) {
    int sum = p->data + q->data + carry;
    carry = sum / 10;
    pushToNumber(sumNum, sum % 10);
    if (p)
      p = p->prev;
    if (q)
      q = q->prev;
  }

  if (carry != 0) {
    pushToNumber(sumNum, carry);
  }

  return sumNum;
}

number *subNums(number *n1, number *n2, int freeNums) {
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

  number *diffNum = (number *)malloc(sizeof(number));
  initNumber(diffNum);
  node *p, *q;

  if (compare(n2, n1) == 1) {
    p = n1->tail;
    q = n2->tail;
  } else if (compare(n1, n2) == 1) {
    q = n1->tail;
    p = n2->tail;
    diffNum->sign = MINUS;
  } else if (compare(n1, n2) == 0) {
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

  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    diffNum = addNums(n1, n2);
    return diffNum;
  }

  int borrow = 0;

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

number *mulNums(number *n1, number *n2, int freeNums) {
  number *mulNum = (number *)malloc(sizeof(number));
  initNumber(mulNum);
  equaliseNums(&n1, &n2);
  if (n1->sign != n2->sign) {
    mulNum->sign = MINUS;
  }
  if (isNumberZero(n1) || isNumberZero(n2)) {
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
    addToNumber(mulNum, 0);
    return mulNum;
  }
  int a = n1->size;
  int b = n2->size;
  int i, j, a1 = 0, a2 = 0;
  int temp_result[2 * a];
  for (i = 0; i < 2 * a; i++)
    temp_result[i] = 0;
  int k = 2 * a - 1;
  node *t2 = n2->tail;
  for (i = 0; i < b; i++) {
    node *t1 = n1->tail;
    int carry1 = 0, carry2 = 0;
    for (j = k - i; j > a - 2; j--) {
      if (t1 != NULL && t2 != NULL) {
        a1 = t1->data * t2->data + carry1;
        t1 = t1->prev;
        carry1 = a1 / 10;
        a1 = a1 % 10;
        a2 = temp_result[j] + a1 + carry2;
        carry2 = a2 / 10;
        a2 = a2 % 10;
        temp_result[j] = a2;
      } else {
        break;
      }
    }
    temp_result[j] = carry1 + carry2 + temp_result[j];
    a--;
    t2 = t2->prev;
  }
  for (i = k; i >= a - 1 && i >= 0; i--) {
    pushToNumber(mulNum, temp_result[i]);
  }
  if (freeNums) {
    destroyNumber(n1);
    destroyNumber(n2);
    free(n1);
    free(n2);
  }
  removeTrailingZeros(mulNum);
  return mulNum;
}

number *divNums(number *n1, number *n2, int returnRemainderOrQuotient) {
  // Long Division Algorithm
  // n1 is the divisor and n2 is the number. n2/n1
  number *temp = (number *)malloc(sizeof(number));
  number *temp2 = NULL;
  number *Q = (number *)malloc(sizeof(number));
  initNumber(temp);
  initNumber(Q);
  int i = 0;
  node *q = n2->head;

  if (isNumberZero(n1)) {
    printf("ERROR: Cannot divide with zero\n");
    exit(0);
  }

  int Q_sign = PLUS;
  int temp_sign = PLUS;

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

  pushToNumber(temp, q->data);

  while (compare(temp, n1) == -1) {
    i++;
    q = q->next;
    addToNumber(temp, q->data);
  }

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
  if (Q->size == 0)
    pushToNumber(Q, 0);
  destroyNumber(n2);
  free(n2);
  if (returnRemainderOrQuotient == 0) {
    destroyNumber(temp);
    free(temp);
    Q->sign = Q_sign;
    return Q;
  } else {
    if (compare(temp, n1) == 0) {
      destroyNumber(n1);
      free(n1);
      destroyNumber(temp);
      pushToNumber(temp, 0);
      return temp;
    }
    destroyNumber(Q);
    free(Q);
    removeTrailingZeros(temp);
    temp->sign = temp_sign;
    return temp;
  }
}

number *power(number *n1, number *n2) {
  number *pow = (number *)malloc(sizeof(number));
  number *temp = (number *)malloc(sizeof(number));
  number *temp2 = NULL;
  initNumber(pow);

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

  copyNumber(n2, temp);

  while (compare(n1, UnityNumber) != 0) {
    pow = mulNums(temp, n2, 0);
    destroyNumber(temp);
    copyNumber(pow, temp);
    temp2 = n1;
    n1 = subNums(UnityNumber, n1, 0);
    destroyNumber(temp2);
  }
  destroyNumber(n1);
  destroyNumber(n2);
  destroyNumber(temp);
  free(n1);
  free(n2);
  free(temp);
  return pow;
}

number *compareNums(number *n1, number *n2) {}
