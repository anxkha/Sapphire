@ECHO OFF

set path=%path%;D:\Programming\bin

echo Compiling...
nasm -felf -omain.obj ke\i386\main.asm
nasm -fwin32 -olowlevel.o ke\i386\lowlevel.asm

gcc -c -nostdinc -fno-builtin -I"include" io\*.c
gcc -c -nostdinc -fno-builtin -I"include" mm\*.c
gcc -c -nostdinc -fno-builtin -I"include" ps\*.c
gcc -c -nostdinc -fno-builtin -I"include" ke\*.c
gcc -c -nostdinc -fno-builtin -I"include" ke\i386\*.c
gcc -c -nostdinc -fno-builtin -I"include" rtl\*.c
gcc -c -nostdinc -fno-builtin -I"include" vfs\*.c

echo Linking...
ld --nostdlib --enable-stdcall-fixup -Tlink.txt -o kernel.bin main.obj *.o
objcopy -O binary kernel.bin sapphire.e

echo Done!
del *.o, *.obj, *.bin
