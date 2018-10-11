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

/**
 * @author bookug
 * @function add one element to the result list
 * @param the address of the head pointer of the result list
 * @param the address of the tail pointer of the result list
 * @param the value that is going to be added into the result list
 * @return no value returned here
 */
void addResult(Node** _p_result_head, Node** _p_result_tail, int _val)
{
    //printf("check %d\n",_val);
    //The size of Node struct is 16 Bytes in x64 system due to alignment
    //printf("size %d\n", sizeof(Node));
    Node* t = (Node*)malloc(sizeof(Node));
    t->val = _val;
    t->next = NULL;
    //add the new node to the tail
    //if we add each item in the head, then the list will be in reverse order
    if(NULL == *_p_result_head)
    //if(*_p_result_head == NULL)
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

