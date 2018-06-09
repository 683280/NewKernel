#make
#./vhd disk build/boot.o 0
#./vhd disk build/loader.bin 1
./vhd disk src/kernel/build/kernel 20
sudo bochs -q -f b