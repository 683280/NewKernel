//
// Created by Carl on 2018/6/13.
//

#ifndef LIBC_FILE_H
#define LIBC_FILE_H
typedef struct _sFile{
    size_t fid;
    size_t offset;
    size_t size;
    short flag;
}FILE;
#endif //LIBC_FILE_H
