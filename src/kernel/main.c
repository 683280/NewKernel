//
// Created by carljay on 17-7-31.
//

#include <video/chr.h>
#include <x86/idt.h>
#include <io/cmode.h>
#include <asm.h>
#include <sched/sched.h>
#include <init/init.h>
#include <multiboot.h>
#include <mm/paging.h>
#include <fs/vfs.h>
#include <fs/ext2/ext2.h>
#include <devices/ata.h>
#include <mm/string.h>
#include <devices/serial.h>
#include <x86/gdt_tool.h>
#include <mm/mm.h>
#include <sched/proc.h>
#include <mm/alloc.h>
#include <devices/graphics/vga.h>
#include <mm/symbols.h>
#include <devices/pci/pci.h>
#include "graphics/graphics.h"

void run_init(void* init){
    PROCESS* pro;
    int i;
    i = get_program(&pro);
    if(i < 0){
        printf("kernel error : can't start init task");
        return ;
    }
    pro->pid = i;
    int code = add_gdt(getDescriptor(0x0,0xfffff,setDpl(type_code,3)));
    pro->regs.cs = code;

    u32 data = add_gdt(getDescriptor(0x0,0xfffff,setDpl(type_data,3)));

    u32 vir_start;
    u32* cr3 = alloc_cr3(init,0x1000,&vir_start);

//    dprintf("init = 0x%08x\n",init);
    u32 esp = ((u32)alloc_vir_page(cr3,1)) + 0xfff;
//    dprintf("cr3 = 0x%08x  *cr3[4] = 0x%08x   vir_start = 0x%08x   esp = 0x%08x\n",cr3,*(cr3 + 4),vir_start,esp);
    pro->regs.cr3 = cr3;
    pro->regs.ss = data;
    pro->regs.es = data;
    pro->regs.fs = data;
    pro->regs.kernel_esp = 0x1000;
    pro->regs.gs = data;
    pro->regs.ds = data;
    pro->regs.eip= vir_start;
    pro->regs.esp= esp;//(u32) alloc_page(16) + (10 * 0x1000);
//    dprintf("pro->regs.esp = 0x%08x\n",pro->regs.esp);
    pro->regs.eflags = 0x1202;
    pro->priority = 10;
    pro->time_film = 1;
    load_gdt(&lgdt);
    p_curr_proc = pro;
    memcpy(&pro->p_name,"init",4);
}
ext2_private_t root_p = {
        .inode  =   2,
};
atadev_private_t ata_data = {
        .start_lba  =   2050,
        .end_lba    =   18432
};
inode_t root = {
        .name   =   "/",
        .dev    =   &atadev,
        .fs     =   &ext2fs,
        .type   =   FS_MOUNTPOINT,
        .p      =   &root_p,
        .d_data =   &ata_data,
};
void init_fs(){
    vfs_mount_root(&root);
}
void kmain(multiboot_info_t * mbd, int magic){
    printf("enter_kernel\n");
    printf("memory mem_lower = %d , memory mem_upper = %d \n",mbd->mem_lower,mbd->mem_upper);
    printf("loader magic = %x\n" ,magic);
    if(magic == MULTIBOOT_BOOTLOADER_MAGIC){
        printf("<<<<<<<<<<<<<GRUB LOAD Kernel>>>>>>>>>>>>>\n");
    }
    init_serial(SERIAL_COM1);
    paging_init(mbd->mem_lower,mbd->mem_upper);
    init_fs();
//    pci_init();
//    init_vga();
//    struct _window window;
//    alloc_window(100,100,100,100,&window);
//    window_draw_rect(&window,3,0,0,640,480);
//    vga_update_screen();
//    kprintf("test vga\n");
    run_init(&init2);
//    run_init(&init);
    extern void entry_init();

    entry_init();
//    sti
    while(1){
        _asm_volatile("hlt")
    }
}

