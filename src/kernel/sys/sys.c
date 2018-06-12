//
// Created by Carl on 2018/4/30.
//

#include "sys/sys.h"

int exit(int a, int c, int b, PROCESS *pro){
    proc_list[pro->pid].p_flag = 0;
}