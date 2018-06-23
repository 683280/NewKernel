//
// Created by Carl on 2018/6/20.
//

#ifndef NEWKERNEL_PCI_H
#define NEWKERNEL_PCI_H

#include <type.h>

typedef struct _pci_header{
    u16 vendor_id;
    u16 device_id;
    u16 command;
    u16 status;
    u8 revision_id;
    u8 prog_if;
    u8 subclass;
    u8 class_code;
    u8 cache_line_size;
    u8 latency_timer;
    u8 header_type;
    u8 BIST;

}PCI_Header;
int pci_init();
#endif //NEWKERNEL_PCI_H
