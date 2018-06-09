vhd.exe disk build/boot.o 0
vhd.exe disk build/loader.bin 1
vhd.exe disk build/kernel 20
"G:\Program Files\Bochs-2.6.8/bochsdbg.exe" -q -f bochsrc.bxrc
