//
// Created by carljay on 17-7-31.
//

#include <chr.h>
#include <idt.h>
#include <vga.h>
#include <asm.h>
#include <task.h>
#include <init.h>
#include <multiboot.h>
#include <mm.h>
#include <paging.h>
#include <alloc.h>
#include <vfs.h>
#include <ext2.h>
#include <ata.h>
#include <string.h>

void run_init(){
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

//    u32 page_catalog = get_page();//页目录
//    u32 page_table = get_page();
//    (int*)page_catalog = page_table & 7;

//    pro->regs.cr3 = page_catalog;
    pro->regs.ss = data;
    pro->regs.es = data;
    pro->regs.fs = data;
    pro->regs.kernel_esp = 0x1000;
    pro->regs.gs = data;
    pro->regs.ds = data;
    pro->regs.eip= (u32)init;
    pro->regs.esp= (u32) 0x45000;
    pro->regs.eflags = 0x1202;
    pro->priority = 10;
    pro->time_film = 10;
    load_gdt(&lgdt);
    p_curr_proc = pro;
    memcpy(&pro->p_name,"init",4);
}
void kmain(multiboot_info_t * mbd, int magic){
    printf("enter_kernel\n");
    printf("memory mem_lower = %d , memory mem_upper = %d \n",mbd->mem_lower,mbd->mem_upper);
    printf("loader magic = %x\n" ,magic);
    if(magic == MULTIBOOT_BOOTLOADER_MAGIC){
        printf("GRUB LOAD Kernel\n");
    }
    paging_init(639,50048);

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
//    vfs_create(&root,"/",&root);
//    int* buf = malloc(512);
//    s_ata_read_sectors(0,1,buf);

//    for (int i = 0; i < 512 / 4; ++i) {
//        printf("0x%08x    ",*buf);
//        if ((i + 1 % 4) == 0){
//            printf("\n");
//        }
//        buf++;
//    }
//    vfs_mount()

    inode_t *root_fs;
    if(!(root_fs = ext2fs.load(&root))){
        printf("load error");
    }
    inode_t* inode = root_fs->list->head;
    printf("load end\n");
    char* buf = malloc(20);
    for (int i = 0; i < root_fs->list->count; ++i) {
//        printf("%s \n", inode->name);
        if (i == 3){
            printf("read %s   filesize= %d  p = %08x  inode = %d \n",inode->name,inode->size,inode->p,((ext2_private_t*)inode->p)->inode);
            ext2fs.read(inode,0,20,buf);
            printf("           %s\n",buf);
        }
        inode = inode->next;
    }
//    root_fs->c_time

//    vfs
//    init_mm(mbd->mem_lower,mbd->mem_upper);
//    run_init();
//    s_lba_load_sector(0,1,&buf);
//    sti
    while(1){
        _asm_volatile("hlt")
    }
}

