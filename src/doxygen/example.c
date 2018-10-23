/*! \file example.c
    \brief A program for merging two chain lists.
*/

/*! \def list1_num
    \brief A macro stores the max length of list1
*/

/*! \fn void function1(int x, int y);
    \brief A function does nothing!
*/

/*! \fn void produceData();
    \brief A function for data producing.
*/
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>

#include "list.h"

#define list1_num 1000
#define list2_num 1000

int list1[list1_num];
int list2[list2_num];

void function1(int x, int y);
	/*!<
	 * \brief Brief description
	 *
	 * This function do nothing at all!
	 */


void produceData(){
	for(int i = 0; i < list1_num; i++)
		list1[i] = 2 * i + 1;
	for(int i = 0; i < list2_num; i++)
		list2[i] = 2 * i;
}

int main(){
	produceData();
	Node *result_head=NULL, *result_tail=NULL /*!< head and tail pointer*/;
	int pos1 = 0, pos2 = 0;
	while(pos1 < list1_num || pos2 < list2_num){
		if(list1[pos1] < list2[pos2]){
			addResult(&result_head, &result_tail, list1[pos1]);
			pos1++;
		}
		else if(list1[pos1] > list2[pos2]){
			addResult(&result_head, &result_tail, list2[pos2]);
			pos2++;
		}
		else{
			addResult(&result_head, &result_tail, list1[pos1]);
			pos1++;
			pos2++;
		}
	}
	for(Node* p = result_head; p != NULL; p = p->next){
		double r = sqrt((double)(p->val));
		printf("%lf\n",r);
	}
	for(Node* p = result_head; p != NULL; p = p->next){
		free(p);
	}
	result_head = result_tail = NULL;
	return 0;
}

// vim:ts=4:sw=4
