/*=============================================================================
# Filename: example.c
# Author: bookug
# Mail: bookug@qq.com
# Last Modified: 2018-10-09 08:13
# Description: union two sorted lists, and apply the sqrt operator on each item
# For debug, gcc -lm -g inc/list.c example.c -o run.exe
# To release, gcc -O2 -lm inc/list.c example.c -o run.exe
=============================================================================*/

#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 
#include <limits.h> 
//Another choice is to use 'inc/list.h' here
#include "list.h"

//A safe way to release memory
#define xfree(p) free(p);p=NULL;

#define list1_num 1000
#define list2_num 1000

//this is placed in .data segment, and it can be assigned larger space than function stack(1M by default). For larger space, use heap memory instead
int list1[list1_num];
int list2[list2_num];

/**
 * @author bookug
 * @email bookug@qq.com
 * @function prepare the data for two lists
 * @param no parameters needed
 * @return no value returned here
 */
void produceData()
{
    for(int i = 0; i < list1_num; ++i)
    {
        list1[i] = 2 * i + 1;
    }

    for(int i = 0; i < list2_num; ++i)
    {
        list2[i] = 2 * (i+1);
    }
}

/**
 * @author bookug
 * @email bookug@qq.com
 * @function the main function of C program
 * @param number of arguments in the command, including the program itself
 * @param all argument strings
 * @return no value returned here
 */
int main(int argc, const char* argv[])
{
    produceData();

    Node *result_head = NULL, *result_tail = NULL;
    int pos1 = 0, pos2 = 0;
    //NOTICE: this strategy not always fails, for example, the final elements of the two lists are the same
    while(pos1 < list1_num && pos2 < list2_num)
    //while(pos1 < list1_num && pos2 < list2_num)
    {
        /*printf("check %d %d\n", pos1, pos2);*/
        if(list1[pos1] < list2[pos2])
        {
            addResult(&result_head, &result_tail, list1[pos1]);
            pos1++;
        }
        else if(list1[pos1] > list2[pos2])
        {
            addResult(&result_head, &result_tail, list2[pos2]);
            pos2++;
        }
        else   //equal case, only add once
        {
            addResult(&result_head, &result_tail, list1[pos1]);
            pos1++;
            pos2++;
        }
    }

    while(pos1 < list1_num)
    {
        addResult(&result_head, &result_tail, list1[pos1]);
        pos1++;
    }
    while(pos2 < list2_num)
    {
        addResult(&result_head, &result_tail, list2[pos2]);
        pos2++;
    }

    //NOTICE: the declaration Node* p=... in for-loop is only allowed in C99 standard
    //output each item in result after sqrt operation
    for(Node* p = result_head; p != NULL; p = p->next)
    {
        //NOTICE: sqrt(8) works but sqrt(n) must be compiled by -lm
        double r = sqrt((double)(p->val));
        printf("%lf\n", r);
    }

    //NOTICE: another bug exists here, but error is not reported when running
    //printf("%p %p\n", result_head, result_tail);
    //release the result list
    for(Node* p = result_head; p != NULL; )
    {
        Node* tmp = p->next;
        free(p);
        p = tmp;
        //NOTICE: this set will cause segment fault immediately
        //p = NULL;
        //printf("%p\n", p);
    }
    //clear the list pointers, head and tail
    result_head = result_tail = NULL;

    return 0;
}

