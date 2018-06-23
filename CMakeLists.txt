cmake_minimum_required(VERSION 3.7.2)
project(NewKernel)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_FLAGS -nostdinc)
include_directories(src/include)
set(SOURCE_FILES
        src/kernel/video/chr.c
        src/arch/x86/gdt.c
        src/kernel/main.c
        src/kernel/mm/mm.c
        src/kernel/tools/gdt_tool.c
        src/boot/loader.c
        src/arch/x86/idt.c
        src/kernel/io/io.c
        src/kernel/io/cmode.c
        src/kernel/sys/sys_call.c
        src/kernel/sys/ipc.c
        src/kernel/sys/sys.c
        src/kernel/mm/alloc.c src/include/mm/alloc.h
        src/arch/init_cpu.c
        src/kernel/sched/sched.c
        src/kernel/sched/proc.c src/include/sched/proc.h
        src/kernel/mm/string.c
        src/kernel/devices/ramdev.c
        src/kernel/mm/symbols.c src/include/mm/symbols.h
        src/kernel/video/print.c src/include/video/print.h
        src/include/mm/string.h
        src/kernel/init/init.c src/include/init/init.h
        src/kernel/sched/exec.c src/include/sched/exec.h
        src/kernel/fs2/devfs.c src/include/devices/device.h src/include/fs/devfs.h
        src/kernel/fs/ext2/ext2.c src/include/fs/ext2/ext2.h
        src/kernel/devices/ata.c src/include/devices/ata.h
        src/kernel/fs2/fsman.c
        src/kernel/fs2/initramfs.c src/include/fs/initramfs.h
        src/kernel/fs/vfs.c src/include/fs/vfs.h
        src/kernel/itr/itr.c src/include/itr/itr.h
        src/kernel/mm/paging.c  src/kernel/devices/device.c
        src/arch/x86/irq.c src/include/x86/irq.h
        src/kernel/itr/pic.c src/include/itr/pic.h
        src/kernel/devices/serial.c src/include/devices/serial.h
        src/kernel/sched/elf.c src/kernel/sys/file.c src/include/sys/file.h
        src/kernel/devices/graphics/vga.c src/include/devices/graphics/vga.h
        src/kernel/devices/graphics/modes.c src/kernel/devices/graphics/en_font.c
        src/kernel/graphics/graphics.c src/kernel/graphics/graphics.h src/kernel/devices/pci/pci.c src/include/devices/pci/pci.h)

add_executable(NewKernel ${SOURCE_FILES})