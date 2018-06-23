//
// Created by Carl on 2018/6/13.
//
#include <stdio.h>
#include <sys.h>

FILE* fopen(const char* filename,const char* _mode){

}
int fseek(FILE *stream, long size, int count){
    size_t _size;
    _size = size * count;
    if(_size <= 0 || !stream || _size > stream->size )return 0;

    stream->offset = _size;
}

size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream){
    size_t _size;
    _size = size * count;
    if(_size <= 0 || !buffer || !stream)return 0;


}
