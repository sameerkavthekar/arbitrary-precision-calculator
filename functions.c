#include "functions.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

number *addNums(number *n1, number *n2) {
  int i = n1->size;
  int j = n2->size;
  if (i > j) {
    while (i > j) {
      pushToNumber(n2, 0);
      j++;
    }
  } else {
    while (j > i) {
      pushToNumber(n1, 0);
      i++;
    }
  }
  node *p = n1->tail;
  node *q = n2->tail;
  int carry = 0;
  number *sumNum;
  sumNum = (number *)malloc(sizeof(number));
  initNumber(sumNum);
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
  int i = n1->size;
  int j = n2->size;
  if (i > j) {
    while (i > j) {
      pushToNumber(n2, 0);
      j++;
    }
  } else {
    while (j > i) {
      pushToNumber(n1, 0);
      i++;
    }
  }
  number *diffNum = (number *)malloc(sizeof(number));
  initNumber(diffNum);
  node *p, *q;
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
    p = p->prev;
    q = q->prev;
  }
  removeTrailingZeros(diffNum);
  return diffNum;
}