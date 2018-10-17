#include <stdio.h>
#include "defs.h"

void input(point *p){
    printf("Input the value of x,y\nx = \n");
    scanf("%lf",&p->x);
    printf("y = \n");
    scanf("%lf",&p->y);
    return;
}

void display(point *p){
    printf("Output:\nx + y = %lf\n", p->x + p->y);
    return;
}
