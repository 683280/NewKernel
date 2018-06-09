//
// Created by 13342 on 2017/8/12.
//

#ifndef NEWKERNEL_FBASE_H
#define NEWKERNEL_FBASE_H

#include "type.h"

//超级块
struct SuperBlock {
    char m_Number[9];                  //序列号
    char m_IsFormatted;//文件是否格式化标记
    u32 m_Used;//已用数据块数
    u32 m_Free;//未用数据块
    u32 m_FreeFcb;//第一个空闲文件控制块的块号
//    list<Index> m_FullFcb;//第一个文件控制块的块号
    u32  m_FreeIb;                              //第一个空闲文件信息块的块号
};
int init_fs();
int read(void* add,int i);
#endif //NEWKERNEL_FBASE_H
