#include <stdio.h>

int func(int* n);

int main(void)
{
    int *a = NULL;
    
    func(a);
    
    return 0;
}

int func(int* n){
    *n = 0;
    
    return 0;
}
