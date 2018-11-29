/*
 * ===========================================================================
 *
 *       Filename:  gdb.c
 *
 *    Description:  call debugger
 *
 *        Version:  1.0
 *        Created:  11/29/2018 10:40:33 AM
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
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int call_debugger(){

    char cmd[2048];
    pid_t pid = getpid();

    /* you will need xterm to run the following line */
    //sprintf(cmd, "xterm -e gdb -p %d", pid);
    /* you will need tmux to run the following line */
    sprintf(cmd, "tmux new-session gdb -p %d", pid);
    return system(cmd);
}

int main(){
    call_debugger();
    return 0;
}

