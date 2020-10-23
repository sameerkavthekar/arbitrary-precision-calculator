#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "list.h"

void initNumber(number *n)
{
    n->head = NULL;
    n->tail = NULL;
    n->size = 0;
    n->sign = PLUS;
}

void addToNumber(number *n, int d)
{
    node *p = (node *)malloc(sizeof(node));
    if (!p)
        return;
    p->data = d;
    p->next = NULL;

    if (n->head == NULL)
    {
        n->head = p;
        n->tail = p;
        return;
    }

    n->tail->next = p;
    n->tail = n->tail->next;
    n->size++;
    return;
}

void printNum(number n)
{
    node *p = n.head;
    if (n.head == NULL)
    {
        printf("Number is empty\n");
        return;
    }

    while (p)
    {
        printf("%d", p->data);
        p = p->next;
    }
    printf("\n");
    return;
}
