#include "functions.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

void equaliseNums(number **n1, number **n2) {
  int i = (*n1)->size;
  int j = (*n2)->size;
  if (i > j) {
    while (i > j) {
      pushToNumber(*n2, 0);
      (*n2)->size++;
      j++;
    }
  } else {
    while (j > i) {
      pushToNumber(*n1, 0);
      (*n1)->size++;
      i++;
    }
  }
}

number *addNums(number *n1, number *n2) {
  equaliseNums(&n1, &n2);
  node *p = n1->tail;
  node *q = n2->tail;
  int carry = 0;
  number *sumNum;
  sumNum = (number *)malloc(sizeof(number));
  initNumber(sumNum);
  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    sumNum = subNums(n2, n1);
    return sumNum;
  }
  while (p && q) {
    int sum = p->data + q->data + carry;
    carry = sum / 10;
    pushToNumber(sumNum, sum % 10);
    sumNum->size++;
    p = p->prev;
    q = q->prev;
  }
  if (carry != 0) {
    pushToNumber(sumNum, carry);
    sumNum->size++;
  }
  return sumNum;
}

void removeTrailingZeros(number *n) {
  if (n->head == NULL) {
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
  return;
}

number *subNums(number *n1, number *n2) {
  equaliseNums(&n1, &n2);
  number *diffNum = (number *)malloc(sizeof(number));
  initNumber(diffNum);
  node *p, *q;
  if (n1->sign != n2->sign) {
    n1->sign = n2->sign;
    diffNum = addNums(n1, n2);
    diffNum->sign = MINUS;
    return diffNum;
  }
  if (n1->head->data > n2->head->data) {
    q = n1->tail;
    p = n2->tail;
    diffNum->sign = MINUS;
  } else {
    p = n1->tail;
    q = n2->tail;
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
    diffNum->size++;
    p = p->prev;
    q = q->prev;
  }
  removeTrailingZeros(diffNum);
  return diffNum;
}

number *mulNums(number *n1, number *n2) {
  number *mulNum = (number *)malloc(sizeof(number));
  initNumber(mulNum);
  if (n1->size > n2->size) {
    mulNum = mulNums(n2, n1);
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
    mulNum->size++;
  }
  removeTrailingZeros(mulNum);
  return mulNum;
}
