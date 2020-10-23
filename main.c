#include "list.h"
#include <stdio.h>
#include <string.h>

int main()
{
    number n;
    initNumber(&n);
    char exp[256];
    scanf("%[^\n]%*c", exp);
    int i = 0;
    while (exp[i] != '\0')
    {
        if (exp[i] < '0' || exp[i] > '9')
            return 0;
        int num = exp[i] - '0';
        addToNumber(&n, num);
        i++;
    }
    printNum(n);
}