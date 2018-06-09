SRC		=	.

C_SRCS	=	$(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*/*.c) $(wildcard $(SRC)/*/*/*.c)
A_SRCS	=	$(wildcard $(SRC)/*.asm)

C_OBJ	=	$(patsubst %.c,%.o,$(C_SRCS))
A_OBJ	=	$(patsubst %.asm,%.ao,$(A_SRCS))

OBJS	=	$(foreach file,$(notdir $(OBJ)),build/$(file))

CC 		:= 	x86_64-elf-gcc
LD 		:= 	x86_64-elf-ld
OBJCOPY := 	x86_64-elf-objcopy
NASM	:= 	nasm

CFLAGS	=	-I../include -m32 -s -std=c99 -fno-stack-protector

all:link

link: $(C_OBJ) $(A_OBJ)
	@echo -e "\033[32m  \033[1mNASM\033[21m    \033[34m$(C_OBJ)\033[0m"
	@echo $(LD) -m elf_i386 -e entry_loader -o boot.bin build/$(C_OBJ) build/$(A_OBJ)
	@$(LD) -m elf_i386 -e entry_loader -o boot.bin ./build/boot.ao ./build/loader.ao ./build/loader.o

$(C_OBJ): %.o: %.c
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	@$(CC) $(CFLAGS) -c -o build/$(notdir $@) $<

$(A_OBJ): %.ao: %.asm
	@echo -e "\033[32m  \033[1mCC\033[21m    \033[34m$<\033[0m"
	@echo $(CC) $(CFLAGS) -c -o $@ $<
	@$(NASM) -f elf -o build/$(notdir $@) $<

clean:
	@rm -f $(C_OBJ)
	@rm -f $(A_OBJ)