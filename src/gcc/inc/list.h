/*=============================================================================
# Filename: list.h
# Author: bookug
# Mail: bookug@qq.com
# Last Modified: 2018-10-09 08:13
# Description: declaration of list in C
=============================================================================*/

//Add this to avoid multiple definitions when the header file is included by many source files
#ifndef _MY_LIST_H
#define _MY_LIST_H

#include <stdio.h>
#include <math.h> 
#include <stdlib.h> 

typedef struct MyNode
{
    int val;
    struct MyNode* next;
}Node;

void addResult(Node** _p_result_head, Node** _p_result_tail, int _val);

//Add the MACRO in comments to indicate the control block of 'endif'
#endif  //_MY_LIST_H
//NOTICE: we had better add an empty line in the end of each file to avoid compile errors

