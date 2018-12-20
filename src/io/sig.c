/*
 * ===========================================================================
 *
 *       Filename:  sig.c
 *
 *    Description:  test source code for POSIX signal
 *
 *        Version:  1.0
 *        Created:  12/20/2018 11:23:04 AM
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
#include <signal.h>
#include <unistd.h>

void h(int i){
    printf("I got a signal TERM.\n");
}

int main(){

    signal(SIGTERM, &h);

    do {
        sleep(2);
        printf("I am sleeping ... \n");
    } while (1);

    return 0;
}
