#include <stdio.h>
#include <malloc.h>
#include "defs.h"

int main(void){
    point    *p;
    
    p = (point*)malloc(sizeof(point));
    if (p != NULL){
        p->x = 0;
        p->y = 0;
    }
    else {
        printf("p is a NULL pointer.\n");
        return 1;
    }

    input(p);
    display(p);
    
    free(p);
    
    return 0;
}
