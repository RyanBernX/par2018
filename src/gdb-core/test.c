#include <stdio.h>
#include <malloc.h>
#define LEN 50
#define LEN_STATIC 80
#define LEN_RESULT 100 

int func(int n);

int main(void){
    int j;
    int *array;
    int array_static[LEN_STATIC] = {0};
    long result;
    
    j = 0;
    array = NULL;	
    array = (int*)malloc(LEN * sizeof(int));
    result = 0;
    
    for (j = 1; j < LEN_RESULT; ++j){
        result += j;
    }
    
    if (array != NULL){
        for (j = 0; j < LEN; ++j){
            array[j] = j * 2;
        }
    }
    else {
        printf("array is a NULL pointer.\n");
        return 1;
    }
	
    for (j = 0; j < LEN_STATIC; ++j){
        array_static[j] = j * 3;
    }
	
    printf("result[1-100] = %ld \n", result);
    printf("result[1-250] = %d \n", func(250));
	
    free(array);
	
    return 0;
}




int func(int n){
    int sum, i;
    sum = 0;
	
    for(i = 0; i < n; ++i){
        sum += i;
    }
	
    return sum;
}
