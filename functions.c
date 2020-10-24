#include "functions.h"

number addNums(number n1, number n2)
{
    node *p = n1.tail;
    node *q = n2.tail;
    int i = n1.size;
    int j = n2.size;
    if (i > j)
    {
        while (i > j)
        {
            pushToNumber(&n2, 0);
            j++;
        }
    }
    else
    {
        while (j > i)
        {
            pushToNumber(&n1, 0);
            i++;
        }
    }
    int carry = 0;
    number sumNum;
    initNumber(&sumNum);
    while (p && q)
    {
        int sum = p->data + q->data + carry;
        carry = sum / 10;
        pushToNumber(&sumNum, sum % 10);
        p = p->prev;
        q = q->prev;
    }
    return sumNum;
}