//
// Created by Carl on 2018/5/24.
//

#include <fs/vfs.h>
#include <sched/proc.h>
#include <mm/alloc.h>
#include <video/chr.h>
#include "sched/exec.h"

int sys_exec(int edx,char* filename, int flag, PROCESS* curr){
    int i;
    char* _filename = vir_to_physics(curr->regs.cr3,filename);
    dprintf("cr3 = 0x%08x   buf = 0x%08x   _buf 0x%08x  %s\n",curr->regs.cr3,filename,_filename,_filename);
    inode_t* file_inode =  vfs_open(_filename);
    dprintf("filename 0x%08x    _filename 0x%08x    size %d  flag %d \n",file_inode,_filename,file_inode->size,flag);
    char* buf = malloc(file_inode->size);
    file_inode->fs->read(file_inode,0,file_inode->size,buf);
    dprintf("%s\n",buf);
    return i;
}