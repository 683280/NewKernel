include ../../../makehead
SRC		=	.

C_SRCS	=	$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c)
A_SRCS	=	$(wildcard $(SRC)/*.asm)

C_OBJ	=	$(patsubst %.c,%.o,$(C_SRCS))
A_OBJ	=	$(patsubst %.asm,%.o,$(A_SRCS))

OBJS	=	$(foreach file,$(notdir $(C_OBJ)),build/$(file)) $(foreach file,$(notdir $(A_OBJ)),build/$(file))

CFLAGS	=	-I./inc -m32 -s -std=c99 -fno-stack-protector -O2 -Wall
AR		=	x86_64-elf-ar
all:link

link: libc.o
	@echo -e "\033[32m  \033[1mLD\033[21m    \033[34mkernel.o\033[0m"

libc.o: $(C_OBJ) $(A_OBJ)

$(C_OBJ): %.o: %.c
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o $(notdir $@) $<

$(A_OBJ): %.o: %.asm
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(NASM) -f elf -o build/$(notdir $@) $<
	@$(NASM) -f elf -o $(notdir $@) $<

clean:
	@rm -f $(C_OBJ)
	@rm -f $(A_OBJ)