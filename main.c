#include "functions.h"
#include <stdio.h>
#include <string.h>

int main()
{
    number n;
    number n1;
    number n2;
    initNumber(&n);
    initNumber(&n1);
    initNumber(&n2);
    addToNumber(&n1, 1);
    addToNumber(&n1, 2);
    addToNumber(&n1, 3);
    printNum(n1);
    addToNumber(&n2, 4);
    addToNumber(&n2, 5);
    addToNumber(&n2, 6);
    printNum(n2);
    n = addNums(n1,n2);
    printNum(n);
}
