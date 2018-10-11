/*=============================================================================
# Filename: list.c
# Author: bookug
# Mail: bookug@qq.com
# Last Modified: 2018-10-09 08:13
# Description: implementation of list in C
=============================================================================*/

#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 
#include "list.h"

void addResult(Node** _p_result_head, Node** _p_result_tail, int _val)
{
    //printf("check %d\n",_val);
    //printf("size %d\n", sizeof(Node));
    Node* t = (Node*)malloc(sizeof(Node));
    t->val = _val;
    t->next = NULL;
    //add the new node to the tail
    if(*_p_result_head == NULL)
    {
        *_p_result_head = t;
        *_p_result_tail = *_p_result_head;
    }
    else
    {
        (*_p_result_tail)->next = t;
        *_p_result_tail = t;
    }
}

