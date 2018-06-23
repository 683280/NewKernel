include ../../makehead
SRC		=	src

C_SRCS	=	$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
A_SRCS	=	$(wildcard $(SRC)/*.asm)

C_OBJ	=	$(patsubst %.c,%.o,$(C_SRCS))
A_OBJ	=	$(patsubst %.asm,%.ao,$(A_SRCS))

OBJS	=	$(foreach file,$(notdir $(C_OBJ)),build/$(file)) $(foreach file,$(notdir $(A_OBJ)),build/$(file))

CFLAGS	=	-I./inc -m32 -s -std=c99 -fno-stack-protector -O2 -Wall
AR		=	x86_64-elf-ar
all:link

link: libc.o
	@echo -e "\033[32m  \033[1mLD\033[21m    \033[34mkernel.o\033[0m"
	@echo @$(LD) -m elf_i386 -r -o libc.o $(OBJS)
	@${AR} rcs libc.a build/*.o

libc.o: $(C_OBJ) $(A_OBJ)

$(C_OBJ): %.o: %.c
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o build/$(notdir $@) $<

$(A_OBJ): %.ao: %.asm
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(NASM) -f elf -o build/$(notdir $@) $<
	@$(NASM) -f elf -o build/$(notdir $@) $<

clean:
	@rm -f $(C_OBJ)
	@rm -f $(A_OBJ)