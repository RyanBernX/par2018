/*
 * ===========================================================================
 *
 *       Filename:  th.c
 *
 *    Description:  test source file for POSIX thread
 *
 *        Version:  1.0
 *        Created:  12/20/2018 11:36:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Haoyang Liu (), liuhaoyang@pku.edu.cn
 *   Organization:  BICMR, Peking University
 *      Copyright:  Copyright (c) 2018, Haoyang Liu
 *
 * ===========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void * h(void *arg){
    printf("I am a thread.\n");
    return NULL;
}

int main(){
    void * parg;
    pthread_t pth;
    pthread_create(&pth, NULL, &h, NULL);
    sleep(1);
    printf("I am the major thread.\n");
    pthread_join(pth, &parg);

    return 0;
}
