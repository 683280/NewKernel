ENTRY (s_kernel_entry)
SECTIONS{
    . = 0x00100000;
    kern_start = .;
    vchars = 0xb8000;
    .text BLOCK(4K) : ALIGN(4K){
        *(.multiboot)
        *(.text)
    }
    kern_end = .;
    .rodata BLOCK(4K) : ALIGN(4K){
        *(.rodata)
    }
    .data BLOCK(4K) : ALIGN(4K){
        *(.data)
    }
    .bss BLOCK(4K) : ALIGN(4K){
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.pte_desc)
        ebss = .;
    }
}