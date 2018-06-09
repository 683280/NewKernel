vhd.exe disk build/boot.o 0
vhd.exe disk build/loader.bin 1
vhd.exe disk build/kernel 20
qemu-system-x86_64.exe -m 512 -hda build/kernel