ENTRY (s_kernel_entry)
SECTIONS{
    . = 0x00100000;
    kern_start = .;
    vchars = 0xb8000;
    .text :{
        *(.multiboot)
        *(.text)
    }
    .rodata ALIGN (0x1000) : {
        *(.rodata)
    }

    .data ALIGN (0x1000) : {
        *(.data)
    }
    .bss ALIGN (0x1000): {
        sbss = .;
        *(COMMON)
        *(.bss)
        *(.pte_desc)
        ebss = .;
    }
}