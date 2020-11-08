#include "list.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Initialise the number
void initNumber(number *n) {
  n->head = NULL;
  n->tail = NULL;
  n->size = 0;
  n->sign = PLUS;
}

// This function appends data to end of given number
void addToNumber(number *n, int d) {
  node *p = (node *)malloc(sizeof(node));
  if (!p)
    return;
  p->data = d;
  p->next = NULL;
  p->prev = NULL;
  if (n->head == NULL) {
    n->size++;
    n->head = p;
    n->tail = p;
    return;
  }

  n->tail->next = p;
  p->prev = n->tail;
  n->tail = n->tail->next;
  n->size++;
  return;
}

// Function to print the number
void printNum(number n) {
  node *p = n.head;
  int i = 0;
  if (n.head == NULL) {
    printf("Number is empty\n");
    return;
  }
  if (n.sign == -1) {
    printf("-");
  }
  while (p) {
    printf("%d", p->data);
    p = p->next;
    i++;
    if (i > 50) {
      printf(" \\");
      printf("\n");
      i = 0;
    }
  }
  printf("\n");
  return;
}

// Function to go  through number and free all nodes
void destroyNumber(number *n) {
  node *p = n->head;
  if (p == NULL) {
    return;
  }
  node *q = NULL;
  while (p) {
    q = p->next;
    free(p);
    p = q;
  }
  n->head = n->tail = NULL;
  n->size = 0;
  n->sign = PLUS;
  return;
}

// Function to add node of given data to beginning of number
void pushToNumber(number *n, int d) {
  node *nn = (node *)malloc(sizeof(node));
  if (!nn)
    return;
  nn->data = d;
  nn->next = NULL;
  nn->prev = NULL;
  if (n->head == NULL) {
    n->size++;
    n->head = n->tail = nn;
    return;
  }
  nn->next = n->head;
  n->head->prev = nn;
  n->head = nn;
  n->size++;
  return;
}