###############################################################
# Configuration file for Bochs（Linux）
###############################################################

# filename of ROM images
romimage: file=$BXSHARE/BIOS-bochs-latest

# VGAROMIMAGE
# You now need to load a VGA ROM BIOS into C0000.
vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest

# what disk images will be used
floppya: 1_44=disk, status=inserted
ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14
ata0-master: type=cdrom, path="myos.iso", status=inserted
ata0-slave: type=none
# choose the boot disk.
boot: cdrom
# where do we send log messages?
log: bochsout.txt

# disable the mouse
mouse: enabled=0

# enable key mapping, using US layout as default.  注意map的路径
#keyboard_mapping: enabled=0, map=/usr/local/Cellar/bochs/2.6.9_1/share/bochs/keymaps/x11-pc-us.map

# how much memory the emulated machine will have
megs: 32