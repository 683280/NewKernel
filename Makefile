include makehead

SRC		=	.

ARCH	= 	x86

CFLAGS	=	-Isrc/include -m32 -s -std=c99 -fno-stack-protector

KERNEL_DIR	=	./src/kernel
KERNEL_SRC	=	$(wildcard $(KERNEL_DIR)/*.c) $(wildcard $(KERNEL_DIR)/*/*.c) $(wildcard $(KERNEL_DIR)/*/*/*.c)

all:link

link: src/kernel/kernel.o src/arch/$(ARCH)_arch.o $(KERNEL_SRC)
	@echo $(LD) -m elf_i386 -T kernel.ld -o kernel_$(ARCH) src/arch/$(ARCH)_arch.o src/kernel/kernel.o
	@$(LD) -m elf_i386 -T kernel.ld -r -o kernel_$(ARCH) src/arch/$(ARCH)_arch.o src/kernel/kernel.o
	@echo -e "\033[32m  \033[1mScript\033[21m    \033[34m symbols\033[0m"
	@scripts/symbols > symbols.c
	@echo -e "\033[32m  \033[1mBuild\033[21m    \033[34m symbols\033[0m"
	@$(CC) $(CFLAGS) -c -o symbols.o symbols.c
	@$(LD) -m elf_i386 -T kernel.ld -o kernel_$(ARCH) src/arch/$(ARCH)_arch.o src/kernel/kernel.o symbols.o
	@cp kernel_$(ARCH) isodir/boot/kernel
	@grub-mkrescue -o myos.iso isodir/

src/kernel/kernel.o:
	@echo -e "\033[32m  \033[1mBUILD\033[21m    \033[34m kernel\033[0m"
	@cd src/kernel; make CC=$(CC) ARCH=$(ARCH)

src/arch/$(ARCH)_arch.o:
	@echo -e "\033[32m  \033[1mBUILD\033[21m    \033[34m $(ARCH)\033[0m"
	@cd src/arch; $(MAKE) CC=$(CC) ARCH=$(ARCH)

run:
	@qemu-system-i386 -cdrom myos.iso -m 50m -fda disk.img -boot c

bochs:
	@bochs -q -f c

build_run:
	@make -B
	@bochs -q -f c
clean:
	@rm -f src/arch/build/*
	@rm -f src/arch/*_arch.o
	@rm -f src/kernel/build/*
	@rm -f src/kernel/kernel.o

#DIRS := $(shell find . -maxdepth 3 -type d)
#CUR_SOURCE1 = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
#ASM_SOURCE1 = $(foreach dir,$(DIRS),$(wildcard $(dir)/*.asm))
#NOT_KERNEL = %/boot.asm %/loader.asm %/loader.c %/feature_tests.c
#ASM_SOURCE = $(filter-out $(NOT_KERNEL),$(ASM_SOURCE1))
#CUR_SOURCE = $(filter-out $(NOT_KERNEL),$(CUR_SOURCE1))
#$(warning $(ASM_SOURCE))
##CUR_SOURCE=${wildcard *.c}
#CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}
#ASM_OBJS=${patsubst %.asm, %.ao, $(ASM_SOURCE)}
#
#OBJS=$(CUR_OBJS) $(ASM_OBJS)
#BUILD_FILE=$(foreach file,$(notdir $(OBJS)),build/$(file))
# #${patsubst build/boot.ao, , $(BUILD_FILE)}
#kernel:$(CUR_OBJS) $(ASM_OBJS) boot.ao loader.bin disk
#	$(LD) -m elf_i386 -T kernel.ld -o build/kernel $(BUILD_FILE)
#boot.ao:src/boot/boot.asm
#	nasm -o build/boot.o src/boot/boot.asm
#loader.bin:src/boot/loader.asm src/boot/loader.c
#	nasm -f elf -o build/loader.ao src/boot/loader.asm
#	$(GCC) -I ./src/include/ -m32 -s -std=c99 -fno-stack-protector -c -o build/loader.oo src/boot/loader.c
#	$(LD) -m elf_i386 -e entry_loader -o build/loader.o build/loader.ao build/loader.oo
#	$(OBJCOPY) -O binary build/loader.o build/loader.bin
#disk:
#    dd if=/dev/zero of=disk bs=1m count=100
#
#all:$(ASM_OBJS)
#$(ASM_OBJS):%.ao:%.asm
#	nasm -f elf -o build/$(notdir $@) $^
#all:$(CUR_OBJS)
#$(CUR_OBJS):%.o:%.c
#	$(GCC) -I ./src/include/ -m32 -ggdb -g -std=c99 -c -o build/$(notdir $@) $^