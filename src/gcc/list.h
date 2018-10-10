/*=============================================================================
# Filename: list.h
# Author: bookug
# Mail: bookug@qq.com
# Last Modified: 2018-10-09 08:13
# Description: declaration of list in C
=============================================================================*/

#ifdef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 

typedef struct MyNode
{
    int val;
    struct MyNode* next;
}Node;

void addResult(Node** _p_result_head, Node** _p_result_tail, int _val);

#endif  //_LIST_H

