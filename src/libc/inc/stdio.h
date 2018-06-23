//
// Created by Carl on 2018/6/13.
//

#ifndef NEWKERNEL_STDIO_H
#define NEWKERNEL_STDIO_H

#include <type.h>
#include <print.h>
#include "file.h"

FILE* fopen(const char* filename,const char* _mode);
int	 fseek(FILE *, long, int);
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
#endif //NEWKERNEL_STDIO_H
