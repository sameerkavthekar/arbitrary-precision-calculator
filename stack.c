#include "stack.h"
#include "functions.h"
#include "list.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define CHAR_ERROR '0'

void ninitStack(n_stack *n) {
  *n = NULL;
  return;
}

void npush(n_stack *n, number *a) {
  n_node *nn = (n_node *)malloc(sizeof(n_node));
  if (!nn)
    return;
  nn->data = a;
  nn->next = NULL;
  if (*n == NULL) {
    *n = nn;
    return;
  } else {
    nn->next = *n;
    *n = nn;
    return;
  }
}

int nisEmpty(n_stack n) {
  if (n == NULL)
    return 1;
  return 0;
}

number *npop(n_stack *n) {
  if (nisEmpty(*n)) {
    return NULL;
  }
  n_node *p = *n;
  if (p->next == NULL) {
    number *a = p->data;
    *n = NULL;
    free(p);
    return a;
  }
  number *a = p->data;
  *n = p->next;
  free(p);
  return a;
}

number *npeek(n_stack n) {
  if (nisEmpty(n)) {
    return NULL;
  }
  return n->data;
}

void display(n_stack n) {
  n_node *p = n;
  while (p) {
    printNum(*(p->data));
    p = p->next;
  }
  printf("\n");
}

/*------------------------------------------------------------*/

void cinitStack(c_stack *c) {
  *c = NULL;
  return;
}

void cpush(c_stack *c, char data) {
  c_node *nn = (c_node *)malloc(sizeof(c_node));
  if (!nn)
    return;
  nn->data = data;
  nn->next = NULL;

  if (*c == NULL) {
    *c = nn;
    return;
  }

  nn->next = *c;
  *c = nn;
  return;
}

int cisempty(c_stack c) {
  if (c == NULL)
    return 1;
  return 0;
}

char cpop(c_stack *c) {
  char x = CHAR_ERROR;
  if (cisempty(*c))
    return x;
  c_node *p = *c;
  if (p->next == NULL) {
    *c = NULL;
    x = p->data;
    free(p);
    return x;
  }
  x = p->data;
  *c = p->next;
  free(p);
  return x;
}

char cpeek(c_stack c) {
  if (cisempty(c))
    return CHAR_ERROR;
  return c->data;
}