#include <stdio.h>

int main()
{
    for(int i = 0; i< 16386; i++)
    {
        printf("%d = (`%C') = %X %c", i, i, i, (i%5==0)?'\n':'\t');
    }
}