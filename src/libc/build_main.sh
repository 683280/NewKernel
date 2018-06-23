#nasm -f elf -o crt0.o crt0.asm
x86_64-elf-gcc -o main main.c
#x86_64-elf-ld -m elf_i386 -e _start -o main libc.a main.o crt0.o
