#include <stdio.h>
#include<string.h>


int main(void)
{
    char* txt ="abc  5";
    printf("%li\n%li\n", strspn(txt," "), strcspn(txt," "));
}

