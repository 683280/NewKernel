//
// Created by Carl on 2018/6/20.
//

#include <devices/pci/pci.h>
#include <io/io.h>
#include <video/chr.h>

#define CONFIG_ADDRESS  0xCF8
#define CONFIG_DATA     0xCFC

uint32_t pciConfigReadWord (uint8_t bus, uint8_t device, uint8_t func, uint8_t reg)
{
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    lbus = (lbus & 0xff) << 16;

    uint32_t ldevice = (uint32_t)device;
    ldevice = (ldevice & 0x1f) << 11;

    uint32_t lfunc = (uint32_t)func;
    lfunc = (lfunc & 0x7) << 8;

    uint32_t lreg = (uint32_t)reg;
    lreg = (lreg & 0x77) << 2;

//    dprintf("bus = 0x%08x   ldevice = 0x%08x   lfunc = 0x%08x   reg = 0x%08x\n",lbus,ldevice,lfunc,lreg);
    uint32_t tmp = 0;

    /* create configuration address as per Figure 1 */
    address = (uint32_t)(lbus | ldevice | lfunc | lreg | ((uint32_t)0x80000000));
//    dprintf("address = 0x%08x\n",address);
    /* write out the address */
    outl (CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (inl(CONFIG_DATA));
    return (tmp);
}
//uint16_t pciCheckVendor(uint8_t bus, uint8_t slot)
//{
//    uint16_t vendor, device;
//    /* try and read the first configuration register. Since there are no */
//    /* vendors that == 0xFFFF, it must be a non-existent device. */
//    if ((vendor = pciConfigReadWord(bus,slot,0,0)) != 0xFFFF) {
//        device = pciConfigReadWord(bus,slot,0,2);
//    } return (vendor);
//}
//int getVendorID(uint8_t bus,uint8_t device,uint8_t function){
//
//}

void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {
}
void checkDevice(uint8_t bus, uint8_t device) {
    uint8_t function = 0;

    int vendorID = pciConfigReadWord(bus, device, function,0) & 0xffff;
    if(vendorID == 0xFFFF) {
        return;        // Device doesn't exist
    }
    dprintf("device exist  bus = %d , device = %d\n",bus,device);
    checkFunction(bus, device, function);
    int headerType = pciConfigReadWord(bus, device, function,3) >> 16;
    if( (headerType & 0x80) != 0) {
        /* It is a multi-function device, so check remaining functions */
        for(function = 1; function < 8; function++) {
            if(pciConfigReadWord(bus, device, function,0) != 0xFFFF) {
                checkFunction(bus, device, function);
            }
        }
    }
}

void checkAllBuses(void) {
    uint16_t bus;
    uint8_t device;

    for(bus = 0; bus < 256; bus++) {
        for(device = 0; device < 32; device++) {
            checkDevice(bus, device);
        }
    }
}
void read_pci_header(uint8_t bus, uint8_t device,PCI_Header * pci_header){
    u32* header = pci_header;
    u32 a = 0;
    for (int i = 0; i < 4 ; i++){
        *header++ = a = pciConfigReadWord(bus,device,0,i);
//        dprintf("a = %08x\n",a);
    }
}
void print_pci_header(PCI_Header* pci_header){
    dprintf("pci:\n");
    dprintf("vendor_id         = %04x        device_id       = %04x\n",pci_header->vendor_id,pci_header->device_id);
    dprintf("command           = %04x        status          = %04x\n",pci_header->command,pci_header->status);
    dprintf("revision_id       = %02x        prog_if         = %02x\n",pci_header->revision_id,pci_header->prog_if);
    dprintf("subclass          = %02x        class_code      = %02x\n",pci_header->subclass,pci_header->class_code);
    dprintf("cache_line_size   = %02x        latency_timer   = %02x\n",pci_header->cache_line_size,pci_header->latency_timer);
    dprintf("header_type       = %02x        BIST            = %02x\n",pci_header->header_type,pci_header->BIST);
}
int pci_init(){
    checkAllBuses();
//    PCI_Header header;
//    for (int i = 0; i < 4; ++i) {
//        read_pci_header(0,i,&header);
//        print_pci_header(&header);
//        dprintf("\n");
//    }
    
}
