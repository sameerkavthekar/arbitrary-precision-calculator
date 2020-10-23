#define PLUS 1
#define MINUS -1

typedef struct node
{
    int data;
    struct node *next;
} node;

typedef struct number
{
    int size;
    node *head, *tail;
    int sign;
} number;

void initNumber(number *n);
void addToNumber(number *n, int d);
void printNum(number n);
void destroyNumber(number *n);
void pushToNumber(number *n, int d);
void remov(number *n, int position);