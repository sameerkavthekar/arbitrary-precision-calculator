#include "list.h"

typedef struct n_node {
  number *data;
  struct n_node *next;
} n_node;

typedef n_node *n_stack;

void ninitStack(n_stack *s);
void npush(n_stack *, number *);
number *npop(n_stack *);
int nisEmpty(n_stack);
number *npeek(n_stack);
void display(n_stack n);

/*----------------------------------------------------------*/

typedef struct c_node {
  char data;
  struct c_node *next;
} c_node;

typedef c_node *c_stack;

void cinitStack(c_stack *c);
void cpush(c_stack *c, char data);
char cpop(c_stack *c);
int cisempty(c_stack c);
char cpeek(c_stack c);