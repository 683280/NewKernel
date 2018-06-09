cp src/kernel/build/kernel isodir/boot
grub-mkrescue -o myos.iso isodir/
qemu-system-i386 -cdrom myos.iso -m 50m