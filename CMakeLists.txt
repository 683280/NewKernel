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
        src/kernel/io/vga.c
        src/kernel/sys/sys_call.c
        src/kernel/sys/ipc.c
        src/kernel/sys/sys.c
        src/kernel/mm/alloc.c src/include/mm/alloc.h
        src/arch/init_cpu.c
        src/usr/usr.c
        src/kernel/sched/sched.c
        src/kernel/mm/string.c
        src/kernel/devices/ramdev.c
        src/kernel/mm/symbols.c src/include/mm/symbols.h
        src/kernel/video/print.c src/include/video/print.h
        src/usr/task.c src/include/mm/string.h
        src/kernel/init/init.c      src/include/init.h
        src/kernel/sched/exec.c src/include/sched/exec.h
        src/kernel/fs2/devfs.c src/include/devices/device.h src/include/fs/devfs.h
        src/kernel/fs/ext2/ext2.c src/include/fs/ext2/ext2.h
        src/kernel/devices/ata.c src/include/devices/ata.h
        src/kernel/fs2/fsman.c
        src/kernel/fs2/initramfs.c  src/include/initramfs.h
        src/kernel/fs/vfs.c src/include/fs/vfs.h
        src/kernel/itr/itr.c src/include/itr/itr.h src/kernel/mm/paging.c src/kernel/sys/process.c src/include/sys/process.h src/kernel/devices/device.c src/arch/x86/irq.c src/include/x86/irq.h src/kernel/itr/pic.c src/include/itr/pic.h src/kernel/devices/serial.c src/include/devices/serial.h)

add_executable(NewKernel ${SOURCE_FILES})